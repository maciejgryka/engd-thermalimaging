#ifndef RANSACPLANEEDGE
#define RANSACPLANEEDGE

#include <math.h>
#include "Grid.h"

using namespace std;

class RansacPlaneEdge {
public:
	RansacPlaneEdge()
	{ 
		maxInliers = 0;
		boundExtension = 0.5f;
	};

	~RansacPlaneEdge(){};
	bool findEdges();
	bool findBestEdge(vector<int> &pointsUsed);
	bool setXYZBorders(const vector<float> &xBorder, const vector<float> &yBorder, const vector<float> &zBorder) 
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

	bool setIterations(int iterations)
	{
		this->iterations = iterations;
		return true;
	};

	bool setPercentOfChillPoints(float percent)
	{
		this->percentOfChillPoints = percent;
		return true;
	};

	bool setBoundaries(float *boundaries)
	{
		this->boundaries = boundaries;
		// extend the boundaries slightly
		boundaries[0] -= boundExtension * abs(boundaries[2] - boundaries[0]);	// minX - boundExtension
		boundaries[2] += boundExtension * abs(boundaries[2] - boundaries[0]);	// maxX - boundExtension
		boundaries[3] -= boundExtension * abs(boundaries[1] - boundaries[3]);	// minZ - boundExtension
		boundaries[1] += boundExtension * abs(boundaries[1] - boundaries[3]);	// maxZ - boundExtension

		return true;
	};

	vector<int> getBestPoints() { 
		return bestPoints; 
	};
	
	vector<vector<float>> getCorners() { 
		return corners; 
	};
	
	vector<int> getPointsUsed() {return pointsUsed; };
	vector<float> getXBorder() { return xBorder; };
	vector<float> getYBorder() { return yBorder; };
	vector<float> getZBorder() { return zBorder; };

private:
	float inlierDistance;		// threshold to count point as an inlier
	int maxInliers;				// number of inliers of the best line for far
	int iterations;				// number of iterations of RANSAC
	float percentOfChillPoints;	// number of points that can be left unassigned

	float *boundaries;
	float boundExtension;

	vector<int> bestPoints;
	vector<vector<float> > corners;
	vector<vector<float> > lineCoeffs;

	vector<float> xBorder;
	vector<float> yBorder;
	vector<float> zBorder;

	vector<int> pointsUsed;

	int findLineCoeffs(int pointIndex, int lastLineIndex);
	int findPointOnLine(int lineIndex, int currentPointIndex);
	
	bool isFiniteNumber(double x)
	{
		return (x <= DBL_MAX && x >= -DBL_MAX);
	}
};

#endif