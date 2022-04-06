#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "global.h"
#include "draw.h"

class Enemy {
public:
    int x, y, width, height, health;
    float speed;
    int dir;
    bool alive;
    Enemy() : x(0), y(0), width(0), height(0), speed(0), dir(0), health(0), alive(0)
    {}
    Enemy(int x, int y, int width, int height, float speed, int dir){
        this->x = x*64;
        this->y = y*64;
        this->width = width;
        this->height = height;
        this->speed = speed;
        // 0= right, 1= down, 2= left, 3=up
        this->dir = dir;
    }

    void Draw() {
    
       drawQuad(x,y,width,height);
    } 
};

#endif //_ENEMY_H_
