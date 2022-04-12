#ifndef _GLOBAL_H_
#define _GLOBAL_H_

enum GameState { START = 0, BUILD, PLAYING, END };

class Global {
public:
	int xres, yres;
    int xMousePos, yMousePos;
    int towerWidth, towerHeight;
    int tileWidth, tileHeight;
    int mapWidth, mapHeight;
    bool buildTower, showTowerRange;
    GameState gameState;
    bool buildState;

    Global() {
        gameState = START;
        buildState = 0;
        //map vars **update map var in TileGrid.h if changed**
        mapWidth = 10;
        mapHeight = 10;
        //
        //tile vars
        tileWidth = 64;
        tileHeight = 64;
        //
        //tower vars
		towerWidth = 50;
        towerHeight = 50;
        buildTower = 0;
        showTowerRange = 0;
        //
        //initialize x,y resolution
        xres = mapWidth * tileWidth; //640px
        yres = mapHeight * tileHeight; //640px
    }
}g;

#endif //_GLOBAL_H_
