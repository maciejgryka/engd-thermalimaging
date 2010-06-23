#include "PointStorage.h"

PointStorage::PointStorage() {
	fullSize = -1;
	subSize = -1;
}

PointStorage::~PointStorage() {
	clearData();
}

void PointStorage::clearData() {
	if (subSize != -1) {
		for (int i = 0; i < subSize; i++) {
			delete[] subCloud[i];
			subCloud[i] = NULL;
			delete[] subColors[i];
			subColors[i] = NULL;
		}
		delete[] subCloud;
		subCloud = NULL;
		delete[] subColors;
		subColors = NULL;
	}
}

void PointStorage::setCloud(float** points, float** colors, int number) {
	fullCloud = points;
	fullColors = colors;
	fullSize = number;
	subSize = -1;
}

void PointStorage::makeSubCloud(float fraction) {
	if (fraction < 0.0f || fraction > 1.0f)
		return;
	clearData();
	subSize = (int) ((float) fullSize * fraction);
	subCloud = new float*[subSize];
	subColors = new float*[subSize];
	int* used = new int[fullSize];
	for (int i = 0; i < fullSize; i++) {
		used[i] = 0;
	}
	int no;
	for (int i = 0; i < subSize; i++) {
		no = rand() % fullSize;
		while (used[no] != 0) {
			no = rand() % fullSize;
		}
		used[no] = 1;
		subCloud[i] = new float[3];
		subColors[i] = new float[3];
		for (int j = 0; j < 3; j++) {
			subCloud[i][j] = fullCloud[no][j];
			subColors[i][j] = fullColors[no][j];
		}
	}

}

int PointStorage::getFullSize() {
	return fullSize;
}

float** PointStorage::getFullCloud() {
	return fullCloud;
}

float** PointStorage::getFullColor() {
	return fullColors;
}

int PointStorage::getSubSize() {
	return subSize;
}

float** PointStorage::getSubCloud() {
	return subCloud;
}

float** PointStorage::getSubColor() {
	return subColors;
}
