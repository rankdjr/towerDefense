#ifndef _TILEGRID_H_
#define _TILEGRID_H_

#include "tile.h"

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
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				int row = (width - 1) - j; //start reading vals from top of 2d array
				if (newMap[row][i] == 0)
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

#endif //_TILEGRID_H_