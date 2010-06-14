#ifndef GRID
#define GRID


#define NOMINMAX

#include <QTDebug>
#include <stdlib.h>
#include <QtGui>
#include <QGLWidget>
//#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

class Grid {

private:
	int** grid;
	float* boundaries;
	int gridSize;

	void clearData();

public:
	Grid();
	~Grid();

	void setGrid(int**,int);
	int** getGrid();
	void setBoundaries(float*);

	void dilate();
	void erode();

	void drawGrid();

};

#endif