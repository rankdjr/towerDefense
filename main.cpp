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
#include <math.h>
#include "fonts.h"
#include "global.h"
#include "image.h"
#include "draw.h"
#include "tower.h"
#include "tile.h"
#include "TileGrid.h"
#include "X11wrapper.h"
#include "enemy.h"


using namespace std;

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

const int numEnemies = 5;
// xpos, ypos, width, height, speed, direction
Enemy enemy[numEnemies];

int main()
{
	init_opengl();
	init_graphics();
    initialize_fonts();
    initEnemies(numEnemies);

    clock_gettime(CLOCK_REALTIME, &timePause);
    clock_gettime(CLOCK_REALTIME, &timeStart);

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
        //
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

void initEnemies(int numEnemies)
{  
    for( int i = 0; i<numEnemies; i++){
        enemy[i].x = -64;
        enemy[i].y = 4*64;
        enemy[i].width = 48;
        enemy[i].height = 48;
        enemy[i].speed = 0.5+(0.10*i);
        enemy[i].dir = 0;
    }
}



void physics()
{
    // int i;    
    // static struct timespec towerTime;
    // static int firsttime=1;
    // if (firsttime) {
    //     firsttime=0;
    //     clock_gettime(CLOCK_REALTIME, &towerTime);
    //  }
    // struct timespec tt;
    // clock_gettime(CLOCK_REALTIME, &tt);
    // double timeSpan = timeDiff(&towerTime, &tt);
    // if (timeSpan < .15)
    //     return;
    // timeCopy(&towerTime, &tt);
    
    
    for (int i = 0; i<numEnemies; i++){
        if(enemy[i].dir == 0) {
            enemy[i].x += enemy[i].speed;
        }
    }
}
void render()
{
	grid.draw();

    for(int i = 0; i<numEnemies; i++){
        enemy[i].Draw();
    }

	if (g.showTowerRange) {
		int mapi = g.xMousePos/64;
		int mapj = 9-g.yMousePos/64;
		Tile *t = grid.getTile(mapi,mapj);
		t->tower.showRange();
	}

	if (g.gameState == BUILD) {
		//get tile based off of mouse position
		grid.drawTileOutline();
	}

}
