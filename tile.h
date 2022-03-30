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

class TileGrid {
public:
	int width, height;
	Tile map[10][10];
	TileGrid() {
		//set map dimensions
		height = 10;
		width = 10;
		//
		//allocate 2d array for map tiles
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++)
				map[i][j] = Tile(&grass, i*64, j*64, 64, 64);
		}
	}
	TileGrid(int newMap[][10]) {
		//set map dimensions
		height = 10;
		width = 10;
		//
		//allocate 2d array for map tiles
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (newMap[j][i] == 0)
					map[i][j] = Tile(&grass, i*64, j*64, 64, 64);
				else
					map[i][j] = Tile(&dirt, i*64, j*64, 64, 64);
			}
		}
	}
	void draw() {
		int width = 10;
		int height = 10;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				Tile t = map[i][j];
				drawQuadTex(*t.texture, t.x, t.y, t.width, t.height);
			}
		}
	}
    void setTile(Tile t, int x, int y, int height, int width) {
            drawQuadTex(*t.texture, x*64, y*64, t.width, t.height);
    }
};

#endif //_TILE_H_