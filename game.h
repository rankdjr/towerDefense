#ifndef _GAME_H_
#define _GAME_H_

#include "enemy.h"
#include "TileGrid.h"

class Game {
public:
    int (*map)[10];
    int numEnemies;
    Enemy enemy[30];
    
    Game() {
        numEnemies = 5;
    }   

    void initEnemies(int numEnemies) {  
        for( int i = 0; i<numEnemies; i++){
            enemy[i].x = grid.startTile.x-64;
            enemy[i].y = grid.startTile.y;
            enemy[i].width = 48;
            enemy[i].height = 48;
            enemy[i].speed = 0.5+(0.10*i);
            enemy[i].dir = 0;
        }
    }
} game;

#endif //_GAME_H_