#include "PlaneCalculator.h"

PlaneCalculator::PlaneCalculator() {
	//plf = new PlaneLimitFinder();
	numberOfPointsOnPlane = -1;
}

PlaneCalculator::~PlaneCalculator() {
	//free(plf);
	removeData();
}

float** PlaneCalculator::getPointsOnPlane() {
	return pointsOnPlane;
}

int PlaneCalculator::getNumberOfPointsOnPlane() {
	return numberOfPointsOnPlane;
}

void PlaneCalculator::removeData() {
	for (int i = 0; i < numberOfPointsOnPlane; i++) {
		free(pointsOnPlane[i]);
	}
	free(pointsOnPlane);
}

void PlaneCalculator::rotate() {
	Vector3f yAxis (0.0f,1.0f,0.0f);

	float angle = acos(yAxis.dot(normal));
	Vector3f axis = yAxis.cross(normal);

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
	MatrixXf matrix(3,3);

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

	matrix(0,0) = xx*2 + s;
	matrix(1,0) = (xy + wz)*2;
	matrix(2,0) = (xz - wy)*2;
	//matrix[3][0] = 0;

	matrix(0,1) = (xy - wz)*2;
	matrix(1,1) = yy*2 + s;
	matrix(2,1) = (yz + wx)*2;
	//matrix[3][1] = 0;

	matrix(0,2) = (xz + wy)*2;
	matrix(1,2) = (yz - wx)*2;
	matrix(2,2) = zz*2 + s;

	//qDebug() << matrix(0,0) << matrix(0,1) << matrix(0,2) << matrix(1,0) << matrix(1,1) << matrix(1,2) << matrix(2,0) << matrix(2,1) << matrix(2,2);

	Vector3f v; 
	for (int i = 0; i < numberOfPointsOnPlane; i++) {
		v(0) = pointsOnPlane[i][0];
		v(1) = pointsOnPlane[i][1];
		v(2) = pointsOnPlane[i][2];
		v = (v.transpose() * matrix).transpose();
		pointsOnPlane[i][0] = v(0);
		pointsOnPlane[i][1] = v(1);
		pointsOnPlane[i][2] = v(2);
	}


}

void PlaneCalculator::removeYDimension() {
	for (int i = 0; i < numberOfPointsOnPlane; i++) {
		pointsOnPlane[i][1] = 0;
	}
}

void PlaneCalculator::toOrigin() {
	if (numberOfPointsOnPlane > 0) {
		for (int i = numberOfPointsOnPlane - 1; i >= 0; i--) {
			for (int j = 0; j < 3; j++) {
				pointsOnPlane[i][j] -= pointsOnPlane[0][j];
			}
		}
	}
}

void PlaneCalculator::setNormal(Vector3f norm) {
	normal = norm.normalized();
}

void PlaneCalculator::setPoints(float** points, int* pointsUsed, int numberOfPointsUsed) {

	if (numberOfPointsOnPlane != -1) {
		removeData();
	}
	numberOfPointsOnPlane = numberOfPointsUsed;

	pointsOnPlane = new float*[numberOfPointsUsed];
	for (int i = 0; i < numberOfPointsUsed; i++) {
		pointsOnPlane[i] = new float[3];
		for (int j = 0; j < 3; j++) {
			pointsOnPlane[i][j] = points[pointsUsed[i]][j];
		}

	}
}