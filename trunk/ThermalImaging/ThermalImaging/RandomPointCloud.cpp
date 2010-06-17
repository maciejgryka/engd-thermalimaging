#include "RandomPointCloud.h"

RandomPointCloud::RandomPointCloud() {
	// initialises the number of elements to 0
	elements = 0;
}

RandomPointCloud::~RandomPointCloud() {
	// clear memory used
	clearPointCloud();
}

void RandomPointCloud::makePointCloud(int planes, int elementsPerPlane, int outliers) {
	// if we have at least 1 plane and at least 3 elements per plane
	if (planes > 0 && elementsPerPlane > 2) {
		float spread = 5.0f;	// the extent of the plane
		float noise = 0.1f;		// the noise in the plane
		clearPointCloud();		// if a point cloud has been created before, clear it
		elements = planes * elementsPerPlane + outliers; // calculate the number of elements

		// initialise all variables
		numberOfPlanes = planes;
		pointCloud = new float*[elements];
		colors = new float*[elements];
		normals = new float*[planes];

		for (int p = 0; p < planes; p++) {
			// create a random normal
			Vector3f normal;
			normal.setRandom().normalize();
			normals[p] = new float[3];
			normals[p][0] = normal(0);
			normals[p][1] = normal(1);
			normals[p][2] = normal(2);

			// create a random origin
			Vector3f origin;
			origin.setRandom().normalize();

			// initialise random color
			Vector3f color (getRandomOverZero(1.0f),getRandomOverZero(1.0f),getRandomOverZero(1.0f));

			// find the two vectors to span the plane
			Vector3f v1 (1,1,(-normal(0)-normal(1))/normal(2));
			v1.normalize();
			Vector3f v2 = normal.cross(v1);
			v2.normalize();

			Vector3f res;

			for (int i = 0; i < elementsPerPlane; i++) {
				pointCloud[p * elementsPerPlane + i] = new float[3];
				// set color
				colors[p * elementsPerPlane + i] = new float[3];
				colors[p * elementsPerPlane + i][0] = color(0);
				colors[p * elementsPerPlane + i][1] = color(1);
				colors[p * elementsPerPlane + i][2] = color(2);
				// get x, y, z coordinates and add noise
				res = origin + getRandomAroundZero(spread) * v1 + getRandomAroundZero(spread) * v2;
				pointCloud[p * elementsPerPlane + i][0] = res(0) + getRandomAroundZero(noise);
				pointCloud[p * elementsPerPlane + i][1] = res(1) + getRandomAroundZero(noise);
				pointCloud[p * elementsPerPlane + i][2] = res(2) + getRandomAroundZero(noise);
			}
		}
		// include some outliers, which are on none of the planes (but can be by coincidence
		for (int o = 0; o < outliers; o++) {
			pointCloud[planes * elementsPerPlane + o] = new float[3];
			colors[planes * elementsPerPlane + o] = new float[3];
			colors[planes * elementsPerPlane + o][0] = getRandomOverZero(1.0f);
			colors[planes * elementsPerPlane + o][1] = getRandomOverZero(1.0f);
			colors[planes * elementsPerPlane + o][2] = getRandomOverZero(1.0f);

			pointCloud[planes * elementsPerPlane + o][0] = getRandomAroundZero(spread);
			pointCloud[planes * elementsPerPlane + o][1] = getRandomAroundZero(spread);
			pointCloud[planes * elementsPerPlane + o][2] = getRandomAroundZero(spread);
		}

	}
}

float inline RandomPointCloud::getRandomAroundZero(float m) {
	// creates a random number between -m and m
	return (float) rand() / (float) RAND_MAX * 2.0f * m - m;
}

float inline RandomPointCloud::getRandomOverZero(float m) {
	// creates a random number between 0 and m
	return (float) rand() / (float) RAND_MAX * m;
}

float** RandomPointCloud::getPointCloud() {
	// returns the pointCloud
	return pointCloud;
}

float** RandomPointCloud::getColors() {
	// returns the colors
	return colors;
}

float** RandomPointCloud::getNormals() {
	// returns the normals
	return normals;
}

int RandomPointCloud::getNumberOfElements() {
	// returns the number of elements
	return elements;
}

void RandomPointCloud::clearPointCloud() {
	if (elements > 0) {
		for (int i = 0; i < elements; i++) {
			delete[] pointCloud[i];
			delete[] colors[i];
			
		}
		for (int i = 0; i < numberOfPlanes; i++) {
			delete[] normals[i];
		}
		delete[] pointCloud;
		delete[] colors;
		delete[] normals;
	}
}