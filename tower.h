#ifndef _TOWER_H_
#define _TOWER_H_

#include "image.h"
#include "draw.h"
#include "enemy.h"

class Tower {
public:
	float x, y, cx, cy, width, height, range;
	float dmg;
	bool active;
	int frameNo;
	Image *texture;
	Enemy *currEnemy;
	struct timespec frameStart, currentTime;

	Tower();
	Tower(Image *img, float x, float y, int width, int height, bool active);
	void setxy(int x, int y);
	void setwh(int w, int h);
	void draw();
	void showRange();
	void setCurrEnemy(Enemy *enemy);
	void attackEnemy();
} nullTower;

Tower::Tower()
{
	texture = &towerBasic;
	x = y = -10;
	width = height = 0;
	range = 0;
	active = 0;
}

Tower::Tower(Image *img, float x, float y, int width, int height, bool active) {
	texture = img;
	this->x = x;
	this->y = y;
	cx = x+g.tileWidth/2;
	cy = y+g.tileHeight/2;
	this->width = width;
	this->height = height;
	this->active = active;
	range = 175;
	dmg = 0.35;
	currEnemy = nullptr;
	frameNo = 1;
	clock_gettime(CLOCK_REALTIME, &frameStart);
}

void Tower::setxy(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Tower::setwh(int w, int h)
{
	this->width = h;
	this->height = w;
}

void Tower::draw()
{
	//offset is used to center tower to tile
	//tx and ty vars are used to map the frames of the sprite sheet
	int offset = 7;
	float tx1 = 0.0f + (float)((frameNo-1) % 11) * (1.0f/11.0f);
	float tx2 = tx1 + (1.0f/11.0f);
	float ty1 = 0.0f;
	float ty2 = 1.0f;
	
	//timespec to control framerate
	static double diff = 0;
	static const double framerate = 0.075;
	clock_gettime(CLOCK_REALTIME, &currentTime);
	diff = timeDiff(&frameStart, &currentTime);
	if (diff > framerate) {
		frameNo++;
		timeCopy(&frameStart, &currentTime);
	}
	if (frameNo >= 11)
		frameNo = 0;

	drawQuadTexAlpha(*texture, x+offset, y+offset, tx1, tx2, ty1, ty2, width, height);
}

void Tower::showRange()
{
	int offset = g.tileWidth/2;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.68, 0.85, 0.90, 0.60);
	drawCircle(x+offset, y+offset, range);
	glDisable(GL_BLEND);
}

void Tower::setCurrEnemy(Enemy *enemy)
{
	currEnemy = enemy;
}

void Tower::attackEnemy()
{
	glColor4ub(255,0,255,255);
	glPushMatrix();
	glBegin(GL_LINES);
		glVertex2f(cx, cy+18);
		glVertex2f(currEnemy->x+24, currEnemy->y+24);
	glEnd();
	glPopMatrix();
	
	currEnemy->health -= dmg;
}
#endif //_TOWER_H_
