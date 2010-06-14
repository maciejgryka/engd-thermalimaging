#ifndef PLANELIMITFINDER
#define PLANELIMITFINDER

#include <math.h>

#include <iostream>
//#include <QTDebug>
#include <bitset>
using namespace std;


class PlaneLimitFinder {
private:
	float** points;
	int* pointsUsed;
	int noPoints;
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
	void setPoints(float**, int);
	void setPointsUsed(int*);
	void findLimits(int);
	int* getClusters();
	int getNumberOfClusters();

	int* findBiggestCluster(int&);
	void findPolygon(int*, int);
};

#endif
