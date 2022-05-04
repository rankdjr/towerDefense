#ifndef _GLOBAL_H_
#define _GLOBAL_H_

enum GameState { START = 0, BUILD, PLAYING, END };
enum BuildState { NONE = 0, BUY, SELL};

class Global {
public:
	int xres, yres;
    int xMousePos, yMousePos;
    int tower_pxSize, towerCost;
    int tile_pxSize;
    int enemy_pxSize;
    int mapWidth, mapHeight;
    bool buildTower, showTowerRange;
    int wave;
    char strWave[30];
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
        tile_pxSize = 64;
        //
        //tower vars
		tower_pxSize = 60;
        towerCost = 10;
        buildTower = 0;
        showTowerRange = 0;
        //
        //enemy vars
        enemy_pxSize = 45;
        //
        //initialize x,y resolution
        xres = mapWidth * tile_pxSize; //640px
        yres = mapHeight * tile_pxSize; //640px

        // wave counter;
        wave = 0;
    }
}g;

#endif //_GLOBAL_H_
