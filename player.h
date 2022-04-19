#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <vector>

class Player {
public:
    int hp;
    int funds;
    char strHp[100];
	char strFunds[100];
    int towerHash[99];
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
    for (int i = 0; i < 99; i++)
        towerHash[i] = 0;
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
    int key = mapi*10 + mapj;
    bool towerExists = towerHash[key];
    if (!towerExists) {
        //add new tower and update hashtable
        towerHash[key] = towers.size(); // save curr index of vector to hash map
        towers.push_back(Tower(&towerBasic, towerTile.x, towerTile.y));
        funds -= g.towerCost;
    } else {
        
        // switch (towerIndex)
        // {
        // case 0:
        //     towerHash[key] = towers.size(); // save curr index of vector to hash map
        //     towers.push_back(Tower(&towerBasic, x, y));
        //     funds -= g.towerCost;
        //     break;
        
        // default:
        //     break;
        // }
    }
       
}

void Player::removeTower(float x, float y)
{
    //loop through towers and find matching x,y to tile paramater
    //once match is found, remove from vector and add funds to player

    //todo list code here

    int sellCost = 10;
    funds += sellCost;
}

void Player::updateHP(int dmg)
{
    hp -= dmg;
}

#endif //_PLAYER_H_
