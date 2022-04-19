#ifndef _GLOBAL_H_
#define _GLOBAL_H_

enum GameState { START = 0, BUILD, PLAYING, END };
enum BuildState { NONE = 0, BUY, SELL};

class Global {
public:
	int xres, yres;
    int xMousePos, yMousePos;
    int towerWidth, towerHeight, towerCost;
    int tileWidth, tileHeight;
    int mapWidth, mapHeight;
    bool buildTower, showTowerRange;
    GameState gameState;
    BuildState buildState;

    Global() {
        //
        gameState = START;
        buildState = NONE;
        //
        //map vars **update map var in TileGrid.h if changed**
        mapWidth = 10;
        mapHeight = 10;
        //
        //tile vars
        tileWidth = 64;
        tileHeight = 64;
        //
        //tower vars
		towerWidth = 60;
        towerHeight = 60;
        towerCost = 10;
        buildTower = 0;
        showTowerRange = 0;
        //
        //initialize x,y resolution
        xres = mapWidth * tileWidth; //640px
        yres = mapHeight * tileHeight; //640px
    }
}g;

#endif //_GLOBAL_H_
