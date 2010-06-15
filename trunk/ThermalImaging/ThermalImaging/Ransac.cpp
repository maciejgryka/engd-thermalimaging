#include "Ransac.h"

Ransac::Ransac(){
	srand(time(0));

}

Ransac::~Ransac(){
	free(pointsUsed);
}

void Ransac::setInlierDistance(float dis) {
	inlierDistance = dis;
}

void Ransac::setPoints(float** ps, int no) {
	pointCloud = ps;
	numberOfPoints = no;
	pointsUsed = new int[numberOfPoints];
	for (int i = 0; i < numberOfPoints; i++) {
		pointsUsed[i] = 0;
	}
	pointsLeft = numberOfPoints;
}

void Ransac::setIterations(int is) {
	iterations = is;
}

int* Ransac::findBestPlane(int planeNumber, int &numberOfPointsOnBestPlane, Vector3f &origin, Vector3f &normal, int* bestPoints) {
	//iterations = pointsLeft / 30;

	int maxInliers = 0, inlier = 0;
	//int bestPoints[3] = {-1,-1,-1};

	int pos1, pos2, pos3;
	float* P1; float* P2; float* P3; float* P;
	float A,B,C,D;
	float dis;
	for (int i=0; i < iterations; i++) {
    // get Random Points
		pos1 = rand() % numberOfPoints;
		pos2 = rand() % numberOfPoints;
		pos3 = rand() % numberOfPoints;
		// ensure all points are different
		while (pointsUsed[pos1] != 0 || pointsUsed[pos2] != 0 || pointsUsed[pos3] != 0 || pos1 == pos2 || pos2 == pos3 || pos1 == pos3) {
			pos1 = rand() % numberOfPoints;
			pos2 = rand() % numberOfPoints;
			pos3 = rand() % numberOfPoints;
		}
		
		P1 = pointCloud[pos1];
		P2 = pointCloud[pos2];
		P3 = pointCloud[pos3];
		A = P1[1] * (P2[2] - P3[2]) + P2[1] * (P3[2] - P1[2]) + P3[1] * (P1[2] - P2[2]);
		B = P1[2] * (P2[0] - P3[0]) + P2[2] * (P3[0] - P1[0]) + P3[2] * (P1[0] - P2[0]);
		C = P1[0] * (P2[1] - P3[1]) + P2[0] * (P3[1] - P1[1]) + P3[0] * (P1[1] - P2[1]);
		D = -(P1[0] * (P2[1] * P3[2] - P3[1] * P2[2]) + P2[0] * (P3[1] * P1[2] - P1[1] * P3[2]) + P3[0] * (P1[1] * P2[2] - P2[1] * P1[2]));
		
		inlier = 0;
    
		for (int j = 0; j < numberOfPoints; j++) {
			// calculate distance between point and plane
			P = pointCloud[j];
			dis = abs(A*P[0] + B*P[1] + C*P[2] + D) / sqrt(A*A + B*B + C*C);
	  
			if (dis < inlierDistance){
				inlier++;
			}
		}
		if (inlier > maxInliers) {
			maxInliers = inlier;
			bestPoints[0] = pos1;
			bestPoints[1] = pos2;
			bestPoints[2] = pos3;
		}
		
	}


	P1 = pointCloud[bestPoints[0]];
	P2 = pointCloud[bestPoints[1]];
	P3 = pointCloud[bestPoints[2]];
	A = P1[1] * (P2[2] - P3[2]) + P2[1] * (P3[2] - P1[2]) + P3[1] * (P1[2] - P2[2]);
	B = P1[2] * (P2[0] - P3[0]) + P2[2] * (P3[0] - P1[0]) + P3[2] * (P1[0] - P2[0]);
	C = P1[0] * (P2[1] - P3[1]) + P2[0] * (P3[1] - P1[1]) + P3[0] * (P1[1] - P2[1]);
	D = -(P1[0] * (P2[1] * P3[2] - P3[1] * P2[2]) + P2[0] * (P3[1] * P1[2] - P1[1] * P3[2]) + P3[0] * (P1[1] * P2[2] - P2[1] * P1[2]));
    
	int pointsPlaced = 0;
	int *pointsOnPlane = new int[maxInliers];

	for (int j = 0; j < numberOfPoints; j++) {
		// calculate distance between point and plane
		P = pointCloud[j];
		float dis = abs(A*P[0] + B*P[1] + C*P[2] + D) / sqrt(A*A + B*B + C*C);
  
		if (dis < inlierDistance){
			pointsOnPlane[pointsPlaced] = j;
			pointsPlaced++;
			pointsUsed[j] = planeNumber;
		}
	}

	numberOfPointsOnBestPlane = maxInliers;
	pointsLeft -= maxInliers;
	
	Vector3f p0(pointCloud[bestPoints[0]][0], pointCloud[bestPoints[0]][1], pointCloud[bestPoints[0]][2]);
	Vector3f p1(pointCloud[bestPoints[1]][0], pointCloud[bestPoints[1]][1], pointCloud[bestPoints[1]][2]);
	Vector3f p2(pointCloud[bestPoints[2]][0], pointCloud[bestPoints[2]][1], pointCloud[bestPoints[2]][2]);

	normal = (p1-p0).cross(p2-p1);
	normal = normal.normalized();
	origin = p0;



	return pointsUsed;

}

int* Ransac::convertToNumberList(int* pointsUsed, int numberOfPoints, int numberOfPointsOnPlane, int planeNumber) {
	int* numberList = new int [numberOfPointsOnPlane];
	int count = 0;
	for (int i = 0; i < numberOfPoints; i++) {
		if (pointsUsed[i] == planeNumber) {
			numberList[count] = i;
			count++;
		}
	} 
	return numberList;
}

