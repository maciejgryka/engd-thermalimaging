#include "PlaneLimitFinder.h"

#define DISTANCETHRESHOLD 1.8

PlaneLimitFinder::PlaneLimitFinder() {
	noClusters = -1;
	noPoints = -1;
}

PlaneLimitFinder::~PlaneLimitFinder() {}

void PlaneLimitFinder::setPoints(float** p, int np) {
	points = p;
	noPoints = np;
}

void PlaneLimitFinder::setPointsUsed(int* p) {
	pointsUsed = p;
}

void PlaneLimitFinder::arrayToZero(int* arr, int no) {
	for (int i = 0; i < no; i++) {
		arr[i] = 0;
	}
}

void PlaneLimitFinder::copyArray(int* to, int* from, int no) {
	for (int i = 0; i < no; i++) {
		to[i] = from[i];
	}
}

int* PlaneLimitFinder::getClusters() {
	return clusters;
}

int PlaneLimitFinder::getNumberOfClusters() {
	return noClusters;
}

void PlaneLimitFinder::findLimits(int plane) {

	int* addedPoints = new int[noPoints];
	arrayToZero(addedPoints,noPoints);
	int* newAddedPoints = new int[noPoints];
	arrayToZero(newAddedPoints,noPoints);
	clusters = new int[noPoints];
	arrayToZero(clusters,noPoints);

	int cluster = 1;
	int clustered = 0;

	int prevCount = -1;
	int count = 0;

	while (clustered != noPoints) {

		int v;
		for (v = 0; v < noPoints && clusters[v] != 0; v++) {}
		addedPoints[v] = 1;
		clusters[v] = cluster;
		/*qDebug() << addedPoints[0] << addedPoints[1] << addedPoints[2] << addedPoints[3] << addedPoints[4] << addedPoints[5] << addedPoints[6] << addedPoints[7] << addedPoints[8] << addedPoints[9]
		<< addedPoints[10] << addedPoints[11] << addedPoints[12] << addedPoints[13] << addedPoints[14] << addedPoints[15] << addedPoints[16] << addedPoints[17] << addedPoints[18] << addedPoints[19] 
		<< addedPoints[20] << addedPoints[21] << addedPoints[22] << addedPoints[23] << addedPoints[24];
		qDebug() << clusters[0] << clusters[1] << clusters[2] << clusters[3] << clusters[4] << clusters[5] << clusters[6] << clusters[7] << clusters[8] << clusters[9]
		<< clusters[10] << clusters[11] << clusters[12] << clusters[13] << clusters[14] << clusters[15] << clusters[16] << clusters[17] << clusters[18] << clusters[19] 
		<< clusters[20] << clusters[21] << clusters[22] << clusters[23] << clusters[24];*/
		
		clustered++;

		prevCount = -1;
		count = 1;
		while (prevCount != count) {
			prevCount = count;
			for (int i = 0; i < noPoints; i++) {
				if (addedPoints[i] == 1) {
					for (int j = 0; j < noPoints; j++) {
						//qDebug() << i << j << getDistance(i,j);
						if (clusters[j] == 0 && getDistance(i,j) < DISTANCETHRESHOLD) {
							//qDebug() << "Added" << j << "Closest" << i;
							newAddedPoints[j] = 1;
							clusters[j] = cluster;
							count++;
							clustered++;
						}
						
					}
				}
			}
			
			copyArray(addedPoints, newAddedPoints,noPoints);
			arrayToZero(newAddedPoints,noPoints);
			/*qDebug() << addedPoints[0] << addedPoints[1] << addedPoints[2] << addedPoints[3] << addedPoints[4] << addedPoints[5] << addedPoints[6] << addedPoints[7] << addedPoints[8] << addedPoints[9]
			<< addedPoints[10] << addedPoints[11] << addedPoints[12] << addedPoints[13] << addedPoints[14] << addedPoints[15] << addedPoints[16] << addedPoints[17] << addedPoints[18] << addedPoints[19] 
			<< addedPoints[20] << addedPoints[21] << addedPoints[22] << addedPoints[23] << addedPoints[24];
			qDebug() << newAddedPoints[0] << newAddedPoints[1] << newAddedPoints[2] << newAddedPoints[3] << newAddedPoints[4] << newAddedPoints[5] << newAddedPoints[6] << newAddedPoints[7] << newAddedPoints[8] << newAddedPoints[9]
			<< newAddedPoints[10] << newAddedPoints[11] << newAddedPoints[12] << newAddedPoints[13] << newAddedPoints[14] << newAddedPoints[15] << newAddedPoints[16] << newAddedPoints[17] << newAddedPoints[18] << newAddedPoints[19] 
			<< newAddedPoints[20] << newAddedPoints[21] << newAddedPoints[22] << newAddedPoints[23] << newAddedPoints[24];*/
		
		}
		cluster++;
	}
	noClusters = cluster - 1;
	free(addedPoints);
	free(newAddedPoints);
	/*qDebug() << clusters[0] << clusters[1] << clusters[2] << clusters[3] << clusters[4] << clusters[5] << clusters[6] << clusters[7] << clusters[8] << clusters[9]
		<< clusters[10] << clusters[11] << clusters[12] << clusters[13] << clusters[14] << clusters[15] << clusters[16] << clusters[17] << clusters[18] << clusters[19] 
		<< clusters[20] << clusters[21] << clusters[22] << clusters[23] << clusters[24];*/
}

float inline PlaneLimitFinder::getDistance(int p1, int p2) {
	return sqrt(sqr(points[p2][0] - points[p1][0])+sqr(points[p2][2] - points[p1][2]));
}

float inline PlaneLimitFinder::sqr(float n) {
	return n*n;
}

int PlaneLimitFinder::findNumberOfActivePoints(int p) {
	int counter = 0;
	for (int i = 0; i < noPoints; i++) {
		if (pointsUsed[i] == p) {
			counter++;
		}
	}
	return counter;
}

int* PlaneLimitFinder::findActivePoints(int ap, int p) {
	int* listActivePoints = new int[ap];
	int counter = 0;
	for (int i = 0; i < noPoints; i++) {
		if (pointsUsed[i] == p) {
			listActivePoints[counter] = i;
			counter++;
		}
	}
	return listActivePoints;
}

int* PlaneLimitFinder::findBiggestCluster(int& elements) {
	if (noClusters < 1)
		return NULL;
	// intitialize counters to 0
	int* count = new int[noClusters];
	for (int i = 0; i < noClusters; i++) {
		count[i] = 0;
	}
	//count how often each cluster comes up
	for (int i = 0; i < noPoints; i++) {
		count[clusters[i]]++;
	}
	// find biggest cluster
	int biggestCluster = -1;
	int numberInBiggest = -1; 
	for (int i = 0; i < noClusters; i++) {
		if (count[i] > numberInBiggest) {
			numberInBiggest = count[i];
			biggestCluster = i;
		}
	}
	free(count);
	int* clusterElements = new int[numberInBiggest];
	int position = -1;
	for (int i = 0; i < noPoints; i++) {
		if (clusters[i] == biggestCluster) {
			clusterElements[++position] = i;
		}
	}
	elements = numberInBiggest;
	return clusterElements;
}

void PlaneLimitFinder::findPolygon(int* clusterElements, int numberOfElements) {
	float minX = +1000.0f;
	float maxX = -1000.0f;
	float minZ = +1000.0f;
	float maxZ = -1000.0f;
	float x, z;

	for (int i = 0; i < numberOfElements; i++) {
		x = points[clusterElements[i]][0];
		z = points[clusterElements[i]][2];
		if (x < minX)
			minX = x;
		if (x > maxX)
			maxX = x;
		if (z < minZ)
			minZ = z;
		if (z > maxZ)
			maxZ = z;
	}
}