#ifndef RANDOMPOINTCLOUD
#define RANDOMPOINTCLOUD

#define NOMINMAX

#include <Eigen/Eigen>
USING_PART_OF_NAMESPACE_EIGEN

#include <stdlib.h>

class RandomPointCloud {

private:
	float **pointCloud;
	float **colors;
	bool used;
	int elements;
	int numberOfPlanes;
	float** normals;

	float getRandomAroundZero(float);
	float getRandomOverZero(float);

public:
	RandomPointCloud();
	~RandomPointCloud();
	void makePointCloud(int, int, int);
	float **getPointCloud();
	void clearPointCloud();
	int getNumberOfElements();
	float **getColors();
	float** getNormals();

};

#endif