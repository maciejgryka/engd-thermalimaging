#ifndef QUAD
#define QUAD

#define NOMINMAX

#include <QTDebug>
#include <stdlib.h>
#include <QtGui>
#include <QGLWidget>
//#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

class Quad {

private:
	int level;
	bool empty;
	bool subdivided;
	int numberOfPoints;
	int* pointList;
	Quad* nw; // pos 0: nw, pos 1: ne, pos 2: sw, pos 3: se
	Quad* ne;
	Quad* sw;
	Quad* se;
	float* boundaries; // pos 0: left, pos 1: top, pos 2: right, pos 3: bottom
	float** points;

	int maxLevels;

	int power(int,int);

	void toGrid(int**, int*, int*);


public:
	Quad(int, int, int*, int, float**, float*);
	~Quad();
	int getLevels();
	int getSize();
	void subdivide();
	void drawQuad();
	int** toGrid();
	void dilateGrid();
	void erodeGrid();

	
};

#endif