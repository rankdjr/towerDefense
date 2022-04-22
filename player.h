#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <vector>
#include <algorithm>

class Player {
public:
    int hp;
    int funds;
    char strHp[100];
	char strFunds[100];
    int towerHash[99][2];
    std::vector<Tower> towers;

    Player();
    void addFunds(int amt);
    void addTower(float x, float y);
    void removeTower(float x, float y);
    void updateHP(int dmg);
} player;

Player::Player()
{
    hp = 10;
    funds = 100;
    for (int i = 0; i < 99; i++) {
        towerHash[i][0] = 0;
        towerHash[i][1] = -1;
    }
}

void Player::addFunds(int amt)
{
    funds += amt;
}

void Player::addTower(float x, float y)
{
    int mapi = g.xMousePos/g.tile_pxSize;
    int mapj = 9-g.yMousePos/g.tile_pxSize;
    Tile towerTile = *(grid.getTile(mapi, mapj));
    if (!towerTile.build) {
        printf("Invalid tile\n");
        return;
    }
    
    if (funds < g.towerCost) {
        printf("Build tower failed: \tInsufficient funds\n");
        return;
    }

    const int towerLvl = 0;
    const int towerIndex = 1;
    int towerId = mapi*10 + mapj;
    int towerExists = towerHash[towerId][towerLvl];

    //debug
    //making sure that mapi and mapj are computing correctly by matching tile x,y to tower id and tower x,y
    // Tile t = *grid.getTile(mapi, mapj);
    // printf("tileX: %f,  tileY: %f\n", t.x, t.y);
    //end debug
    switch (towerExists)
    {
        case 0:
        {
            //not tower exists --> add new tower and update hashtable
            towerHash[towerId][towerIndex] = towers.size(); // save curr index of vector to hash map
            towers.push_back(Tower(&towerBasic, towerTile.x, towerTile.y)); //push new tower
            towerHash[towerId][towerLvl]++; //update tower level in hash
            funds -= g.towerCost;
            
            //debug
            // printf("towrX: %f,  towrY: %f\n", towers[towers.size()-1].x, towers[towers.size()-1].y);
            // printf("towId: %i, towLvl: %i\n", towers[towers.size()-1].id, towers[towers.size()-1].level);
            //end debug
            break;
        }
        case 1:
        {
            //tower level 1 --> upgrade tower
            int vecIndex = towerHash[towerId][towerIndex]; // get index of tower in player.towers vector
            towers[vecIndex].range *= 1.15;
            towers[vecIndex].dmg *= 1.25;
            towerHash[towerId][towerLvl]++; //update tower level in hash
            funds -= g.towerCost;

            //debug
            // printf("towrX: %f,  towrY: %f\n", towers[towers.size()-1].x, towers[towers.size()-1].y);
            // printf("towId: %i, towLvl: %i\n", towers[towers.size()-1].id, towers[towers.size()-1].level);
            //end debug
            break;
        }
        case 2:
        {
            //tower level 2 --> upgrade tower
            int vecIndex = towerHash[towerId][towerIndex]; // get index of tower in player.towers vector
            towers[vecIndex].range *= 1.15;
            towers[vecIndex].dmg *= 1.25;
            towerHash[towerId][towerLvl]++;
            funds -= g.towerCost;  //update tower level in hash

            //debug
            // printf("towrX: %f,  towrY: %f\n", towers[towers.size()-1].x, towers[towers.size()-1].y);
            // printf("towId: %i, towLvl: %i\n", towers[towers.size()-1].id, towers[towers.size()-1].level);
            //end debug
            break;
        }
        case 3:
        {
            //tower level 3 --> max level tower
            printf("Max tower level reached\n");
            break;
        }
        default:
        {
            printf("add tower err; player.h addTower()");
            break;
        }
    }

    //hash debug
    // printf("id: %i, lvl: %i\n", towerId, towerHash[towerId][towerLvl]);
    // printf("numtowers: %li\n\n", towers.size());
    //end hash debug
    return;
       
}

void Player::removeTower(float x, float y)
{
    //loop through towers and find matching x,y to tile paramater
    //once match is found, remove from vector and add funds to player
    const int towerLvl = 0;
    const int towerIndex = 1;
    int mapi = g.xMousePos/g.tile_pxSize;
    int mapj = 9-g.yMousePos/g.tile_pxSize;
    int towerId = mapi*10 + mapj;
    int towerExists = towerHash[towerId][towerLvl];
    //
    if (!towerExists) {
        //no tower exists on tile
        printf("Tower missing\n");
    } else {
        //remove tower from player
        int vecIndex = towerHash[towerId][towerIndex];  // get index of tower in player.towers vector
        //printf("id: %i, lvl: %i\n", towers[vecIndex].id, towers[vecIndex].level);
        
        swap(towers[vecIndex], towers.back());          // swap tower that is to be deleted, to the end of the vector
        towerHash[towers.back().id][towerLvl] = 0;      // reset hash table for tower to be deleted
        towerHash[towers.back().id][towerIndex] = -1;   // reset hash table for tower to be deleted
        //printf("id: %i, lvl: %i\n",towers.back().id, towers.back().level);
        
        towers.pop_back();                              // pop deleted tower 
        //printf("id: %i, lvl: %i\n", towers[vecIndex].id, towers[vecIndex].level);
        funds += (float)g.towerCost * 0.4;
        //
        //update vector values in tower hash table for active towers
        for (int i = 0; i < (int)towers.size(); i++) {
            int currTowerID = towers[i].id;
            int currTowerLvl = towers[i].level;
            towerHash[currTowerID][towerLvl] = currTowerLvl;
            towerHash[currTowerID][towerIndex] = i;
        }
    }

    return;
}

void Player::updateHP(int dmg)
{
    hp -= dmg;
}

#endif //_PLAYER_H_
