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
#include "player.h"
#include "tower.h"
#include "tile.h"
#include "TileGrid.h"
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
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
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
    if (g.gameState == PLAYING) {
        for (int i = 0; i<game.numEnemies; i++){
            if(game.enemy[i].dir == 0) {
                game.enemy[i].x += game.enemy[i].speed;
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

	if (g.buildState == 1) {
		//get tile based off of mouse position
		grid.drawTileOutline();
	}

    //temporary code to test if function can access enemies and towers
    if(!player.towers.empty()) {
        //get enemies distance to end tile
        for (int i = 0; i < game.numEnemies; i++) {
            float dx = (grid.endTile.x + g.tileWidth)- game.enemy[i].x;
            float dy = grid.endTile.y - game.enemy[i].y;
            game.enemy[i].distToEnd = dx*dx + dy*dy;
        }
        //
        //print enemy dist to end
        // for (int i = 0; i < game.numEnemies; i++) {
        //     printf("{ %i, %f }, ", i, game.enemy[i].distToEnd);
        // }
        // printf("\n");
        //
        //bubble sort enemies in descending order by distance to end tile
        // if (g.gameState == PLAYING) {
        //     for (int i = 0; i < game.numEnemies; i++) {
        //         for (int j = 1; j < game.numEnemies-1; j++) {
        //             if (game.enemy[i].distToEnd < game.enemy[j].distToEnd) {
        //                 Enemy temp = game.enemy[j];
        //                 game.enemy[j] = game.enemy[i];
        //                 game.enemy[i] = temp;
        //                 printf("swapped\n");
        //             }
        //         }
        //     }   
        // }
        //
        //loop through towers and set currEnemy in each tower object
        for (long unsigned int i = 0; i < player.towers.size(); i++) {
            static int range = player.towers[i].range*player.towers[i].range;
            if (!player.towers[i].currEnemy) {
                for (int j = game.numEnemies; j >= 0; --j) {
                    float dx = player.towers[i].cx - (game.enemy[j].x+24);
                    float dy = player.towers[i].cy - (game.enemy[j].y+24);
                    float dist = dx*dx + dy*dy;
                    //printf("{ %i, %f }, ", j, dist);
                    if (dist < range) {
                        player.towers[i].acquireEnemy(&game.enemy[j]);
                        //printf("tower[%li] --> enemy[%i]\n", i, j);
                        j = 0;
                    }
                }
            }
        }
        for (long unsigned int i = 0; i < player.towers.size(); i++) {
            if (player.towers[i].currEnemy)
                player.towers[i].attackEnemy();
        }    
    }
}
