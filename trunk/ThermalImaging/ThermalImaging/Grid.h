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
	int** border;

	void clearData();

	void dilateErode(int,int, bool);

public:
	Grid();
	~Grid();

	void setGrid(int**,int);
	int** getGrid();
	int getGridSize();
	void setBoundaries(float*);

	void dilate(int);
	void dilate(int,bool);
	void erode(int);
	void erode(int, bool);
	void dilateAndErode(int);
	void erodeAndDilate(int);
	void dilateAndErode(int, bool);
	void erodeAndDilate(int, bool);

	void drawGrid();
	void drawAsPolygon();
	void calculateBorder();


};

#endif