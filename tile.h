#ifndef _TILE_H_
#define _TILE_H_

#include "image.h"
#include "draw.h"

enum TileType {grassType, dirtType, endType};

class Tile {
public:
	float x, y, width, height;
	Image *texture;
	Tower tower;
	int numOfTowers;
	TileType type;
	bool build, path;
	int dir;
	
	Tile();
	Tile(Image *img, float x, float y, TileType type);
	Tile(Image *img, float x, float y, TileType type, int dir);
	void draw();
	void addTower();
	void upgradeTower();
	
} nullTile;

Tile::Tile()
{
	width = height = 0;
	numOfTowers = 0;
	build = path = 0;
}

Tile::Tile(Image *img, float x, float y, TileType type)
{
	texture = img;
	this->x = x;
	this->y = y;
	this->width = g.tile_pxSize;
	this->height = g.tile_pxSize;
	this->type = type;
	numOfTowers = 0;
	if (type == dirtType) {
		path = 1;
		build = 0;
	}
	if (type == grassType) {
		path = 0;
		build = 1;
	}
}

Tile::Tile(Image *img, float x, float y, TileType type, int dir)
{
	texture = img;
	this->x = x;
	this->y = y;
	this->width = g.tile_pxSize;
	this->height = g.tile_pxSize;
	this->type = type;
	this-> dir = dir;
	numOfTowers = 0;
	if (type == dirtType) {
		path = 1;
		build = 0;
	}
	if (type == grassType) {
		path = 0;
		build = 1;
	}
}

void Tile::draw()
{
	drawQuadTex(*texture, x, y, width, height);
}

void Tile::addTower()
{
	Tower *temp = new Tower(&towerBasic, x, y);
	tower = *temp;
	delete [] temp;
	numOfTowers++;
}

void Tile::upgradeTower()
{
	tower.range += 25;
	numOfTowers++;
}


#endif //_TILE_H_
