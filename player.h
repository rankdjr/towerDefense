#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <vector>

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
    printf("id: %i, lvl: %i\n", towerId, towerExists);
    switch (towerExists)
    {
        case 0:
        {
            //not tower exists --> add new tower and update hashtable
            towerHash[towerId][towerIndex] = towers.size(); // save curr index of vector to hash map
            towers.push_back(Tower(&towerBasic, towerTile.x, towerTile.y)); //push new tower
            towerHash[towerId][towerLvl]++; //update tower level in hash
            funds -= g.towerCost;
            break;
        }
        case 1:
        {
            //tower level 1 --> upgrade tower
            int vecIndex = towerHash[towerId][towerIndex]; // get index of tower in player.towers vector
            towers[vecIndex].upgradeTower();
            towerHash[towerId][towerLvl]++; //update tower level in hash
            funds -= g.towerCost;
            break;
        }
        case 2:
        {
            //tower level 2 --> upgrade tower
            int vecIndex = towerHash[towerId][towerIndex]; // get index of tower in player.towers vector
            towers[vecIndex].upgradeTower();
            towerHash[towerId][towerLvl]++;
            funds -= g.towerCost;  //update tower level in hash
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
    if (towerExists) {
        //remove tower from player
        int vecIndex = towerHash[towerId][towerIndex];  // get index of tower in player.towers vector
        towers.erase(towers.begin() + vecIndex);
        funds += (float)g.towerCost * 0.4;
        //
        //update hashTable for current tower
        towerHash[towerId][towerLvl] = 0;
        towerHash[towerId][towerIndex] = -1;
        //
        //update vector index values in tower hash table for active towers
        for (int i = 0; i < (int)towers.size(); i++) {
            int currTowerID = towers[i].id;
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
