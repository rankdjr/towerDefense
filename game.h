#ifndef _GAME_H_
#define _GAME_H_

#include "enemy.h"
#include "TileGrid.h"
double timeDiff(struct timespec *start, struct timespec *end);
void timeCopy(struct timespec *dest, struct timespec *source);

class Game {
public:
    int (*currMap)[10];
    int numEnemies;
    //std::vector<Enemy> enemy;
    Enemy enemy[30];
    
    //struct timespec timeCurrent, towerAtkStart;
    //double towerAtkTimeSpan, towerAtkCountdown;
    Game();
    void initEnemies(int);
    void killEnemy(Enemy *enemy);
    void pathContinues(TileGrid grid);
    void sortEnemiesByDistance();
    void updateTowerActions();
} game;

Game::Game() {
    //map vars
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
    currMap = map;
    grid.setMap(currMap);
    //
    //tower vars
    // towerAtkTimeSpan = 0;
    // towerAtkCountdown = 0;
    //
    //enemy vars
    numEnemies = 10;
}   

void Game::initEnemies(int numEnemies) {  
    for( int i = 0; i<numEnemies; i++){
        enemy[i].x = grid.startTile.x;
        enemy[i].y = grid.startTile.y;
        enemy[i].width = 48;
        enemy[i].height = 48;
        enemy[i].speed = 0.5+(0.10*i);
        enemy[i].health = 100;
        enemy[i].dir = 0;
        enemy[i].alive =1;
    }

    // for( int i = 0; i<numEnemies; i++){
    //     float speed = 0.5+(0.10*i);
    //     Enemy newEnemy = Enemy(grid.startTile.x, grid.startTile.y, speed, 0);
    //     enemy.push_back(newEnemy);
    // }
}

void Game::pathContinues(TileGrid grid){
    // printf("here!");
    
    for (int i = 0; i<numEnemies; i++){
        switch(enemy[i].dir){
            case 0:
            {
            Tile *myTile = (grid.getTile((enemy[i].x/64), (int)(enemy[i].y/64)));
            Tile *nextTileX = grid.getTile((int)(enemy[i].x/64) +1, (int)(enemy[i].y/64));
            Tile *nextTileY = grid.getTile((int)(enemy[i].x/64) , (int)(enemy[i].y/64)+1);
            // cout << myTile-> type;
        if ((nextTileX->type != myTile->type) && (nextTileY-> type != myTile-> type)){ 
            enemy[i].dir = 3;
            // cout << "if" << endl;
        }
            else if ((nextTileX->type != myTile->type) && (nextTileY-> type == myTile-> type)){
            enemy[i].dir = 1;
            // cout << "else" << endl;    
        }
        
        break;
            }        
            case 1:
            {
            Tile *myTile = (grid.getTile((enemy[i].x/64), (int)(enemy[i].y/64)));
            Tile *nextTileX = grid.getTile((int)(enemy[i].x/64) +1, (int)(enemy[i].y/64));
            Tile *nextTileY = grid.getTile((int)(enemy[i].x/64) , (int)(enemy[i].y/64)+1);
            // cout << myTile-> type;
            if (((nextTileY->type != myTile->type) && myTile->type !=8) && (nextTileX->type == myTile->type)){
                
                enemy[i].dir = 0;
                cout << "if" << endl;
            }
            else if (((nextTileY->type != myTile->type) && myTile->type !=8) && (nextTileX-> type != myTile->type))
                    {
                    enemy[i].dir = 2;
                    cout << "else" << endl;
                    }
        break;
            }                
        case 2:
            
            
            {                    
            Tile *myTile = (grid.getTile((enemy[i].x/64), (int)(enemy[i].y/64)));
            Tile *nextTileX = grid.getTile((int)(enemy[i].x/64)-1 , (int)(enemy[i].y/64));
            // cout << myTile-> type;
            
            if ((nextTileX->type != myTile->type) && myTile->type !=8){
                enemy[i].dir++;
                cout << "left" << endl;
            }
            
        break;
            }

        case 3:
            {
            Tile *myTile = (grid.getTile((enemy[i].x/64), (int)(enemy[i].y/64)));
            Tile *nextTileY = grid.getTile((int)(enemy[i].x/64) , (int)(enemy[i].y/64)-1);
            cout << myTile-> type << endl;
            cout << nextTileY->type << endl;
            if ((nextTileY->type != myTile->type) && myTile->type !=8){
                enemy[i].dir = 0;
                cout << "down" << endl;
            }
        break;
            }


        }
    }
}

void Game::killEnemy(Enemy *enemy)
{   
    // printf("ne,  %p\n", &nullEnemy);
    // printf("ee,  %p\n", enemy);
    // enemy = &nullEnemy;
    // printf("eef, %p\n", enemy);

    enemy->width = 0;
    enemy->height = 0;
    enemy->alive = 0;
    enemy->speed = 0;
    enemy->x = -100;
    enemy->y = -100;
}

void Game::sortEnemiesByDistance()
{
    //get enemies distance to end tile
    for (int i = 0; i < numEnemies; i++) {
        float dx = (grid.endTile.x + g.tileWidth)- enemy[i].x;
        float dy = grid.endTile.y - enemy[i].y;
        enemy[i].distToEnd = dx*dx + dy*dy;
    }
    //
    //bubble sort enemies in descending order by distance to end tile
    if (g.gameState == PLAYING) {
        for (int i = 0; i < numEnemies-1; i++) {
            for (int j = i+1; j < numEnemies; j++) {
                if (enemy[i].distToEnd < enemy[j].distToEnd) {
                    Enemy temp = enemy[j];
                    enemy[j] = enemy[i];
                    enemy[i] = temp;
                    // printf("swapped in t[%i]\n", i);
                }
            }
        }  
    }
}

void Game::updateTowerActions()
{
    //Loop though towers and set current enemy for any towers that have no target
    for (long unsigned int i = 0; i < player.towers.size(); i++) {
        //set static range value for current tower
        int range = player.towers[i].range*player.towers[i].range;
        if (player.towers[i].currEnemy != nullptr) {
            //if currEnemy has already be targeted, attack target until target is out of range or no longer alive
            float dx = player.towers[i].cx - ((player.towers[i].currEnemy->x)+24);
            float dy = player.towers[i].cy - ((player.towers[i].currEnemy->y)+24);
            float dist = dx*dx + dy*dy;
            if (dist < range) {
                //printf("range: %i,  dist: %f\n", range, dist);
                //printf("%p\n",player.towers[i].currEnemy);
                player.towers[i].attackEnemy();     
                if (player.towers[i].currEnemy->health <= 0) {
                    game.killEnemy(player.towers[i].currEnemy);
                    player.towers[i].currEnemy = nullptr;
                }

                //temporary code to test "zap" style attack
                // clock_gettime(CLOCK_REALTIME, &timeCurrent);
                // towerAtkTimeSpan = timeDiff(&towerAtkStart, &timeCurrent);
                // timeCopy(&towerAtkStart, &timeCurrent);
                // towerAtkCountdown += towerAtkTimeSpan;
                // printf("%f\n", towerAtkCountdown);
                // if(towerAtkCountdown >= 1.5) {
                //     player.towers[i].attackEnemy();
                //     towerAtkCountdown = 0;
                // }
            }
            else {
                //enemy is out of range
                player.towers[i].currEnemy = nullptr;
            }
        } 
        else {
            //if current tower has not target, then find and select target
            //loop through sorted enemy array and look for first enemy in range
            sortEnemiesByDistance();
            for (int j = numEnemies; j >= 0; --j) {
                float dx = player.towers[i].cx - (enemy[j].x+24);
                float dy = player.towers[i].cy - (enemy[j].y+24);
                float dist = dx*dx + dy*dy;
                //printf("{ %i, %f }, ", j, dist);
                if (dist < range) {
                    player.towers[i].setCurrEnemy(&enemy[j]);
                    //printf("tower[%li] --> enemy[%i]\n", i, j);
                    j = 0;
                }
            }
        }
    }
}

#endif //_GAME_H_
