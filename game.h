#ifndef _GAME_H_
#define _GAME_H_

#include "enemy.h"
#include "TileGrid.h"
double timeDiff(struct timespec *start, struct timespec *end);
void timeCopy(struct timespec *dest, struct timespec *source);

class Game {
public:
    int (*currMap)[10];
    int numEnemies, enemiesalive;
    Enemy enemy[30];
    
    Game();
    void initEnemies(int);
    void killEnemy(Enemy *enemy);
    void pathContinues(TileGrid grid);
    void sortEnemiesByDistance();
    void checkCurrEnemy();
    void updateTowerActions();
} game;

Game::Game() {
    //
    //enemy vars
    numEnemies = 10;
    enemiesalive = 0;
    //
    //map vars
    // int map[10][10] = { 
	// 	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	// 	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	// 	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	// 	{0, 0, 0, 1, 1, 1, 1, 1, 0, 0},
	// 	{0, 0, 0, 1, 0, 0, 0, 1, 0, 0},
	// 	{8, 1, 1, 1, 0, 0, 0, 1, 1, 9},
	// 	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	// 	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	// 	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	// 	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	// };

    int map[10][10] = { 
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
		{0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
		{8, 1, 1, 1, 0, 1, 0, 1, 1, 9},
		{0, 0, 0, 0, 0, 1, 0, 1, 0, 0},
		{0, 0, 0, 1, 1, 1, 0, 1, 0, 0},
		{0, 0, 0, 1, 0, 0, 0, 1, 0, 0},
		{0, 0, 0, 1, 1, 1, 1, 1, 0, 0},
	};
    currMap = map;
    grid.setMap(currMap);
    for (int i = 0; i < g.mapHeight; i++) {
        for (int j = 0; j<g.mapWidth; j++) {
            if (map[i][j] != 0)
                grid.pathDist += 64;
        }
    }
}   

void Game::initEnemies(int numEnemies) { 
    // static int i = 0;
    // if (i < numEnemies) {
    //     enemy[i].x = grid.startTile.x;
    //     enemy[i].y = grid.startTile.y;
    //     enemy[i].width = 48;
    //     enemy[i].height = 48;
    //     enemy[i].speed = 0.5+(0.025*i);
    //     enemy[i].health = 100;
    //     enemy[i].dir = 0;
    //     enemy[i].alive =1;
    //     i++;
    // } else {
    //     i = 0;
    // }


    for( int i = 0; i<numEnemies; i++) {
        enemy[i].x = grid.startTile.x;
        enemy[i].y = grid.startTile.y;
        enemy[i].width = g.enemy_pxSize;
        enemy[i].height = g.enemy_pxSize;
        enemy[i].health = enemy[i].maxHealth;
        enemy[i].alive = 1;
        enemy[i].speed = 0.5+(0.025*i);
        enemy[i].dir = 0;
        enemiesalive++;
    }
}

void Game::pathContinues(TileGrid grid){ 
    for (int i = 0; i<numEnemies; i++){
        switch(enemy[i].dir)
        {
            //right
            case 0:
            {
                Tile *myTile = (grid.getTile((enemy[i].x/g.tile_pxSize), (int)(enemy[i].y/g.tile_pxSize)));
                Tile *nextTileX = grid.getTile((int)(enemy[i].x/g.tile_pxSize) +1, (int)(enemy[i].y/g.tile_pxSize));
                Tile *nextTileY = grid.getTile((int)(enemy[i].x/g.tile_pxSize) , (int)(enemy[i].y/g.tile_pxSize)+1);
                //cout << nextTileX ->type << endl;
                if (((nextTileX->type != myTile->type) && (nextTileX->type != 2)) && (nextTileY-> type != myTile-> type))
                {
                    enemy[i].dir = 3;
                }
      
                else if (((nextTileX->type != myTile->type) && (myTile->type !=8 && myTile->type != 9)) && (nextTileY-> type == myTile-> type))
                {
                    enemy[i].dir = 1;
                }
                break;
            }
            //down        
            case 1:
            {
                Tile *myTile = (grid.getTile((enemy[i].x/g.tile_pxSize), (int)(enemy[i].y/g.tile_pxSize)));
                Tile *nextTileX = grid.getTile((int)(enemy[i].x/g.tile_pxSize) +1, (int)(enemy[i].y/g.tile_pxSize));
                Tile *nextTileY = grid.getTile((int)(enemy[i].x/g.tile_pxSize) , (int)(enemy[i].y/g.tile_pxSize)+1);
  
                if (((nextTileY->type != myTile->type) && myTile->type !=8) && (nextTileX->type == myTile->type)){
                    enemy[i].dir = 0;
               }
               else if (((nextTileY->type != myTile->type) && myTile->type !=8) && (nextTileX-> type != myTile->type))
               {
               enemy[i].dir = 2;
               }
               break;
            }
            //left                
            case 2:
            {                    
                Tile *myTile = (grid.getTile((enemy[i].x/g.tile_pxSize), (int)(enemy[i].y/g.tile_pxSize)));
                Tile *nextTileX = grid.getTile((int)((enemy[i].x+48)/g.tile_pxSize)-1 , (int)(enemy[i].y/g.tile_pxSize));
                Tile *nextTileY = grid.getTile((int)(enemy[i].x/g.tile_pxSize) , (int)(enemy[i].y/g.tile_pxSize)+1); 
                if (((nextTileX->type != myTile->type) && (nextTileX->type != 2)) && (nextTileY-> type == myTile-> type))
                {
                    enemy[i].dir = 1;
                }
      
                else if (((nextTileX->type != myTile->type) && (myTile->type !=8 && myTile->type != 9)) && (nextTileY-> type != myTile-> type))
                {
                    enemy[i].dir = 3;
                }
                break;
            }
            
            //up
            case 3:
            {
                Tile *myTile = (grid.getTile((enemy[i].x/g.tile_pxSize), (int)((enemy[i].y)/g.tile_pxSize)));
                Tile *nextTileX  = grid.getTile((int)(enemy[i].x/g.tile_pxSize)+1 , (int)(enemy[i].y/g.tile_pxSize));
                Tile *nextTileY = grid.getTile((int)(enemy[i].x/g.tile_pxSize) , (int)((enemy[i].y + 48)/g.tile_pxSize)-1);
           
                if (((nextTileY->type != myTile->type) && (myTile->type != 8 && myTile ->type != 9)) && (nextTileX->type == myTile->type))
                {   
                    enemy[i].dir = 0;  
                }
                else if (((nextTileY->type != myTile->type) && (myTile->type !=8 && myTile->type!= 9)) && (nextTileX-> type != myTile->type))
                {
                    enemy[i].dir = 2;
                }
                 break;
            } 


        }
    }
}

void Game::killEnemy(Enemy *enemy)
{   
    enemy->width = 0;
    enemy->height = 0;
    enemy->alive = 0;
    enemy->speed = 0;
    enemy->x = -100;
    enemy->y = -100;
    enemy->distToEnd = 0;
    enemiesalive--;
}

void Game::sortEnemiesByDistance()
{
    //bubble sort enemies in descending order by distance to end tile
    if (enemiesalive > 1) {
        for (int i = 0; i < numEnemies-1; i++) {
            for (int j = i+1; j < numEnemies; j++) {
                if (enemy[i].distToEnd < enemy[j].distToEnd) {
                    Enemy temp = enemy[j];
                    enemy[j] = enemy[i];
                    enemy[i] = temp;
                    //printf("swap %i\n",j);
                }
            }
        }  
    }
}

void Game::checkCurrEnemy()
{
    //loop through towers and:
    //1. check range of towers to current enemy and reset when necessary
    //2. update all nullptr enemies to active enemies
    for (long unsigned int i = 0; i < player.towers.size(); i++) {
        if (player.towers[i].currEnemy != nullptr) {
            //check distance of enemy
            float dx = player.towers[i].cx - ((player.towers[i].currEnemy->x)+g.enemy_pxSize/2);
            float dy = player.towers[i].cy - ((player.towers[i].currEnemy->y)+g.enemy_pxSize/2);
            float dist = sqrt(dx*dx + dy*dy);
            if (dist > player.towers[i].range) {
                //enemy is out of range, reset curr enemy ptr
                player.towers[i].currEnemy = nullptr;
            }
        } else {
            //update all nullptr enemies in tower vector
            sortEnemiesByDistance();
            for (int j = numEnemies; j >= 0; --j) {
                float dx = player.towers[i].cx - (enemy[j].x+ g.enemy_pxSize/2);
                float dy = player.towers[i].cy - (enemy[j].y+ g.enemy_pxSize/2);
                float dist = sqrt(dx*dx + dy*dy);
                //printf("{ %i, %f }, ", j, dist);
                if (dist < player.towers[i].range) {
                    player.towers[i].setCurrEnemy(&enemy[j]);
                    //printf("NAE  --  tower[%li] --> enemy[%i]\n", i, j);
                    j = 0;
                }
            }
        }
    }
}

void Game::updateTowerActions()
{
    checkCurrEnemy();
    //Loop though towers + attack
    for (long unsigned int i = 0; i < player.towers.size(); i++) {
        if (player.towers[i].currEnemy) {
            //tower has a currEnemy;
            player.towers[i].attackEnemy();
            if (player.towers[i].currEnemy->health < 0) {
                //printf("EK\t--  range: %f,  dist: %f\n", player.towers[i].range, dist);
                game.killEnemy(player.towers[i].currEnemy);
                player.towers[i].currEnemy = nullptr;
            }
        }
    }
}

#endif //_GAME_H_
