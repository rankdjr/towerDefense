#ifndef _TILEGRID_H_
#define _TILEGRID_H_

#include "tile.h"

class TileGrid {
public:
	int width, height;
	Tile startTile, endTile;
	Tile map[10][10]; //**update global vars if changed**

	TileGrid();
	void setMap(int newMap[][10]);
	void draw();
	void drawTileOutline();
	void changeTile(TileType type, Tile *tile);
	Tile *getTile(int x, int y);

} grid;

TileGrid::TileGrid()
{
	//set map dimensions
	height = g.mapHeight;
	width = g.mapWidth;
	//
	//allocate 2d array for map tiles
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++)
			map[i][j] = Tile(&grass, i*64, j*64, 64, 64, grassType);
	}
}

void TileGrid::setMap(int newMap[][10]) 
{
	//Nested for loop iterates through 2d array and assigns tiles based on
	//integer values in array
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int row = (width - 1) - j; //start reading vals from "top" of 2d array
			switch (newMap[row][i])
			{
				case 0:
					//grass
					map[i][j] = Tile(&grass, i*64, j*64, 64, 64, grassType);
					break;
				case 1:
					//dirt
					map[i][j] = Tile(&dirt, i*64, j*64, 64, 64, dirtType);
					break;
				case 8:
					//start tile
					map[i][j] = Tile(&dirt, i*64, j*64, 64, 64, dirtType);
					startTile = map[i][j];
					break;
				case 9:
					//end tile
					map[i][j] = Tile(&dirt, i*64, j*64, 64, 64, endType);
					endTile = map[i][j];
					break;
				default:
					break;
			}
		}
	}
}

Tile* TileGrid::getTile(int i, int j)
{	
	Tile *tile = &map[i][j];
	return tile;
}

void TileGrid::draw()
{
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Tile *t = &map[i][j];
			drawQuadTex(*(t->texture), t->x, t->y, t->width, t->height);
			if (t->numOfTowers)
				t->tower.draw();
		}
	}
}

void TileGrid::drawTileOutline()
{
	int i = g.xMousePos/64;
	int j = 9-g.yMousePos/64;
	Tile tile = *grid.getTile(i, j);
	int weight = 10; //width of outline is 10 px wide
	int offset = tile.width - weight;
	if (g.buildState == BUY)
		glColor4f(1.0, 1.0, 1.0, 0.50);
	else if (g.buildState == SELL)
		glColor4f(0, 0, 0, 0.50);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	drawQuad(tile.x,        tile.y,        weight,         tile.height);//left side
	drawQuad(tile.x+offset, tile.y,        weight,         tile.height);//right side
	drawQuad(tile.x+weight, tile.y+offset, offset-weight,     weight);  //top
	drawQuad(tile.x+weight, tile.y,        offset-weight,     weight); 	//bottom
	glDisable(GL_BLEND);
}


#endif //_TILEGRID_H_
