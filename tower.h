#ifndef _TOWER_H_
#define _TOWER_H_

#include "image.h"
#include "draw.h"


class Tower {
public:
	float x, y, width, height, range;
	bool active;
	Image *texture;

	Tower();
	Tower(Image *img, float x, float y, int width, int height, bool active);
	void setxy(int x, int y);
	void setwh(int w, int h);
	void draw();
	void showRange();

} nullTower;

Tower::Tower()
{
	texture = &towerBasic;
	x = y = -10; //if drawn without setting x and y, this will be a red flag
	width = height = 50;
	range = 200;
	active = 0;
}

Tower::Tower(Image *img, float x, float y, int width, int height, bool active) {
	texture = img;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->active = active;
	range = 200;
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
	int offset = 7;
	drawQuadTex(*texture, x+offset, y+offset, width, height);
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

#endif //_TOWER_H_