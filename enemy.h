#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "global.h"
#include "draw.h"

class Enemy {
public:
    int x, y, width, height, health;
    float speed;
    double delay;
    int dir;

    Enemy(int x, int y, int width, int height, float speed, int dir){
        this->x = x*64;
        this->y = y*64;
        this->width = width;
        this->height = height;
        this->speed = speed;
        this->delay =.15;
        // 0= right, 1= down, 2= left, 3=up
        this->dir = dir;
    }

    void Draw() {
    
       drawQuad(x,y,width,height);
    } 
};

#endif //_ENEMY_H_
