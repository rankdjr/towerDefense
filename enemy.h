#ifndef _ENEMY_H_
#define _ENEMY_H_

//#include "global.h"
//#include "draw.h"

class Enemy {
public:
    float x, y, distToEnd; 
    int width, height;
    float health, speed;
    int dir;
    bool alive;
    Image *texture;
    Enemy() : x(-100), y(-100), width(0), height(0), health(0), speed(0), dir(0), alive(0), texture(&enemyBasic)
    {}
    Enemy(float x, float y, float speed, int dir){
        this->x = x*64;
        this->y = y*64;
        this->width = 48;
        this->height = 48;
        this->speed = speed;
        // 0= right, 1= down, 2= left, 3=up
        this->dir = dir;
        distToEnd = 0;
        health = 100;
        alive = 1;
        texture = &enemyBasic;
    }

    void Draw() {
        //draw sprite texture
        drawQuadTex(*texture,x,y,width,height);
        
        //draw health bar
        int xpos = x;
        int ypos = y+height+5;
        static int maxHealth = 100;
        //black quad as backdrop to use as reference when health decreases
        glColor3ub(0,0,0);
        drawQuad(xpos, ypos, width, 5);
        //main health bar
        float hpWidth = width*((float)health/maxHealth);
        glColor3ub(255,0,0);
        drawQuad(xpos, ypos, hpWidth, 5);
    } 
} nullEnemy;

#endif //_ENEMY_H_
