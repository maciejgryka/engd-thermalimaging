#include "Quad.h"

#define MAXLEVELS 4

Quad::Quad(int l, int* ps, int nps, float** pts, float* bs) {
	subdivided = false;
	numberOfPoints = nps;
	pointList = ps;
	points = pts;
	level = l;
	nw = NULL;
	ne = NULL;
	se = NULL;
	sw = NULL;
	boundaries = bs;
	if(nps == 0)
		empty = true;
	else
		empty = false;
}

Quad::~Quad() {
	if (subdivided) {
		/*for (int i = 3; i >= 0; i--) {
			free(quads[i]);
			quads[i] = NULL;
		}*/
	}
	//free(quads);
	free(nw);
	nw = NULL;
	free(ne);
	ne = NULL;
	free(se);
	se = NULL;
	free(sw);
	sw = NULL;
//	quads = NULL;
}

void Quad::subdivide() {
	if (numberOfPoints > 0 && level < MAXLEVELS) {
		int* counters = new int[4];
		for (int i = 0; i < 4; i++) {
			counters[i] = 0;
		}
		float xMid = boundaries[0] + (boundaries[2] - boundaries[0]) / 2.0f;
		float zMid = boundaries[3] + (boundaries[1] - boundaries[3]) / 2.0f;
		for (int i = 0; i < numberOfPoints; i++) {
			if (points[pointList[i]][0] < xMid && points[pointList[i]][2] >= zMid)
				counters[0]++;
			else if (points[pointList[i]][0] >= xMid && points[pointList[i]][2] >= zMid)
				counters[1]++;
			else if (points[pointList[i]][0] > xMid && points[pointList[i]][2] < zMid)
				counters[2]++;
			else
				counters[3]++;
		}
		subdivided = true;
		int** newPoints = new int*[4];
		for (int i = 0; i < 4; i++) {
			newPoints[i] = new int[counters[i]];
			counters[i] = 0;
		}
		for (int i = 0; i < numberOfPoints; i++) {
			if (points[pointList[i]][0] < xMid && points[pointList[i]][2] >= zMid) {
				newPoints[0][counters[0]] = pointList[i];
				counters[0]++;
			} else if (points[pointList[i]][0] >= xMid && points[pointList[i]][2] >= zMid) {
				newPoints[1][counters[1]] = pointList[i];
				counters[1]++;
			} else if (points[pointList[i]][0] > xMid && points[pointList[i]][2] < zMid) {
				newPoints[2][counters[2]] = pointList[i];
				counters[2]++;
			} else {
				newPoints[3][counters[3]] = pointList[i];
				counters[3]++;
			} 
			
		}
		float** bs = new float*[4];
		for (int i = 0; i < 4; i++) {
			bs[i] = new float[4];
		}
		bs[0][0] = boundaries[0];
		bs[0][1] = boundaries[1];
		bs[0][2] = xMid;
		bs[0][3] = zMid;

		bs[1][0] = xMid;
		bs[1][1] = boundaries[1];
		bs[1][2] = boundaries[2];
		bs[1][3] = zMid;

		bs[2][0] = xMid;
		bs[2][1] = zMid;
		bs[2][2] = boundaries[2];
		bs[2][3] = boundaries[3];

		bs[3][0] = boundaries[0];
		bs[3][1] = zMid;
		bs[3][2] = xMid;
		bs[3][3] = boundaries[3];

		/*for (int i = 0; i < 4; i++) {
			quads = new Quad*[4];
			quads[i] = new Quad(MAXLEVELS+1, newPoints[i], counters[i], points, bs[i]);
			quads[i]->subdivide();
		}*/

		nw = new Quad(level+1, newPoints[0], counters[0], points, bs[0]);
		nw->subdivide();
		ne = new Quad(level+1, newPoints[1], counters[1], points, bs[1]);
		ne->subdivide();
		se = new Quad(level+1, newPoints[2], counters[2], points, bs[2]);
		se->subdivide();
		sw = new Quad(level+1, newPoints[3], counters[3], points, bs[3]);
		sw->subdivide();
	}
	
}

void Quad::drawQuad() {
	if (empty) {
		/*
		glBegin(GL_QUADS);
		glColor3f(0.0f,0.0f,0.0f);
		glVertex3f(boundaries[0],0.0f,boundaries[1]);
		glVertex3f(boundaries[2],0.0f,boundaries[1]);
		glVertex3f(boundaries[2],0.0f,boundaries[3]);
		glVertex3f(boundaries[0],0.0f,boundaries[3]);
		glEnd();*/
	} else if (!empty && !subdivided) {
		glBegin(GL_QUADS);
		glColor3f(1.0f,1.0f,1.0f);
		glVertex3f(boundaries[0],0.0f,boundaries[1]);
		glVertex3f(boundaries[2],0.0f,boundaries[1]);
		glVertex3f(boundaries[2],0.0f,boundaries[3]);
		glVertex3f(boundaries[0],0.0f,boundaries[3]);
		//qDebug() << boundaries[0] << boundaries[1] << boundaries[2] << boundaries[3];
		glEnd();
	} else if (subdivided) {
		nw->drawQuad();
		ne->drawQuad();
		se->drawQuad();
		sw->drawQuad();
	}
}

int Quad::getLevels() {
	return MAXLEVELS;
}

int Quad::getSize() {
	return power(2,MAXLEVELS);
}

void Quad::toGrid(int** grid, int* xPath, int* zPath) {

	if (subdivided) {
		xPath[level] = 0;
		zPath[level] = 1;
		nw->toGrid(grid, xPath, zPath);
		xPath[level] = 1;
		zPath[level] = 1;
		ne->toGrid(grid, xPath, zPath);
		xPath[level] = 1;
		zPath[level] = 0;
		se->toGrid(grid, xPath, zPath);
		xPath[level] = 0;
		zPath[level] = 0;
		sw->toGrid(grid, xPath, zPath);
	} else if (!empty) {
		int xStart = 0;
		int zStart = 0;
		for (int i = 0; i < level; i++) {
			xStart += power(2, MAXLEVELS - 1 - i) * xPath[i];
			zStart += power(2, MAXLEVELS - 1 - i) * zPath[i];
		}
		int xEnd = xStart + power(2,MAXLEVELS - level);
		int zEnd = zStart + power(2,MAXLEVELS - level);
		for (int i = xStart; i < xEnd; i++) {
			for (int j = zStart; j < zEnd; j++) {
				grid[i][j] = 1;
			}

		}
	}
}

int** Quad::toGrid() {
	int** grid = new int*[power(2,MAXLEVELS)];
	for (int i = 0; i < power(2,MAXLEVELS); i++) {
		grid[i] = new int[power(2,MAXLEVELS)];
		for(int j = 0; j < power(2,MAXLEVELS); j++) {
			grid[i][j] = 0;
		}
	}
	if(subdivided) {
		int* xPath = new int[MAXLEVELS];
		int* zPath = new int[MAXLEVELS];
		xPath[0] = 0;
		zPath[0] = 1;
		nw->toGrid(grid, xPath, zPath);
		xPath[0] = 1;
		zPath[0] = 1;
		ne->toGrid(grid, xPath, zPath);
		xPath[0] = 1;
		zPath[0] = 0;
		se->toGrid(grid, xPath, zPath);
		xPath[0] = 0;
		zPath[0] = 0;
		sw->toGrid(grid, xPath, zPath);
		free(xPath);
		free(zPath);
		xPath = NULL;
		zPath = NULL;
	}
		
	return grid;
}

int Quad::power(int p, int e) {
	int res = 1;
	for (int i = 0; i < e; i++) {
		res *= p;
	}
	return res;
}
