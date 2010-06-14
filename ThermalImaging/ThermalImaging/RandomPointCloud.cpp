#include "RandomPointCloud.h"

RandomPointCloud::RandomPointCloud() {
	elements = 0;
}

RandomPointCloud::~RandomPointCloud() {
	clearPointCloud();
}

void RandomPointCloud::makePointCloud(int planes, int elementsPerPlane, int outliers) {
	if (planes > 0 && elementsPerPlane > 1) {
		float spread = 5.0f;
		float noise = 0.1f;
		clearPointCloud();
		elements = planes * elementsPerPlane + outliers;
		numberOfPlanes = planes;
		pointCloud = new float*[elements];
		colors = new float*[elements];
		normals = new float*[planes];

		for (int p = 0; p < planes; p++) {
			Vector3f normal;
			normal.setRandom().normalize();
			normals[p] = new float[3];
			normals[p][0] = normal(0);
			normals[p][1] = normal(1);
			normals[p][2] = normal(2);

			Vector3f origin;
			origin.setRandom().normalize();

			Vector3f color (getRandomOverZero(1.0f),getRandomOverZero(1.0f),getRandomOverZero(1.0f));

			Vector3f v1 (1,1,(-normal(0)-normal(1))/normal(2));
			v1.normalize();
			Vector3f v2 = normal.cross(v1);
			v2.normalize();

			Vector3f res;

			for (int i = 0; i < elementsPerPlane; i++) {
				pointCloud[p * elementsPerPlane + i] = new float[3];
				colors[p * elementsPerPlane + i] = new float[3];
				colors[p * elementsPerPlane + i][0] = color(0);
				colors[p * elementsPerPlane + i][1] = color(1);
				colors[p * elementsPerPlane + i][2] = color(2);
				res = origin + getRandomAroundZero(spread) * v1 + getRandomAroundZero(spread) * v2;
				pointCloud[p * elementsPerPlane + i][0] = res(0) + getRandomAroundZero(noise);
				pointCloud[p * elementsPerPlane + i][1] = res(1) + getRandomAroundZero(noise);
				pointCloud[p * elementsPerPlane + i][2] = res(2) + getRandomAroundZero(noise);
			}
		}
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
	return (float) rand() / (float) RAND_MAX * 2.0f * m - m;
}

float inline RandomPointCloud::getRandomOverZero(float m) {
	return (float) rand() / (float) RAND_MAX * m;
}

float** RandomPointCloud::getPointCloud() {
	return pointCloud;
}

float** RandomPointCloud::getColors() {
	return colors;
}

float** RandomPointCloud::getNormals() {
	return normals;
}

int RandomPointCloud::getNumberOfElements() {
	return elements;
}

void RandomPointCloud::clearPointCloud() {
	if (elements > 0) {
		for (int i = 0; i < elements; i++) {
			free(pointCloud[i]);
			free(colors[i]);
			
		}
		for (int i = 0; i < numberOfPlanes; i++) {
			free(normals[i]);
		}
		free(pointCloud);
		free(colors);
		free(normals);
	}
}