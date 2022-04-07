#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "global.h"
#include "draw.h"

class Enemy {
public:
    float x, y; 
    int width, height, health;
    float speed;
    int dir;
    bool alive;
    Image *texture;
    Enemy() : x(0), y(0), width(0), height(0), health(0), speed(0), dir(0), alive(0)
    {}
    Enemy(float x, float y, float speed, int dir){
        this->x = x*64;
        this->y = y*64;
        this->width = 48;
        this->height = 48;
        this->speed = speed;
        // 0= right, 1= down, 2= left, 3=up
        this->dir = dir;
        health = 100;
        alive = 1;
        texture = &enemyBasic;
    }

    void Draw() {
        drawQuad(x, y,width,height);
    } 
};

#endif //_ENEMY_H_
