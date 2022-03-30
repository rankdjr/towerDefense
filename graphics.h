#include <fstream>
#include <unistd.h>
#include <cstring>
#include <GL/glx.h>
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
};

void init_graphics()
{
    g.temp = new Image("images/grass.png");
}

unsigned char *buildAlphaData(Image *img)
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
        *(ptr+3) = (a!=0 && b!=0 && c!=0);
        //-----------------------------------------------
        ptr += 4;
        data += 3;
    }
    return newdata;
}

void drawQuad(float x, float y, float width, float height) 
{
    glPushMatrix();
    glBegin(GL_QUADS);
        glVertex2f(x-width, y-width);
        glVertex2f(x-width, y+width);
        glVertex2f(x+width, y+width);
        glVertex2f(x+width, y-width);
    glEnd();
    glPopMatrix();
}

void drawQuadTex(Image img, float x, float y, float width, float height) 
{
    //intialize textures for background
	glGenTextures(1, &img.texid);
	glBindTexture(GL_TEXTURE_2D, img.texid);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, img.width, img.height, 0,
							GL_RGB, GL_UNSIGNED_BYTE, img.data);
    //
    //draw texture to quad
	glColor3ub(255,255,255);
	glBindTexture(GL_TEXTURE_2D, img.texid);
    glTranslatef(x, y, 0);
	glBegin(GL_QUADS);
		glTexCoord2f(0,1); glVertex2i(0,     0);
		glTexCoord2f(0,0); glVertex2i(0,     height);
		glTexCoord2f(1,0); glVertex2i(width, height);
		glTexCoord2f(1,1); glVertex2i(width, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
    
}