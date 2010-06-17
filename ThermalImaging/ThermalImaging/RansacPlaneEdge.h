#ifndef RANSACPLANEEDGE
#define RANSACPLANEEDGE

#include <math.h>
#include "Grid.h"

using namespace std;

class RansacPlaneEdge {
public:
	RansacPlaneEdge(){ maxInliers = 0; iterations = 1000;};
	~RansacPlaneEdge(){};
	bool findBestEdge();
	bool setXYZBorders(vector<float> xBorder, vector<float> yBorder, vector<float> zBorder) 
	{
		this->xBorder = xBorder;
		this->yBorder = yBorder;
		this->zBorder = zBorder;
		return true;
	};

	bool setInlierDistance(float inlierDistance)
	{
		this->inlierDistance = inlierDistance;
		return true;
	};

	int* getBestPoints() { return bestPoints; };

private:
	float inlierDistance;
	int iterations;
	int maxInliers;

	int bestPoints[2];
	vector<float> xBorder;
	vector<float> yBorder;
	vector<float> zBorder;
};

#endif