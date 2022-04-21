#ifndef _X11wrapper_H_
#define _X11wrapper_H_

/*
	This code was originally written by Gordon Griesel
	of California State Univ, Bakersfield

	Modifed by Douglas Rank and Kenneth wood in Spring of 2022
*/

#include <iostream>
#include <X11/Xlib.h>
#include <X11/extensions/Xdbe.h>
#include <GL/glx.h>
#include "global.h"
#include "player.h"
using namespace std;

// X11 setup --------------------------------------------------------------------------------------

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
	void drawText(int x, int y, char *text);
	unsigned long set_color_3i(int r, int g, int b);
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
	//printf("Xdbe version %d.%d\n",major,minor);
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
// end x11 setup --------------------------------------------------------------------------------
//

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
		if(g.showTowerRange)
			g.showTowerRange = 0;
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button was pressed.
			if (g.buildState == BUY) {
				player.addTower(g.xMousePos, g.yMousePos);
			} 
			else if (g.buildState == SELL) {
				player.removeTower(g.xMousePos, g.yMousePos);
			}
			return;
		}
		if (e->xbutton.button==3) {
			//Right button was pressed.
			int mapi = g.xMousePos/g.tile_pxSize;
			int mapj = 9-g.yMousePos/g.tile_pxSize;
			int key = mapi*10 + mapj;
			if (player.towerHash[key])
				g.showTowerRange = 1;
			return;
		}
	}
	if (e->type == MotionNotify) {
		//The mouse moved!
		if (savex != e->xbutton.x || savey != e->xbutton.y) {
			savex = e->xbutton.x;
			savey = e->xbutton.y;
			//Code placed here will execute whenever the mouse moves.
			//printf("mouse moved\n");
			g.xMousePos = (int)savex;
			g.yMousePos = (int)savey;
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
			case XK_b:
				if(g.buildState != BUY) {
					g.buildState = BUY;
					break;
				}
				g.buildState = NONE;
				break;
			case XK_s:
				game.initEnemies(game.numEnemies);
				g.gameState = PLAYING;
				break;
            case XK_k:
                game.killEnemy(&game.enemy[0]);
                break;
            case XK_x:
				if(g.buildState != SELL) {
					g.buildState = SELL;
					break;
				}
				g.buildState = NONE;
				break;
			case XK_Escape:
				return 1;
		}
	}
	return 0;
}

#endif //_X11wrapper_H_
