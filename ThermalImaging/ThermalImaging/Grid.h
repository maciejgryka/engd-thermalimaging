#ifndef GRID
#define GRID


#define NOMINMAX

#include <Eigen/Eigen>
USING_PART_OF_NAMESPACE_EIGEN

#include <QTDebug>
#include <stdlib.h>
#include <QtGui>
#include <QGLWidget>
//#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <vector>

using namespace std;

class Grid {

private:
	int** grid;
	float* boundaries;
	int gridSize;
	int** border;
	vector<float> xBorder;
	vector<float> yBorder;
	vector<float> zBorder;

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
	vector<float> getXBorder();
	vector<float> getYBorder();
	vector<float> getZBorder();

	void unrotateBorder(Matrix3f, Vector3f);


};

#endif