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
    unsigned int texid;

	Image() { }
	Image(const char *fname) {
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
  dirt("res/dirt.png");


#endif //_IMAGE_H_