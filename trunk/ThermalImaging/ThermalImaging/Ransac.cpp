#include "Ransac.h"

Ransac::Ransac(){

	//intialise random number generator
	int seed = time(0);
	qDebug() << "Seed " << seed;
	seed = 1277908061;
	srand(seed);

}

Ransac::~Ransac(){
	// free the memory of all the memory acquired by this class
	//delete[] pointsUsed;
}

void Ransac::setInlierDistance(float dis) {
	// sets the distance from the plane when a point will still be considered an inlier
	inlierDistance = dis;
}

void Ransac::setPoints(float** ps, int no) {
	// sets the pointCloud variable and also initialises pointsused to all zeros
	// sets the number of points
	pointCloud = ps;
	numberOfPoints = no;
	pointsLeft = numberOfPoints;
}

void Ransac::setIterations(int is) {
	// sets the number of iterations Ransac should take to find the best plane
	iterations = is;
}

void Ransac::findBestPlane(int planeNumber, int &numberOfPointsOnBestPlane, Vector3f &origin, Vector3f &normal, int* bestPoints, int* pointList) {
	//finds the best plane
	//input: the plane number
	//output: a 0/1 array where all the points that have been used have a value of 1, the number of points on the plane, the planes origin and normal, as well as the 3 vertex numbers of the vertices that span the best plane
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
		// ensure all points are different and that they have not been used before
		while (pointList[pos1] != 0 || pointList[pos2] != 0 || pointList[pos3] != 0 || pos1 == pos2 || pos2 == pos3 || pos1 == pos3) {
			pos1 = rand() % numberOfPoints;
			pos2 = rand() % numberOfPoints;
			pos3 = rand() % numberOfPoints;
		}
		
		P1 = pointCloud[pos1];
		P2 = pointCloud[pos2];
		P3 = pointCloud[pos3];
		// calculate A, B, C and D, for the plane spanned by points pos1, pos2 and pos 3
		A = P1[1] * (P2[2] - P3[2]) + P2[1] * (P3[2] - P1[2]) + P3[1] * (P1[2] - P2[2]);
		B = P1[2] * (P2[0] - P3[0]) + P2[2] * (P3[0] - P1[0]) + P3[2] * (P1[0] - P2[0]);
		C = P1[0] * (P2[1] - P3[1]) + P2[0] * (P3[1] - P1[1]) + P3[0] * (P1[1] - P2[1]);
		D = -(P1[0] * (P2[1] * P3[2] - P3[1] * P2[2]) + P2[0] * (P3[1] * P1[2] - P1[1] * P3[2]) + P3[0] * (P1[1] * P2[2] - P2[1] * P1[2]));
		
		// set inliers to 0
		inlier = 0;
    
		for (int j = 0; j < numberOfPoints; j++) {

			
			// calculate distance between point and plane
			P = pointCloud[j];
			// calculate the distance between the point and the plane
			dis = abs(A*P[0] + B*P[1] + C*P[2] + D) / sqrt(A*A + B*B + C*C);
	  
			if (dis < inlierDistance){
				inlier++;
			}
			/*if (pointList[j] != 0 && dis < inlierDistance) {
				inlier--;
			}*/
		}
		// if the inlier number is better than the best plane found so far, then we take these points to be considered the new best plane
		if (inlier > maxInliers) {
			maxInliers = inlier;
			bestPoints[0] = pos1;
			bestPoints[1] = pos2;
			bestPoints[2] = pos3;
		}
		
	}


	// use the best points, recreate the plane
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
		
		// find all inliers and mark them
		if (dis < inlierDistance){
			pointsOnPlane[pointsPlaced] = j;
			pointsPlaced++;
			pointList[j] = planeNumber;
		}
	}

	numberOfPointsOnBestPlane = maxInliers;
	pointsLeft -= maxInliers;
	
	// calculate the normal and origin of the plane
	Vector3f p0(pointCloud[bestPoints[0]][0], pointCloud[bestPoints[0]][1], pointCloud[bestPoints[0]][2]);
	Vector3f p1(pointCloud[bestPoints[1]][0], pointCloud[bestPoints[1]][1], pointCloud[bestPoints[1]][2]);
	Vector3f p2(pointCloud[bestPoints[2]][0], pointCloud[bestPoints[2]][1], pointCloud[bestPoints[2]][2]);

	normal = (p1-p0).cross(p2-p1);
	normal = normal.normalized();
	origin = p0;


}

int* Ransac::convertToNumberList(int* pointsUsed, int numberOfPoints, int numberOfPointsOnPlane, int planeNumber) {
/*	if (planeNumber == 2)
	{
		int* blee;
		blee = new int[1];
	}
	int* bla;
	bla = new int[1];*/
	// converts 0/1 array into an array which contains the position of all the elements that have a the same value as the plane number
	int* numberList;
	
	numberList = new (nothrow) int[numberOfPointsOnPlane];
	//memset(numberList, 0 , sizeof(int) * numberOfPointsOnPlane);

	int count = 0;
	for (int i = 0; i < numberOfPoints; i++) {
		// if the point is used in the corresponding plane
		if (pointsUsed[i] == planeNumber) {
			numberList[count] = i;
			count++;
		}
	} 
	return numberList;
}

void Ransac::printToFile(QString fileName, int* data, int nElements) {
	// prints all the points used into a data file seperated by a space, so that it can be read by the PointViewer
	QFile file (fileName);
	if (!file.open(QIODevice::WriteOnly)) {
		return;
	}
	QTextStream ts(&file);
	for (int i = 0; i < nElements; i++) {
		ts << data[i] << " ";
	}
	file.close();

}