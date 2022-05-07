#ifndef _GLOBAL_H_
#define _GLOBAL_H_

enum GameState { START = 0, BUILD, PLAYING, PAUSE, END };
enum BuildState { NONE = 0, BUY, SELL};

class button {
public:
	float pos[2];
	float width, height;
	bool clicked;
    button();
	button(float w, float p0, float p1) {
		width = w;
        height = 40;
        pos[0] = p0;
        pos[1] = p1;
		clicked = 0;
	}
} startButton(105, 270, 220),
  ctrlButton(90, 280, 475);

class Global {
public:
	int xres, yres;
    int xMousePos, yMousePos;
    int tower_pxSize, towerCost;
    int tile_pxSize;
    int enemy_pxSize;
    int mapWidth, mapHeight;
    bool buildTower, showTowerRange, showControls;

    int wave;
    char strWave[30];
    GameState gameState;
    BuildState buildState;

    Global() {
        //
        gameState = START;
        buildState = NONE;
        showControls = 0;
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
        //
        // wave counter;
        wave = 0;
    }
}g;

#endif //_GLOBAL_H_
