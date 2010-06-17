#ifndef RANDOMPOINTCLOUD
#define RANDOMPOINTCLOUD

#define NOMINMAX

#include <Eigen/Eigen>
USING_PART_OF_NAMESPACE_EIGEN

#include <stdlib.h>

class RandomPointCloud {
	// This class creates a random point cloud using the parameters provided

private:
	float **pointCloud; // the point cloud
	float **colors;		// the colours of the different vertices
	int elements;       // the number of points in total
	int numberOfPlanes;	// the number of planes that have been created in the point cloud
	float** normals;	// the normals of the planes

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