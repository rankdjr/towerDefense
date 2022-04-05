#ifndef _TILE_H_
#define _TILE_H_

#include "image.h"
#include "draw.h"

enum TileType {grassType, dirtType};

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
	Tile(Image *img, float x, float y, float width, float height, TileType type);
	Tile(Image *img, float x, float y, float width, float height, TileType type, int dir);
	void draw();
	void addTower();
	
} nullTile;

Tile::Tile()
{
	width = height = 0;
	numOfTowers = 0;
	build = path = 0;
}

Tile::Tile(Image *img, float x, float y, float width, float height, TileType type)
{
	texture = img;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
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

Tile::Tile(Image *img, float x, float y, float width, float height, TileType type, int dir)
{
	texture = img;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
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
	if(!build) {
		printf("invalid tile\n");
		return;
	}

	if(numOfTowers == 3) {
		//tower limit reached
		printf("max tower limit reached\n");
		return;
	} else if(numOfTowers < 1) {
		//add new tower
		Tower *temp = new Tower(&towerBasic, x, y, g.towerWidth, g.towerHeight, 1);
		tower = *temp;
		delete [] temp;
		//tower = new Tower(&towerBasic, x, y, g.towerWidth, g.towerHeight);
		numOfTowers++;
	} else {
		//upgrade tower
		numOfTowers++;
	}
}


#endif //_TILE_H_