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
    int numEnemies;
    int waveCtr, baseWaveSize;
    double waveTimer; //updated for new waves; total time to spawn wave and time between waves
    bool waitForWave, saveWaveSpawn;
    const double waveRate = 4.0;
    const double spawnRate = 0.65;
    struct timespec lastEnemySpawn, lastWaveSpawn, currentTime;
    vector<Enemy> wave;
    
    Game();
    void initWave();
    void checkWave();
    void killEnemy(Enemy *enemy);
    void pathContinues(TileGrid grid);
    void checkCurrEnemy();
    void updateTowerActions();
} game;                     


Game::Game() {
    //initilaize starting values for game start
    baseWaveSize = 5;
    numEnemies = 0;
    waveCtr = 0;
    waitForWave = 0;
    waitForWave = 1;
    waveTimer = 0;
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

void Game::checkWave() {
    static double timeFrLastWvSpawn = 0; //tracks time since last wave
    clock_gettime(CLOCK_REALTIME, &currentTime);
    timeFrLastWvSpawn = timeDiff(&lastWaveSpawn, &currentTime);
    if (g.debug) {
        printf("%f\t\t%i\n", timeFrLastWvSpawn, g.spawnWave);
    }
    if (timeFrLastWvSpawn > waveTimer) {
        g.spawnWave = 1;
        saveWaveSpawn = 1;
    }
}

void Game::initWave() {
    //
    int enemyCount = baseWaveSize + waveCtr; // total num of enemies in curr wave
    static int enemyItr = 0; //iterator to track how many enemies have been spawned
    static double timeFrLastEnSpawn = 0; //tracks time since last spawn

    //initiliaze timers
    clock_gettime(CLOCK_REALTIME, &currentTime);
    timeFrLastEnSpawn = timeDiff(&lastEnemySpawn, &currentTime); //initial diff will always be greater than frame rate

    //save new wave data; flag is reset once waveDiff is greater than waveTimer
    if (saveWaveSpawn) {
        waveTimer = (enemyCount * spawnRate) + waveRate; 
        timeCopy(&lastWaveSpawn, &currentTime);
        saveWaveSpawn = 0;
        printf("%f\n",waveTimer);
    }

    //spawn enemy up to current wave size (starting wave size + 1 enemy for each wave)
    if (timeFrLastEnSpawn > spawnRate) {
        //create new enemy and push to wave
        if (enemyItr < enemyCount) {
            //num of enemies is less than wave size --> continue adding enemies
            Enemy *e = new Enemy(grid.startTile.x, grid.startTile.y, 1.5, 0, (int)wave.size());
            wave.push_back(*e);
            delete e;
            enemyItr++;
            numEnemies++;
            //update time of last spawn
            timeCopy(&lastEnemySpawn, &currentTime);
        } else {
            //wave size has been reached --> inc wave ctr, reset enemy itr for next wave and set flags
            waveCtr++;
            g.spawnWave = 0;
            enemyItr = 0;
        }
    }
}


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
    int end = wave.size();
    for (int i = 0; i < end; i++) {
        if (enemy == &wave[i]) {
            wave.erase(wave.begin() + i);
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
            sort(wave.begin(), wave.end(), greater<Enemy>());
            for (int j = 0; j < (int)wave.size(); j++) {
                float dx = player.towers[i].cx - (wave[j].x + g.enemy_pxSize/2);
                float dy = player.towers[i].cy - (wave[j].y + g.enemy_pxSize/2);
                float dist = sqrt(dx*dx + dy*dy);
                //printf("{ %i, %f }, ", j, dist);
                if (dist < player.towers[i].range) {
                    player.towers[i].setCurrEnemy(&wave[j]);
                    //printf("NAE  --  tower[%li] --> wave[%i]\n", i, j);
                    j = (int)wave.size();
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
