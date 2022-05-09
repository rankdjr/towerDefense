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
	int level;
	int id;
	Image *texture;
	Enemy *currEnemy;
	struct timespec frameStart, currentTime;

	Tower();
	Tower(Image *img, float x, float y);
	void setxy(int x, int y);
	void setwh(int w, int h);
	void draw();
	void drawProjectile();
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
	level = 0;
	id = -1;
}

Tower::Tower(Image *img, float x, float y) {
	texture = img;
	this->x = x;
	this->y = y;
	cx = x+g.tile_pxSize/2;
	cy = y+g.tile_pxSize/2;
	width = g.tower_pxSize;
	height = g.tower_pxSize;
	id = (x/g.tile_pxSize)*10 + (y/g.tile_pxSize);
	level = 1;
	active = 1;
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
	static const float offset = (g.tile_pxSize - g.tower_pxSize)/2.0f;
	float tx1 = 0.0f + (float)((frameNo-1) % 11) * (1.0f/11.0f);
	float tx2 = tx1 + (1.0f/11.0f);
	static const float ty1 = 0.0f;
	static const float ty2 = 1.0f;
	
	//timespec to control sprite frames
	static double diff = 0;
	static const double framerate = 0.075;
	clock_gettime(CLOCK_REALTIME, &currentTime);
	diff = timeDiff(&frameStart, &currentTime); //initial diff will always be greater than frame rate
	if (diff > framerate) {
		frameNo++;
		timeCopy(&frameStart, &currentTime);
	}
	if (frameNo >= 11)
		frameNo = 1;
	
	//draw tower
	drawQuadTexAlpha(*texture, x+offset, y+offset, tx1, tx2, ty1, ty2, width, height);
}

void Tower::drawProjectile()
{
	//draw attack
	if (currEnemy) {
		//get center of enemy texture
		int e_cx = currEnemy->x+12;
		int e_cy = currEnemy->y+15;
		//draw line
		glColor4ub(255,0,255,255);
		glPushMatrix();
		glBegin(GL_LINES);
			glVertex2f(cx, cy+18);
			glVertex2f(e_cx, e_cy);
		glEnd();
		glPopMatrix();
	}
}


void Tower::showRange()
{
	const int offset = g.tile_pxSize/2;
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
	currEnemy->health -= dmg;
}

#endif //_TOWER_H_
