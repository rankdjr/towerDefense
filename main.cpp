//modified   by: Douglas Rank and Kenneth Wood
//date: Spring of 2022
//
// Credit to Gordon Griesel for X11 code, libggfontsa, timers.cpp, timers.h, and portions of the main loop in main.cpp
// as well as the basis of the OpenGL compenents in draw.h
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
#include "timers.h"
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
void doGameLogic(void);
void applyPhysics(void);
void physics(void);
void render(void);
void initEnemies(int enemies);

//Setup timers
const double physicsRate = 1.0 / 60.0;
const double oobillion = 1.0 / 1e9;
struct timespec timeStart, timeCurrent, towerAtkStart;
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
        doGameLogic();      //function to hold game logic
        applyPhysics();     //apply physics at consistent rate
		render();           //draw things
		x11.swapBuffers();  //make video memory visible
		usleep(1000);       //pause to let X11 work better
	}
    cleanup_fonts();
	return 0;
}

void doGameLogic()
{
    game.updateTowerActions();
    game.pathContinues(grid);
    
    for (int i = 0; i<game.numEnemies; i++)
    {
        Tile *myTile = (grid.getTile((game.enemy[i].x/g.tile_pxSize), (int)(game.enemy[i].y/g.tile_pxSize)));
        if ( myTile -> type == 2)
        {
            game.killEnemy(&game.enemy[i]);
            player.updateHP(1);
        }
    } 
}

void applyPhysics()
{
    clock_gettime(CLOCK_REALTIME, &timeCurrent);
    timeSpan = timeDiff(&timeStart, &timeCurrent);
    timeCopy(&timeStart, &timeCurrent);
    physicsCountdown += timeSpan;
    while(physicsCountdown >= physicsRate) {
        physics();
        physicsCountdown -= physicsRate;
    }
}

void physics()
{
  if (g.gameState == PLAYING)     
    {
        for (int i = 0; i<game.numEnemies; i++)
        {
            game.enemy[i].distToEnd = grid.pathDist - game.enemy[i].speed;
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
    //draw map
	grid.draw();

    //draw enemies
    for(int i = 0; i<game.numEnemies; i++){
        game.enemy[i].draw();
    }

    //draw towers
    if(!player.towers.empty()) {
        for (int i = 0; i < (int)player.towers.size(); i++) {
            player.towers[i].draw();                
        }
    }

    //show tower range; flag set in x11.checkMouse() (3)right-click
	if (g.showTowerRange) {
		int mapi = g.xMousePos/g.tile_pxSize;
		int mapj = 9-g.yMousePos/g.tile_pxSize;
        const int towerLvl = 0;
        const int towerIndex = 1;
        int towerId = mapi*10 + mapj;
        int towerExists = player.towerHash[towerId][towerLvl];
		if (towerExists) {
            int vecIndex = player.towerHash[towerId][towerIndex]; // get index of tower in player.towers vector
            player.towers[vecIndex].showRange();
        }
	}

    //show tile outlines; flag set in x11.checkKeys() (b)build/(s)sell
	if (g.buildState) {
		//get tile based off of mouse position
		grid.drawTileOutline();
	}

    //Game information rendered at top right of screen
    //draw backdrop for text on screen
    static int recWidth = 100;
    static int recHeight = 40;
    static int xpos = g.xres-recWidth-10;
    static int ypos = g.yres-recHeight-10;
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0, 0, 0, 0.70);
    drawQuad(xpos, ypos, recWidth, recHeight);
    glDisable(GL_BLEND);
    //print game text
    Rect r;
	r.left = xpos+10;
	r.bot = ypos+recHeight-15;
	r.center = 0;
    sprintf(player.strHp,    "Health:  %i", player.hp);
	sprintf(player.strFunds, "Gold:    %i", player.funds);
	ggprint8b(&r, 20, set_color_3i(255, 255, 0), "%s", player.strHp);
	ggprint8b(&r, 20, set_color_3i(255, 255, 0), "%s", player.strFunds);
}
