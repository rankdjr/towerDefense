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
#include "global.h"
#include "image.h"
#include "draw.h"
#include "tower.h"
#include "tile.h"
#include "TileGrid.h"
#include "X11wrapper.h"


using namespace std;

//Function prototypes
void physics(void);
void render(void);

int main()
{
	init_opengl();
	init_graphics();
    initialize_fonts();
	//manually declare map
	int map[10][10] = { 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
		{0, 0, 0, 1, 1, 0, 1, 0, 0, 0},
		{1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	};
	//initialize map
	grid.setMap(map);

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

void physics()
{

}

void render()
{
	grid.draw();

	if (g.gameState == BUILD) {
		//get tile based off of mouse position
		grid.drawTileOutline();
	}

}
