#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <fstream>
#include <cstring>
#include <unistd.h>
using namespace std;

class Image {
public:	
	int width, height, max;
	char *data;
	const char *fname;
    unsigned int texid;

	Image() { }
	Image(const char *fname) {
		this->fname = fname;
		bool isPPM = true;
		char str[1200];
		char newfile[200];
		ifstream fin;
		char *p = strstr((char *)fname, ".ppm");
		if(!p) {
			//not a ppm
			isPPM = false;
			strcpy(newfile, fname);
			newfile[strlen(newfile)-4] = '\0';
			strcat(newfile, ".ppm");
			sprintf(str, "convert %s %s", fname, newfile);
			system(str);
			fin.open(newfile);
		} else {
			fin.open(fname);
		}

		char p6[10];
		fin >> p6;
		fin >> width >> height;
		fin >> max;
		data = new char [width*height*3];
		fin.read(data, 1);
		fin.read(data, width*height*3);
		fin.close();
		if(!isPPM)
			unlink(newfile);
	}
} grass("res/grass.png"),
  dirt("res/dirt.png"),
  towerBasic("res/towerSprite.png"),
  enemyBasic("res/enemy1Sprite.png");

unsigned char *buildAlphaData(Image *img, int r1, int g1, int b1)
{
    //Add 4th component to an RGB stream...
    //RGBA
    //When you do this, OpenGL is able to use the A component to determine
    //transparency information.
    //It is used in this application to erase parts of a texture-map from view.
    int i;
    int a,b,c;
    unsigned char *newdata, *ptr;
    unsigned char *data = (unsigned char *)img->data;
    newdata = (unsigned char *)malloc(img->width * img->height * 4);
    ptr = newdata;
    for (i=0; i<img->width * img->height * 3; i+=3) {
        a = *(data+0);
        b = *(data+1);
        c = *(data+2);
        *(ptr+0) = a;
        *(ptr+1) = b;
        *(ptr+2) = c;
        //-----------------------------------------------
        //get largest color component...
        //*(ptr+3) = (unsigned char)((
        //        (int)*(ptr+0) +
        //        (int)*(ptr+1) +
        //        (int)*(ptr+2)) / 3);
        //d = a;
        //if (b >= a && b >= c) d = b;
        //if (c >= a && c >= b) d = c;
        //*(ptr+3) = d;
        //-----------------------------------------------
        //this code optimizes the commented code above.
        //code contributed by student: Chris Smith
        //
        *(ptr+3) = (a!=r1 && b!=g1 && c!=b1);
        //-----------------------------------------------
        ptr += 4;
        data += 3;
    }
    return newdata;
}

#endif //_IMAGE_H_