#ifndef _TOWER_H_
#define _TOWER_H_

#include "image.h"
#include "draw.h"
//#include "tile.h"

class Tower {
public:
	float x, y, width, height;
	Image *texture;

	Tower(Image *img, float width, float height);
	void setxy(int x, int y);
	void setwh(int w, int h);
	void draw();

} tower1(&tower, g.towerWidth, g.towerWidth);

Tower::Tower(Image *img, float width, float height) {
	texture = img;
	this->width = width;
	this->height = height;
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
	int offset = 6;
	drawQuadTex(*texture, x+offset, y+offset, width, height);
}

#endif //_TOWER_H_