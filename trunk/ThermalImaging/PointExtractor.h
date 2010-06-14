#ifndef POINTEXTRACTOR
#define POINTEXTRACTOR

#define NOMINMAX

#include "Ply2OpenGL.h"

class PointExtractor {
private:
	float* points;
	float* colors;
	float* normals;
	int noPoints;
	Ply2OpenGL* c;

public:
	PointExtractor();
	void collectPoints();
	float** getPoints();
	float** getColors();
	float** getNormals();
	int getNumberOfPoints();
	

};

#endif