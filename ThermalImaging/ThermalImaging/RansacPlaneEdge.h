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
		pointList = NULL;
		boundaries = NULL;
	};

	~RansacPlaneEdge(){};

	bool reset()
	{
		maxInliers = 0;
		boundExtension = 0.5f;
		/*xBorder.clear();
		yBorder.clear();
		zBorder.clear();*/
		inlierDistance = 0.0f;
		iterations = 0;
		percentOfChillPoints = 0.0f;
		bestPoints.clear();
		corners.clear();
		pointsUsed.clear();
		lineCoeffs.clear();
		if (pointList != NULL) {
			delete[] pointList;
			pointList = NULL;
		}
		if (boundaries != NULL) {
			delete[] boundaries;
			boundaries = NULL;
		}
		return false;
	}

	bool findEdges();
	bool findBestEdge(vector<int> &pointsUsed);
	bool setXYZBorders(const vector<float> xBorder, const vector<float> yBorder, const vector<float> zBorder) 
	{
		if (pointList != NULL)
			delete[] pointList;
		pointList = new int[xBorder.size()];
		for (int i = 0; i < xBorder.size(); i++) {
			pointList[i] = 0;
		}
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

	bool setBoundaries(float *bds)
	{
		if (boundaries != NULL) {
			delete[] boundaries;
			boundaries = NULL;
		}

		boundaries = new float[4];
		// extend the boundaries slightly
		boundaries[0] = bds[0] - boundExtension * abs(bds[2] - bds[0]);	// minX - boundExtension
		boundaries[2] = bds[2] + boundExtension * abs(bds[2] - bds[0]);	// maxX - boundExtension
		boundaries[3] = bds[3] - boundExtension * abs(bds[1] - bds[3]);	// minZ - boundExtension
		boundaries[1] = bds[1] + boundExtension * abs(bds[1] - bds[3]);	// maxZ - boundExtension

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

	int* pointList;

	int findLineCoeffs(int pointIndex, int lastLineIndex);
	int findPointOnLine(int lineIndex, int currentPointIndex);
	
	bool isFiniteNumber(double x)
	{
		return (x <= DBL_MAX && x >= -DBL_MAX);
	}
};

#endif