//modified by: Douglas Rank
//date: Spring of 2022
//
// Credit to Gordon Griesel for X11 code, libggfontsa, timers.cpp, and timers.h
//
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <X11/Xlib.h>
#include <X11/extensions/Xdbe.h>
#include <GL/glx.h>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <GL/glx.h>
#include <math.h>
#include "fonts.h"
#include "global.h"
#include "image.h"
#include "draw.h"
#include "enemy.h"
#include "tower.h"
#include "tile.h"
#include "TileGrid.h"
#include "player.h"
#include "game.h"
#include "X11wrapper.h"

//Function prototypes
void physics(void);
void render(void);
void initEnemies(int enemies);

//Setup timers
const double physicsRate = 1.0 / 30.0;
const double oobillion = 1.0 / 1e9;
struct timespec timeStart, timeCurrent;
struct timespec timePause;
double physicsCountdown = 0.0;
double timeSpan = 0.0;

double timeDiff(struct timespec *start, struct timespec *end) 
{
    return (double)(end->tv_sec - start->tv_sec ) +
            (double)(end->tv_nsec - start->tv_nsec) * oobillion;
}

void timeCopy(struct timespec *dest, struct timespec *source) 
{
    memcpy(dest, source, sizeof(struct timespec));
}

int main()
{
	init_opengl();
	init_graphics();
    initialize_fonts();
    clock_gettime(CLOCK_REALTIME, &timePause);
    clock_gettime(CLOCK_REALTIME, &timeStart);
    //
	//manually declare map
	int map[10][10] = { 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 1, 1, 1, 1, 1, 0, 0},
		{0, 0, 0, 1, 0, 0, 0, 1, 0, 0},
		{8, 1, 1, 1, 0, 0, 0, 1, 1, 9},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	};
	//initialize map
	grid.setMap(map);
    //game.initEnemies(game.numEnemies);

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
        //
        game.pathContinues(grid);
        clock_gettime(CLOCK_REALTIME, &timeCurrent);
        timeSpan = timeDiff(&timeStart, &timeCurrent);
        timeCopy(&timeStart, &timeCurrent);
        physicsCountdown += timeSpan;
        while(physicsCountdown >= physicsRate) {
            physics();
            physicsCountdown -= physicsRate;
        }
        
		render();            //draw things
		x11.swapBuffers();   //make video memory visible
		usleep(1000);        //pause to let X11 work better
	}
    cleanup_fonts();
	return 0;
}

void physics()
{
  if (g.gameState == PLAYING)     
    {
        for (int i = 0; i<game.numEnemies; i++)
        {
            switch(game.enemy[i].dir)
            { 
                case 0:
                    game.enemy[i].x += game.enemy[i].speed;
                    break;
                case 1:
                    game.enemy[i].y += game.enemy[i].speed;
                    break;
                /*    
                case 2: 
                    game.enemy[i].x -= game.enemy[i].speed;
                    break;
                */
                case 3:
                    game.enemy[i].y -= game.enemy[i].speed;
                    break;
            }
        }   
    }
}

void render()
{
	grid.draw();

    for(int i = 0; i<game.numEnemies; i++){
        game.enemy[i].Draw();
    }

	if (g.showTowerRange) {
		int mapi = g.xMousePos/64;
		int mapj = 9-g.yMousePos/64;
		Tile *t = grid.getTile(mapi,mapj);
		t->tower.showRange();
	}

	if (g.buildState) {
		//get tile based off of mouse position
		grid.drawTileOutline();
	}

    //temporary code to test if function can access enemies and towers
    if(!player.towers.empty()) {
        game.updateTowerActions();
    }

    //Game information rendered at top right of screen
    //draw backdrop for text on screen
    static int recWidth = 100;
    static int recHeight = 60;
    static int xpos = g.xres-recWidth-10;
    static int ypos = g.yres-recHeight-10;
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0, 0, 0, 0.70);
    drawQuad(xpos, ypos, recWidth, recHeight);
    glDisable(GL_BLEND);
    //print game text
    Rect r;
	r.left = xpos+5;
	r.bot = ypos+recHeight-25;
	r.center = 0;
    sprintf(player.strHp,    "Health: %i", player.hp);
	sprintf(player.strFunds, "Gold:    %i", player.funds);
	ggprint12(&r, 20, x11.set_color_3i(255, 255, 0), "%s", player.strHp);
	ggprint12(&r, 20, x11.set_color_3i(255, 255, 0), "%s", player.strFunds);
	
}
