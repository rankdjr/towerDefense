#ifndef _TILE_H_
#define _TILE_H_

#include "image.h"
#include "draw.h"

class Tile {
public:
	float x, y, width, height;
	Image *texture;
	//bool build, path;
	Tile() {}
	Tile(Image *img, float x, float y, float width, float height) {
		texture = img;
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	void draw() {
		drawQuadTex(*texture, x, y, width, height);
	}
};

#endif //_TILE_H_