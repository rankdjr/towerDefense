#ifndef _GAME_H_
#define _GAME_H_

#include "enemy.h"
#include "TileGrid.h"

class Game {
public:
    int (*map)[10];
    int numEnemies;
    Enemy enemy[30];
    void killEnemy(Enemy *enemy);
    void pathContinues(TileGrid grid);
    Game() {
        numEnemies = 4;
    }   
    
    void initEnemies(int numEnemies) {  
        for( int i = 0; i<numEnemies; i++){
            enemy[i].x = grid.startTile.x;
            enemy[i].y = grid.startTile.y;
            enemy[i].width = 48;
            enemy[i].height = 48;
            enemy[i].speed = 0.5+(0.10*i);
            enemy[i].dir = 0;
            enemy[i].alive =1;
        }
    }
    
    
    } game;
        
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
            enemy->width = 0;
            enemy->height = 0;
            enemy->alive = 0;
        }

#endif //_GAME_H_
