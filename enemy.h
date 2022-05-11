#ifndef _ENEMY_H_
#define _ENEMY_H_

//#include "global.h"
//#include "draw.h"

class Enemy {
public:
    float x, y, cx, cy, distToEnd; 
    int width, height;
    float health, speed;
    static const int maxHealth = 50;
    int dir; // 0= right, 1= down, 2= left, 3=up
    int frameNo;
    bool alive;
    Image *texture;
    struct timespec frameStart, currentTime;
    
    bool operator > (const Enemy& e) const
    {
        return (distToEnd > e.distToEnd);
    }

    Enemy() : x(-100), y(-100), width(0), height(0), health(0), speed(0), dir(0), alive(0), texture(&enemyBasic)
    {}
    Enemy(float x, float y, float speed, int dir){
        this->x = x;
        this->y = y;
        this->width = g.enemy_pxSize;
        this->height = g.enemy_pxSize;
        this->speed = speed;
        this->dir = dir;
        distToEnd = 0;
        health = maxHealth;
        alive = 1;
        frameNo = 1;
        texture = &enemyBasic;
    }

    void draw() {
        //health bar
        int xpos = x;
        int ypos = y+height+5;
        //static const int maxHealth = 100;
        //black quad as backdrop to use as a reference when health decreases
        glColor3ub(0,0,0);
        drawQuad(xpos, ypos, width, 5);
        //main health bar
        float hpWidth = width*((float)health/maxHealth);
        glColor3ub(255,0,0);
        drawQuad(xpos, ypos, hpWidth, 5);

        //sprite texture
        float tx1 = 0.0f + (float)((frameNo-1) % 7) * (1.0f/7.0f);
        float tx2 = tx1 + (1.0f/7.0f);
        float ty1 = 0.0f;
        float ty2 = 1.0f;
        //
        //timespec to control framerate
        static double diff = 0;
        static const double framerate = 0.15;
        clock_gettime(CLOCK_REALTIME, &currentTime);
        diff = timeDiff(&frameStart, &currentTime); //initial diff will always be greater than frame rate
        if (diff > framerate) {
            frameNo++;
            timeCopy(&frameStart, &currentTime);
        }
        if (frameNo >= 7)
            frameNo = 1;
        //
        //draw texture
        if (dir == 2)
            swap(tx1, tx2);
        drawQuadTexAlpha(*texture, x, y, tx1, tx2, ty1, ty2, width, height);
    } 
} nullEnemy;

#endif //_ENEMY_H_
