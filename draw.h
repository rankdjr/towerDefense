#ifndef _DRAW_H_
#define _DRAW_H_

#include <GL/glx.h>
#include "image.h"
#include "global.h"

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

void init_texture_alpha(Image *img, int r, int g, int b)
{
	unsigned char *alphaData = buildAlphaData(img,r,g,b);
	//intialize textures for background
	glGenTextures(1, &(img->texid));
	glBindTexture(GL_TEXTURE_2D, img->texid);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, img->width, img->height, 0,
							GL_RGB, GL_UNSIGNED_BYTE, alphaData);
	delete alphaData;
}

void init_graphics()
{
	//intialize tile textures
	init_texture(&grass);
	init_texture(&dirt);
	//intialize tower textures
	init_texture(&towerBasic);
	//init_texture_alpha(&towerBasic,0,0,0);
	//initialize enemy textures
	init_texture(&enemyBasic);
	//init_texture_alpha(&enemyBasic, 0, 0, 0);
}

void drawQuad(float x, float y, float width, float height) 
{
	glPushMatrix();
    glTranslatef(x, y, 0);
	glBegin(GL_QUADS);
		glVertex2i(0,     0);
		glVertex2i(0,     height);
		glVertex2i(width, height);
		glVertex2i(width, 0);
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

void drawQuadTexAlpha(Image img, float x, float y, float width, float height) 
{
	//draw texture to quad with alpha channel
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
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
	glDisable(GL_ALPHA_TEST);
}

void drawCircle(float x, float y, int radius)
{
	int i;
	static int firstTime=1;
	static float verts[32][2];
	static int n=32;
	if (firstTime) {
		float ang=0.0;
		float inc = 3.14159 * 2.0 / (float)n;
		for (i=0; i<n; i++) {
			verts[i][0] = sin(ang);
			verts[i][1] = cos(ang);
			ang += inc;
		}
		firstTime=0;
	}

	glPushMatrix();
	glTranslatef(x, y, 0);
	glBegin(GL_TRIANGLE_FAN);
		for (i=0; i<n; i++) {
			glVertex2f(verts[i][0]*radius, verts[i][1]*radius);
		}
	glEnd();
	glPopMatrix();
}
#endif //_DRAW_H_
