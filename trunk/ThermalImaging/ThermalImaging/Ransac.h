#ifndef RANSAC
#define RANSAC

#define NOMINMAX

#include <Eigen/Eigen>
USING_PART_OF_NAMESPACE_EIGEN

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <ctime>

#include "QTextStream.h"
#include "QFile.h"

using namespace std;

class Ransac {
	/* This class performs Ransac to find planes in a point cloud.
	* To do so, the Inlier Distance, the iterations and the points need to be set.
	* It will return some basic information about the plane.
	* The class is also able to convert a list of 0s and 1s into a list of numbers, where the new list contains the position of every elment that had a 1 before.
	* It also prints the elements into a file */

public:
	Ransac();
	~Ransac();
	void setInlierDistance(float);
	void setIterations(int);
	void setPoints(float**,int);
	int* findBestPlane(int, int&, Vector3f&, Vector3f&, int*);
	int* convertToNumberList(int*, int, int, int);
	void printToFile(QString, int*, int);


private:
	float inlierDistance;	// The distance between the plane spanned and a point, which will cause the point to be considered an inlier.
	int iterations;			// the number of iterations that Ransac should be performed
	float** pointCloud;		// The point cloud as a N by 3 vector
	int *pointsUsed;		// contains information on which points have been used. 1 for has been used, 0 otherwise
	int numberOfPoints;		// the number of points in the point cloud
	int pointsLeft;			// the number of points that have not been allocated to a plane yet

};

#endif