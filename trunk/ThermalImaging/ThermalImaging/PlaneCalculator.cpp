#include "PlaneCalculator.h"

PlaneCalculator::PlaneCalculator() {
	//plf = new PlaneLimitFinder();
	//numberOfPointsOnPlane = -1;
}

PlaneCalculator::~PlaneCalculator() {
	//free(plf);
	removeData();
}

/*int* PlaneCalculator::getPointsUsed() {
	return pointsUsed;
}

int PlaneCalculator::getNumberOfPointsOnPlane() {
	return numberOfPointsOnPlane;
}*/

void PlaneCalculator::removeData() {
	/*for (int i = 0; i < numberOfPointsOnPlane; i++) {
		free(points[i]);
	}
	free(points);*/
}

void PlaneCalculator::makeMatrixAndVector(int* pointsUsed, int numberOfPoints, Vector3f normal, Vector3f& translationVector, Matrix3f& rotationMatrix) {
	for (int i = 0; i < 3; i++) {
		translationVector(i) = points[pointsUsed[0]][i];
	}
	

	Vector3f yAxis (0.0f,1.0f,0.0f);

	float angle = -acos(yAxis.dot(normal));
	//angle = 3.14159265358f/4.0f;
	Vector3f axis = yAxis.cross(normal).normalized();

	float x = axis(0);
	float y = axis(1);
	float z = axis(2);

	if (angle == 0.0 || (x == 0.0 && y == 0.0 && z == 0.0)) {
		return;
	}

	// make a normalized quaternion
	double w = cos(0.5*angle);
	double f = sin(0.5*angle)/sqrt(x*x+y*y+z*z);
	x *= f;
	y *= f;
	z *= f;

	// convert the quaternion to a matrix
	//rotationMatrix(3,3);

	double ww = w*w;
	double wx = w*x;
	double wy = w*y;
	double wz = w*z;

	double xx = x*x;
	double yy = y*y;
	double zz = z*z;

	double xy = x*y;
	double xz = x*z;
	double yz = y*z;

	double s = ww - xx - yy - zz;

	rotationMatrix(0,0) = xx*2 + s;
	rotationMatrix(1,0) = (xy + wz)*2;
	rotationMatrix(2,0) = (xz - wy)*2;
	//matrix[3][0] = 0;

	rotationMatrix(0,1) = (xy - wz)*2;
	rotationMatrix(1,1) = yy*2 + s;
	rotationMatrix(2,1) = (yz + wx)*2;
	//matrix[3][1] = 0;

	rotationMatrix(0,2) = (xz + wy)*2;
	rotationMatrix(1,2) = (yz - wx)*2;
	rotationMatrix(2,2) = zz*2 + s;
}

void PlaneCalculator::rotateBack(int* pointsUsed, int numberOfPoints, Matrix3f rotationMatrix) {
	rotationMatrix = rotationMatrix.inverse();
	rotate(pointsUsed, numberOfPoints, rotationMatrix);
	rotationMatrix = rotationMatrix.inverse();
}

void PlaneCalculator::rotateBack(int* pointList, int value, int numberOfPoints, Matrix3f rotationMatrix) {
	rotationMatrix = rotationMatrix.inverse();
	rotate(pointList, value, numberOfPoints, rotationMatrix);
	rotationMatrix = rotationMatrix.inverse();
}

void PlaneCalculator::rotate(int* pointsUsed, int numberOfPoints, Matrix3f rotationMatrix) {

	Vector3f v; 
	for (int i = 0; i < numberOfPoints; i++) {
		v(0) = points[pointsUsed[i]][0];
		v(1) = points[pointsUsed[i]][1];
		v(2) = points[pointsUsed[i]][2];
		v = (rotationMatrix * v);
		points[pointsUsed[i]][0] = v(0);
		points[pointsUsed[i]][1] = v(1);
		points[pointsUsed[i]][2] = v(2);
	}

}

void PlaneCalculator::rotate(int* pointList, int value, int numberOfPoints, Matrix3f rotationMatrix) {

	Vector3f v; 
	for (int i = 0; i < numberOfPoints; i++) {
		if (pointList[i] == value) {
			v(0) = points[i][0];
			v(1) = points[i][1];
			v(2) = points[i][2];
			v = (rotationMatrix * v);
			points[i][0] = v(0);
			points[i][1] = v(1);
			points[i][2] = v(2);
		}
	}

}

void PlaneCalculator::rotate(vector<float>& x, vector<float>& y, vector<float>& z, Matrix3f m, Vector3f t) {

	//m = m.inverse();

	Vector3f v;
	for (int i = 0; i < x.size(); i++) {
		v(0) = x.at(i) - t(0);
		v(1) = y.at(i) - t(1);
		v(2) = z.at(i) - t(2);
		v = (m * v);
		x.at(i) = v(0);
		y.at(i) = v(1);
		z.at(i) = v(2);
	}
	//m = m.inverse();

}

void PlaneCalculator::rotateBack(vector<float>& x, vector<float>& y, vector<float>& z, Matrix3f m, Vector3f t) {

	m = m.inverse();

	Vector3f v;
	for (int i = 0; i < x.size(); i++) {
		v(0) = x.at(i);
		v(1) = y.at(i);
		v(2) = z.at(i);
		v = (m * v);
		x.at(i) = v(0) + t(0);
		y.at(i) = v(1) + t(1);
		z.at(i) = v(2) + t(2);
	}
	m = m.inverse();

}

void PlaneCalculator::removeDimension(int dimension, int* pointsUsed, int numberOfPoints) {
	for (int i = 0; i < numberOfPoints; i++) {
		points[pointsUsed[i]][dimension] = 0.0f;
	}
}

void PlaneCalculator::translate(int* pointsUsed, int numberOfPoints, Vector3f translation) {
	if (numberOfPoints > 0) {

		for (int i = numberOfPoints - 1; i >= 0; i--) {
			for (int j = 0; j < 3; j++) {
				points[pointsUsed[i]][j] -= translation(j);
			}
		}
	}
}

void PlaneCalculator::translate(int* pointList, int value, int numberOfPoints, Vector3f translation) {
	if (numberOfPoints > 0) {

		for (int i = numberOfPoints - 1; i >= 0; i--) {
			if (pointList[i] == value) {
				for (int j = 0; j < 3; j++) {
					points[i][j] -= translation(j);
				}
			}
		}
	}
}

void PlaneCalculator::translateBack(int* pointsUsed, int numberOfPoints, Vector3f translation) {
	if (numberOfPoints > 0) {

		for (int i = numberOfPoints - 1; i >= 0; i--) {
			for (int j = 0; j < 3; j++) {
				points[pointsUsed[i]][j] += translation(j);
			}
		}
	}
}

void PlaneCalculator::translateBack(int* pointList, int value, int numberOfPoints, Vector3f translation) {
	if (numberOfPoints > 0) {

		for (int i = numberOfPoints - 1; i >= 0; i--) {
			if (pointList[i] == value) {
				for (int j = 0; j < 3; j++) {
					points[i][j] += translation(j);
				}
			}
		}
	}
}

/*void PlaneCalculator::setNormal(Vector3f norm) {
	normal = norm.normalized();
}*/

void PlaneCalculator::setPoints(float** ps) { //, int* pUsed, int numberOfPointsUsed) {

	/*if (numberOfPointsOnPlane != -1) {
		removeData();
	}
	numberOfPointsOnPlane = numberOfPointsUsed;
	pointsUsed = pUsed;*/
	points = ps;


	/*pointsOnPlane = new float*[numberOfPointsUsed];
	for (int i = 0; i < numberOfPointsUsed; i++) {
		pointsOnPlane[i] = new float[3];
		for (int j = 0; j < 3; j++) {
			pointsOnPlane[i][j] = points[pointsUsed[i]][j];
		}

	}*/

}

/*Matrix3f PlaneCalculator::getRotationMatrix() {
	return rotationMatrix;
}

Vector3f PlaneCalculator::getTranslationVector() {
	return translationVector;
}

void PlaneCalculator::setRotationMatrix(Matrix3f r) {
	rotationMatrix = r;
}

void PlaneCalculator::setTranslationVector(Vector3f v) {
	translationVector = v;
}*/