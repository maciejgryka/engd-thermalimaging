#ifndef PLANELIMITFINDER
#define PLANELIMITFINDER

#include <math.h>

#include <iostream>
//#include <QTDebug>
#include <bitset>
using namespace std;

#include "QTextStream.h"
#include "QFile.h"


class PlaneLimitFinder {
private:
	float** points;
	int noPoints;
	int* pointsUsed;
	int* clusters;
	int noClusters;

	int findNumberOfActivePoints(int);
	int* findActivePoints(int, int);
	float getDistance(int,int);
	float sqr(float);
	void arrayToZero(int*,int);
	void copyArray(int*,int*,int);

public:
	PlaneLimitFinder();
	~PlaneLimitFinder();
	void setPoints(float**);
	void setPointsUsed(int*, int);
	void findLimits(int);
	int* getClusters();
	int getNumberOfClusters();
	float** getPoints();
	int getNumberOfPoints();



	int* findBiggestCluster(int&, int*, QString);
	int* findBiggestCluster(int&, int*);
	void findPolygon(int*, int);
};

#endif
