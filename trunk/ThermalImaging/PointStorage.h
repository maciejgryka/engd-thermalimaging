#ifndef POINTSTORAGE
#define POINTSTORAGE

#include <stdlib.h>

class PointStorage {

private:
	int fullSize;
	float** fullCloud;
	float** fullColors;

	int subSize;
	float** subCloud;
	float** subColors;

	void clearData();

private:
	PointStorage();
	~PointStorage();
	void setCloud(float**, float**, int);
	void makeSubCloud(float);
	int getFullSize();
	float** getFullCloud();
	float** getFullColor();
	int getSubSize();
	float** getSubCloud();
	float** getSubColor();

};

#endif