#ifndef _GAME_H_
#define _GAME_H_

#include <vector>
#include "enemy.h"
#include "TileGrid.h"
double timeDiff(struct timespec *start, struct timespec *end);
void timeCopy(struct timespec *dest, struct timespec *source);

class Game {
public:
    int (*currMap)[10];
    int numEnemies, enemiesalive, baseEnemies;
    int waveCtr, sizeOfWave;
    struct timespec spawnStart, currentTime;
    Enemy enemy[100];
    vector<Enemy> wave;
    
    Game();
    void initEnemies();
    void killEnemy(Enemy *enemy);
    void pathContinues(TileGrid grid);
    void sortEnemiesByDistance();
    void checkCurrEnemy();
    void updateTowerActions();
} game;                     

Game::Game() {
    //initilaize starting values for game start
    sizeOfWave = 5;
    numEnemies = 0;
    waveCtr = 0;
    //
    //initiliaze starting map
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
    //
    //set tilegrid with integer array map
    currMap = map;
    grid.setMap(currMap);
    for (int i = 0; i < g.mapHeight; i++) {
        for (int j = 0; j<g.mapWidth; j++) {
            if (map[i][j] != 0)
                grid.pathDist += 64;
        }
    }
}   

void Game::initEnemies() {
    //
    static int i = 0;
    if (i < sizeOfWave + waveCtr) {
        Enemy *e = new Enemy(grid.startTile.x, grid.startTile.y, 1.5, 0);
        wave.push_back(*e);
        delete e;

        // enemy[i].x = grid.startTile.x;
        // enemy[i].y = grid.startTile.y;
        // enemy[i].width = 48;
        // enemy[i].height = 48;
        // enemy[i].speed = 0.5+(0.025*i);
        // enemy[i].health = 100;
        // enemy[i].dir = 0;
        // enemy[i].alive =1;
        
        i++;
        numEnemies++;
    } else {
        i = 0;
    }


    // for( int i = 0; i<numEnemies; i++) {
    //     enemy[i].x = grid.startTile.x;
    //     enemy[i].y = grid.startTile.y + 24;
    //     enemy[i].width = g.enemy_pxSize;
    //     enemy[i].height = g.enemy_pxSize;
    //     enemy[i].health = enemy[i].maxHealth;
    //     enemy[i].alive = 1;
    //     enemy[i].speed = 0.5+(0.025*i);
    //     enemy[i].dir = 0;
    //     enemiesalive++;
    // }
}

// void Game::pathContinues(TileGrid grid){ 
//     for (int i = 0; i<numEnemies; i++){
//         switch(enemy[i].dir)
//         {
//             //right
//             case 0:
//             {
//                 Tile *myTile = (grid.getTile((enemy[i].x/g.tile_pxSize), (int)(enemy[i].y/g.tile_pxSize)));
//                 Tile *nextTileX = grid.getTile((int)((enemy[i].x- 16)/g.tile_pxSize) +1, (int)(enemy[i].y/g.tile_pxSize));
//                 Tile *nextTileY = grid.getTile((int)(enemy[i].x/g.tile_pxSize) , (int)(enemy[i].y/g.tile_pxSize)+1);
//                 //cout << nextTileX ->type << endl;
//                 if (((nextTileX->type != myTile->type) && (nextTileX->type != 2)) && (nextTileY-> type != myTile-> type))
//                 {
//                     enemy[i].dir = 3;
//                 }
      
//                 else if (((nextTileX->type != myTile->type)) && (nextTileY-> type == myTile-> type))
//                 {
//                     enemy[i].dir = 1;
//                 }
//                 break;
//             }
//             //down        
//             case 1:
//             {
//                 Tile *myTile = (grid.getTile((enemy[i].x/g.tile_pxSize), (int)(enemy[i].y/g.tile_pxSize)));
//                 Tile *nextTileX = grid.getTile((int)(enemy[i].x/g.tile_pxSize) +1, (int)(enemy[i].y/g.tile_pxSize));
//                 Tile *nextTileY = grid.getTile((int)(enemy[i].x/g.tile_pxSize) , (int)((enemy[i].y -16)/g.tile_pxSize)+1);
  
//                 if (((nextTileY->type != myTile->type) && myTile->type !=8) && (nextTileX->type == myTile->type)){
//                     enemy[i].dir = 0;
//                }
//                else if (((nextTileY->type != myTile->type) && myTile->type !=8) && (nextTileX-> type != myTile->type))
//                {
//                enemy[i].dir = 2;
//                }
//                break;
//             }
//             //left                
//             case 2:
//             {                    
//                 Tile *myTile = (grid.getTile((enemy[i].x/g.tile_pxSize), (int)(enemy[i].y/g.tile_pxSize)));
//                 Tile *nextTileX = grid.getTile((int)((enemy[i].x+48)/g.tile_pxSize)-1 , (int)(enemy[i].y/g.tile_pxSize));
//                 Tile *nextTileY = grid.getTile((int)(enemy[i].x/g.tile_pxSize) , (int)(enemy[i].y/g.tile_pxSize)+1); 
//                 if (((nextTileX->type != myTile->type) && (nextTileX->type != 2)) && (nextTileY-> type == myTile-> type))
//                 {
//                     enemy[i].dir = 1;
//                 }
      
//                 else if (((nextTileX->type != myTile->type) && (myTile->type !=8 && myTile->type != 9)) && (nextTileY-> type != myTile-> type))
//                 {
//                     enemy[i].dir = 3;
//                 }
//                 break;
//             }
            
//             //up
//             case 3:
//             {
//                 Tile *myTile = (grid.getTile((enemy[i].x/g.tile_pxSize), (int)((enemy[i].y)/g.tile_pxSize)));
//                 Tile *nextTileX  = grid.getTile((int)(enemy[i].x/g.tile_pxSize)+1 , (int)(enemy[i].y/g.tile_pxSize));
//                 Tile *nextTileY = grid.getTile((int)(enemy[i].x/g.tile_pxSize) , (int)((enemy[i].y + 48)/g.tile_pxSize)-1);
           
//                 if (((nextTileY->type != myTile->type) && (myTile->type != 8 && myTile ->type != 9)) && (nextTileX->type == myTile->type))
//                 {   
//                     enemy[i].dir = 0;  
//                 }
//                 else if (((nextTileY->type != myTile->type) && (myTile->type !=8 && myTile->type!= 9)) && (nextTileX-> type != myTile->type))
//                 {
//                     enemy[i].dir = 2;
//                 }
//                  break;
//             } 


//         }
//     }
// }


void Game::pathContinues(TileGrid grid) { 
    for (int i = 0; i<(int)wave.size(); i++){
        switch(wave[i].dir)
        {
            //right
            case 0:
            {
                Tile *myTile = (grid.getTile((wave[i].x/g.tile_pxSize), (int)(wave[i].y/g.tile_pxSize)));
                Tile *nextTileX = grid.getTile((int)((wave[i].x- 16)/g.tile_pxSize) +1, (int)(wave[i].y/g.tile_pxSize));
                Tile *nextTileY = grid.getTile((int)(wave[i].x/g.tile_pxSize) , (int)(wave[i].y/g.tile_pxSize)+1);
                //cout << nextTileX ->type << endl;
                if (((nextTileX->type != myTile->type) && (nextTileX->type != 2)) && (nextTileY-> type != myTile-> type))
                {
                    wave[i].dir = 3;
                }
      
                else if (((nextTileX->type != myTile->type)) && (nextTileY-> type == myTile-> type))
                {
                    wave[i].dir = 1;
                }
                break;
            }
            //down        
            case 1:
            {
                Tile *myTile = (grid.getTile((wave[i].x/g.tile_pxSize), (int)(wave[i].y/g.tile_pxSize)));
                Tile *nextTileX = grid.getTile((int)(wave[i].x/g.tile_pxSize) +1, (int)(wave[i].y/g.tile_pxSize));
                Tile *nextTileY = grid.getTile((int)(wave[i].x/g.tile_pxSize) , (int)((wave[i].y -16)/g.tile_pxSize)+1);
  
                if (((nextTileY->type != myTile->type) && myTile->type !=8) && (nextTileX->type == myTile->type)){
                    wave[i].dir = 0;
               }
               else if (((nextTileY->type != myTile->type) && myTile->type !=8) && (nextTileX-> type != myTile->type))
               {
               wave[i].dir = 2;
               }
               break;
            }
            //left                
            case 2:
            {                    
                Tile *myTile = (grid.getTile((wave[i].x/g.tile_pxSize), (int)(wave[i].y/g.tile_pxSize)));
                Tile *nextTileX = grid.getTile((int)((wave[i].x+48)/g.tile_pxSize)-1 , (int)(wave[i].y/g.tile_pxSize));
                Tile *nextTileY = grid.getTile((int)(wave[i].x/g.tile_pxSize) , (int)(wave[i].y/g.tile_pxSize)+1); 
                if (((nextTileX->type != myTile->type) && (nextTileX->type != 2)) && (nextTileY-> type == myTile-> type))
                {
                    wave[i].dir = 1;
                }
      
                else if (((nextTileX->type != myTile->type) && (myTile->type !=8 && myTile->type != 9)) && (nextTileY-> type != myTile-> type))
                {
                    wave[i].dir = 3;
                }
                break;
            }
            
            //up
            case 3:
            {
                Tile *myTile = (grid.getTile((wave[i].x/g.tile_pxSize), (int)((wave[i].y)/g.tile_pxSize)));
                Tile *nextTileX  = grid.getTile((int)(wave[i].x/g.tile_pxSize)+1 , (int)(wave[i].y/g.tile_pxSize));
                Tile *nextTileY = grid.getTile((int)(wave[i].x/g.tile_pxSize) , (int)((wave[i].y + 48)/g.tile_pxSize)-1);
           
                if (((nextTileY->type != myTile->type) && (myTile->type != 8 && myTile ->type != 9)) && (nextTileX->type == myTile->type))
                {   
                    wave[i].dir = 0;  
                }
                else if (((nextTileY->type != myTile->type) && (myTile->type !=8 && myTile->type!= 9)) && (nextTileX-> type != myTile->type))
                {
                    wave[i].dir = 2;
                }
                 break;
            } 


        }
    }
}

void Game::killEnemy(Enemy *enemy)
{   
    for (int i = 0; i < (int)game.wave.size(); i++) {
        if (enemy == &wave[i]) {
            int end = wave.size()-1;
            swap(wave[i],wave[end]);
            wave[end].alive = 0;
            wave.pop_back();
        }
    }

    // enemy->width = 0;
    // enemy->height = 0;
    // enemy->alive = 0;
    // enemy->speed = 0;
    // enemy->x = -100;
    // enemy->y = -100;
    // enemy->distToEnd = 0;
    // player.funds++;
    // enemiesalive--;
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
            if (player.towers[i].currEnemy->health < 0 && ( player.towers[i].currEnemy-> alive != 0)) {
                //printf("EK\t--  range: %f,  dist: %f\n", player.towers[i].range, dist);
                Enemy *e = player.towers[i].currEnemy;
                //set all towers currEnemy that match the dead enemy to nullptr
                for (long unsigned int i = 0; i < player.towers.size(); i++) {
                    if (player.towers[i].currEnemy == e)
                        player.towers[i].currEnemy = nullptr;
                }
                game.killEnemy(e);
                
            }
        }
    }
}

#endif //_GAME_H_
