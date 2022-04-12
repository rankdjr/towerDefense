#ifndef _TOWER_H_
#define _TOWER_H_

#include "image.h"
#include "draw.h"
#include "enemy.h"

class Tower {
public:
	float x, y, width, height, range;
	bool active;
	Image *texture;

	Tower();
	Tower(Image *img, float x, float y, int width, int height, bool active);
	void setxy(int x, int y);
	void setwh(int w, int h);
	void draw();
	void showRange();
	void targetEnemy(Enemy enemy);
} nullTower;

Tower::Tower()
{
	texture = &towerBasic;
	x = y = -10; //if drawn without setting x and y, this will be a red flag
	width = height = 50;
	range = 200;
	active = 0;
}

Tower::Tower(Image *img, float x, float y, int width, int height, bool active) {
	texture = img;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->active = active;
	range = 200;
}

void Tower::setxy(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Tower::setwh(int w, int h)
{
	this->width = h;
	this->height = w;
}

void Tower::draw()
{
	//x and y offset is used to center tower to tile
	int offset = 7;
	drawQuadTex(*texture, x+offset, y+offset, width, height);
}

void Tower::showRange()
{
	int offset = g.tileWidth/2;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.68, 0.85, 0.90, 0.60);
	drawCircle(x+offset, y+offset, range);
	glDisable(GL_BLEND);
}

void Tower::targetEnemy(Enemy enemy)
{
	// for (int i = 0; i < numEnemies; i++) {
	// 	glPushMatrix();
	// 	glBegin(GL_LINES);
	// 		glVertex2f(x, y);
	// 		glVertex2f(enemy[i].x, enemy[i].x);
	// 	glEnd();
	// 	glPopMatrix();
	// }

		glPushMatrix();
		glBegin(GL_LINES);
			glVertex2f(x, y);
			glVertex2f(enemy.x, enemy.y);
		glEnd();
		glPopMatrix();

		printf("x: %f,  y:%f\n", enemy.x, enemy.y);
	//1. sort enemies from distance to endpoint
	//2. search array for furthest enemy within range and set to activeEnemy
	//3. while enemy is in range --> fire projectile
	//4. if enemy dies or exits range of tower, go back to 1
	//this should loop through the entirety of the game; call before render but after physics

}
#endif //_TOWER_H_