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
#include "enemy.h"


using namespace std;

//Function prototypes
void physics(void);
void render(void);


//Setup timers
const double physicsRate = 1.0 / 60.0;
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

// xpos, ypos, width, height, speed, direction
Enemy *enemy = new Enemy(0, 5, 48, 48, 2, 0);


int main()
{
	init_opengl();
	init_graphics();
    initialize_fonts();
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
<<<<<<< Updated upstream
        //Below is a process to apply physics at a consistent rate.
        //1. Get the time right now.
=======
        game.pathContinues(grid);
>>>>>>> Stashed changes
        clock_gettime(CLOCK_REALTIME, &timeCurrent);
        //2. How long since we were here last?
        timeSpan = timeDiff(&timeStart, &timeCurrent);
        //3. Save the current time as our new starting time.
        timeCopy(&timeStart, &timeCurrent);
        //4. Add time-span to our countdown amount.
        physicsCountdown += timeSpan;
        //5. Has countdown gone beyond our physics rate?
        //       if yes,
        //           In a loop...
        //              Apply physics
        //              Reducing countdown by physics-rate.
        //              Break when countdown < physics-rate.
        //       if no,
        //           Apply no physics this frame.
        while(physicsCountdown >= physicsRate) {
            //6. Apply physics
            //
            physics();
            //7. Reduce the countdown by our physics-rate
            physicsCountdown -= physicsRate;
        }
		physics();
		render();            //draw things
		x11.swapBuffers();   //make video memory visible
		usleep(1000);        //pause to let X11 work better
	}
    cleanup_fonts();
	return 0;
}

void physics()
{
<<<<<<< Updated upstream
   //  int i;    
    static struct timespec towerTime;
    static int firsttime=1;
    if (firsttime) {
        firsttime=0;
        clock_gettime(CLOCK_REALTIME, &towerTime);
     }
    struct timespec tt;
    clock_gettime(CLOCK_REALTIME, &tt);
    double timeSpan = timeDiff(&towerTime, &tt);
    if (timeSpan < enemy->delay)
        return;
    timeCopy(&towerTime, &tt);
    
    /*
    //move the snake segments...
    int curpos[2];
    int newpos[2];
    int oldpos[2];
    //save the head position.
    curpos[0] = enemy->x;
    curpos[1] = enemy->y;
    //printf("\n%d %d", g.pacman.pos[0][0], g.pacman.pos[0][1]);
    */
    if(enemy->dir == 0)
    enemy->x += 10;

    /*
    newpos[0] = curpos[0];
    newpos[1] = curpos[1];
     for (int i=1; i<1; i++) {
        oldpos[0] = enemy->x;
        oldpos[1] = enemy->y;
        if (enemy->x  == newpos[0] &&
            enemy->y  == newpos[1])
            break;
          
        enemy->x = newpos[0];
        enemy->y = newpos[1];
        newpos[0] = oldpos[0];
        newpos[1] = oldpos[1];
        
     }
     */
=======
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
>>>>>>> Stashed changes
}


void render()
{
	grid.draw();
<<<<<<< Updated upstream
	enemy->Draw();
	if (g.gameState == BUILD) {
=======

    for(int i = 0; i<game.numEnemies; i++){
        
        if (game.enemy[i].alive == 1 ) {
        game.enemy[i].Draw();
        
        }
    }

	if (g.showTowerRange) {
		int mapi = g.xMousePos/64;
		int mapj = 9-g.yMousePos/64;
		Tile *t = grid.getTile(mapi,mapj);
		t->tower.showRange();
	}

	if (g.buildState == 1) {
>>>>>>> Stashed changes
		//get tile based off of mouse position
		grid.drawTileOutline();
	}

}
