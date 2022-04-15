#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <vector>
// #include "global.h"
// #include "tower.h"
// #include "tile.h"

class Player {
public:
    int hp;
    int funds;
    char strHp[100];
	char strFunds[100];
    std::vector<Tower> towers;

    Player();
    void addFunds(int amt);
    void addTower(Tile *tile);
    void removeTower(Tile *tile);
    void updateHP(int dmg);
} player;

Player::Player()
{
    hp = 10;
    funds = 100;
}

void Player::addFunds(int amt)
{
    funds += amt;
}

void Player::addTower(Tile *tile)
{
    if (funds > g.towerCost) {
        funds -= g.towerCost;
        tile->addTower();
        towers.push_back(tile->tower);
    } else {
        printf("Build tower failed: \tInsufficient funds\n");
    }
}

void Player::removeTower(Tile *tile)
{
    if (towers.empty())
        return;
    //
    //loop through towers and find matching x,y to tile paramater
    //once match is found, remove from vector and add funds to player
    for (long unsigned int i = 0; i < towers.size(); i++) {
        if (towers[i].x == tile->x && towers[i].y == tile->y)
            towers.erase(towers.begin()+i);
    }
    tile->tower = nullTower;
    tile->numOfTowers--;
    int sellCost = 10;
    funds += sellCost;
}

void Player::updateHP(int dmg)
{
    hp -= dmg;
}

#endif //_PLAYER_H_
