//modified by: Douglas Rank
//date: Spring of 2022
//
// Credit to Gordon Griesel for X11 code, libggfontsa, timers.cpp, and timers.h
//
#include <stdio.h>
#include <iostream>
#include <X11/Xlib.h>
#include <X11/extensions/Xdbe.h>
#include <GL/glx.h>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <GL/glx.h>
#include "fonts.h"
#include "image.h"

using namespace std;

#define DEBUG_ON 0

typedef double Flt;
typedef Flt Vec[2];
#define PI 3.14159265358979
#define SWAP(x,y) (x)^=(y);(y)^=(x);(x)^=(y)
#define rnd() (Flt)rand() / (Flt)RAND_MAX

//Function prototypes
void physics(void);
void render(void);
void drawQuadTex(Image img, float x, float y, float width, float height);

struct vector {
	float x,y,z;
};

class Tile {
public:
	float x, y, width, height;
	Image *texture;
	bool build, path;
	Tile() {}
	Tile(Image *img, float x, float y, float width, float height) {
		texture = img;
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	void draw() {
		drawQuadTex(*texture, x, y, width, height);
	}
};

class Global {
public:
	int xres, yres;
	unsigned int texid[2];
    Image *grass;
	Image *dirt;
    Global() {
        xres = 1280;
        yres = 960;
    }
	~Global() {
		delete [] grass;
		delete [] dirt;
	}
} g;

class TileGrid {
public:
	Tile **map;
	TileGrid() {
		map = new Tile*[20];
		for (int i = 0; i < 20; i++) {
			map[i] = new Tile[15];
			for (int j = 0; j < 15; j++) {
				map[i][j] = Tile(g.grass, i*64, j*64, 64, 64);
				map[i][j].draw();
			}
		}
	}

};
// X11 code --------------------------------------------------------------------------------------

class X11_wrapper {
private:
	Display *dpy;
	Window win;
	GLXContext glc;
	GC gc;
	XdbeBackBuffer backBuffer;
	XdbeSwapInfo swapInfo;
public:
	~X11_wrapper();
	X11_wrapper();
	void set_title();
	bool getXPending();
	XEvent getXNextEvent();
	void swapBuffers();
	void reshape_window(int width, int height);
	void check_resize(XEvent *e);
	void check_mouse(XEvent *e);
	int check_keys(XEvent *e);
	void set_color_3i(int r, int g, int b);
	void drawPoint(int x, int y);
} x11;


X11_wrapper::~X11_wrapper()
{
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

X11_wrapper::X11_wrapper()
{
	int major, minor;
	XdbeBackBufferAttributes *backAttr;
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	int w = g.xres, h = g.yres;
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		cout << "\n\tcannot connect to X server\n" << endl;
		exit(EXIT_FAILURE);
	}
	Window root = DefaultRootWindow(dpy);
	XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
	if (vi == NULL) {
		cout << "\n\tno appropriate visual found\n" << endl;
		exit(EXIT_FAILURE);
	} 
	Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	XSetWindowAttributes swa;
	swa.colormap = cmap;
	swa.event_mask =
		ExposureMask | KeyPressMask | KeyReleaseMask |
		ButtonPress | ButtonReleaseMask |
		PointerMotionMask |
		StructureNotifyMask | SubstructureNotifyMask;
	win = XCreateWindow(dpy, root, 0, 0, w, h, 0, vi->depth,
		InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
	//Create gc
	gc = XCreateGC(dpy, win, 0, NULL);
	//Get DBE version
	if (!XdbeQueryExtension(dpy, &major, &minor)) {
		fprintf(stderr, "Error : unable to fetch Xdbe Version.\n");
		XFreeGC(dpy, gc);
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
	}
	printf("Xdbe version %d.%d\n",major,minor);
	//Get back buffer and attributes (used for swapping)
	backBuffer = XdbeAllocateBackBufferName(dpy, win, XdbeUndefined);
	backAttr = XdbeGetBackBufferAttributes(dpy, backBuffer);
	swapInfo.swap_window = backAttr->window;
	swapInfo.swap_action = XdbeUndefined;
	XFree(backAttr);
	//Map and raise window
	set_title();
	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
}

void X11_wrapper::set_title()
{
	//Set the window title bar.
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "Tower Defense Project");
}

bool X11_wrapper::getXPending()
{
	//See if there are pending events.
	return XPending(dpy);
}

XEvent X11_wrapper::getXNextEvent()
{
	//Get a pending event.
	XEvent e;
	XNextEvent(dpy, &e);
	return e;
}

void X11_wrapper::swapBuffers()
{
	glXSwapBuffers(dpy, win);
}

void X11_wrapper::reshape_window(int width, int height)
{
	//window has been resized.
	g.xres = width;
	g.yres = height;
	//
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
}

void X11_wrapper::check_resize(XEvent *e)
{
	//The ConfigureNotify is sent by the
	//server if the window is resized.
	if (e->type != ConfigureNotify)
		return;
	XConfigureEvent xce = e->xconfigure;
	if (xce.width != g.xres || xce.height != g.yres) {
		//Window size did change.
		reshape_window(xce.width, xce.height);
	}
}

// end x11 code --------------------------------------------------------------------------------
//

void init_opengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, g.xres, g.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	//Set 2D mode (no perspective)
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
    //
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //Set the screen background color
	glClearColor(0.1, 0.1, 0.1, 1.0);
    glEnable(GL_TEXTURE_2D);
}

void init_texture(Image *img)
{
	//intialize textures for background
	glGenTextures(1, &(img->texid));
	glBindTexture(GL_TEXTURE_2D, img->texid);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, img->width, img->height, 0,
							GL_RGB, GL_UNSIGNED_BYTE, img->data);
}

void init_graphics()
{
    g.grass = new Image("images/grass.png");
	g.dirt = new Image("images/dirt.png");
	//intialize textures
	init_texture(g.grass);
	init_texture(g.dirt);
}

int main()
{
	init_opengl();
	init_graphics();
    initialize_fonts();
	//main game loop
	int done = 0;
	while (!done) {
		//process events...
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.check_resize(&e);
			x11.check_mouse(&e);
			done = x11.check_keys(&e);
		}
		physics();           //move things
		render();            //draw things
		x11.swapBuffers();   //make video memory visible
		usleep(1000);        //pause to let X11 work better
	}
    cleanup_fonts();
	return 0;
}

//-----------------------------------------------------------------------------

void X11_wrapper::check_mouse(XEvent *e)
{
	static int savex = 0;
	static int savey = 0;

	//Weed out non-mouse events
	if (e->type != ButtonRelease &&
		e->type != ButtonPress &&
		e->type != MotionNotify) {
		//This is not a mouse event that we care about.
		return;
	}
	//
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button was pressed.
            return;
		}
		if (e->xbutton.button==3) {
			//Right button was pressed.
			return;
		}
	}
	if (e->type == MotionNotify) {
		//The mouse moved!
		if (savex != e->xbutton.x || savey != e->xbutton.y) {
			savex = e->xbutton.x;
			savey = e->xbutton.y;
			//Code placed here will execute whenever the mouse moves.


		}
	}
}

int X11_wrapper::check_keys(XEvent *e)
{
	if (e->type != KeyPress && e->type != KeyRelease)
		return 0;
	int key = XLookupKeysym(&e->xkey, 0);
	if (e->type == KeyPress) {
		switch (key) {
			case XK_1:
				//Key 1 was pressed
				break;
			case XK_Escape:
				//Escape key was pressed
				return 1;
		}
	}
	return 0;
}

void drawQuad(float x, float y, float width, float height) 
{
	glPushMatrix();
    glTranslatef(x, y, 0);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex2i(0,     0);
		glTexCoord2f(0, 0); glVertex2i(0,     height);
		glTexCoord2f(1, 0); glVertex2i(width, height);
		glTexCoord2f(1, 1); glVertex2i(width, 0);
	glEnd();
	glPopMatrix();
}

void drawQuadTex(Image img, float x, float y, float width, float height) 
{
    //draw texture to quad
	glColor3ub(255,255,255);
	glBindTexture(GL_TEXTURE_2D, img.texid);
	glPushMatrix();
    glTranslatef(x, y, 0);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex2i(0,     0);
		glTexCoord2f(0, 0); glVertex2i(0,     height);
		glTexCoord2f(1, 0); glVertex2i(width, height);
		glTexCoord2f(1, 1); glVertex2i(width, 0);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void physics()
{

}

void render()
{
	int x , y;
	int width, height;
	x = y = 0;
	width = height = 64;

	for (int i = 0; i < g.xres/64; i++) {
		drawQuadTex(*g.grass, x, y, width, height);
		if (i%2 == 0)
			drawQuadTex(*g.dirt, x, y, width, height);
		x += width;
	}

	glColor3ub(255,255,0);
	drawQuad(540, 540, width, height);

	Tile *tile = new Tile(g.dirt, 604, 540, 64, 64);
	Tile *tile2 = new Tile(g.grass, 180, 180, 64, 64);
	tile->draw();
	tile2->draw();
}
