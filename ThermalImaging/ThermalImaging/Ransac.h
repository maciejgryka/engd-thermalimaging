#ifndef RANSAC
#define RANSAC

#define NOMINMAX

#include <Eigen/Eigen>
USING_PART_OF_NAMESPACE_EIGEN

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <ctime>

using namespace std;

class Ransac {

public:
	Ransac();
	~Ransac();
	void setInlierDistance(float);
	void setIterations(int);
	void setPoints(float**,int);
	int* findBestPlane(int, int&, Vector3f&, Vector3f&, int*);
	int* Ransac::convertToNumberList(int*, int, int, int);


private:
	float inlierDistance;
	int iterations;
	float** pointCloud;
	int *pointsUsed;
	int numberOfPoints;
	int pointsLeft;

};

#endif