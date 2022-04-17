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
	Image *texture;
	Enemy *currEnemy;

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
	//x and y offset is used to center tower to tile
	//set an alpha channel
	//https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glAlphaFunc.xml
	glEnable(GL_ALPHA_TEST);
	//
	//transparent if alpha value is greater than 0.0
	glAlphaFunc(GL_GREATER, 0.0f);
	//
	//Set 4-channels of color intensity
	glColor4ub(255,255,255,255);
	int offset = 7;
	drawQuadTex(*texture, x+offset, y+offset, width, height);
	glDisable(GL_ALPHA_TEST);
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
	//change below if statement to while loop when implementing thread
	glColor4ub(255,255,255,255);
	glPushMatrix();
	glBegin(GL_LINES);
		glVertex2f(cx, cy);
		glVertex2f(currEnemy->x+24, currEnemy->y+24);
	glEnd();
	glPopMatrix();
	
	currEnemy->health -= dmg;
}
#endif //_TOWER_H_
