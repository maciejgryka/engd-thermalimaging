#ifndef PLANECALCULATOR
#define PLANECALCULATOR

#include <Eigen/Eigen>
USING_PART_OF_NAMESPACE_EIGEN

#include <QtDebug>

#include "PlaneLimitFinder.h"

class PlaneCalculator {

private:
	//PlaneLimitFinder* plf;
	float** pointsOnPlane;
	int numberOfPointsOnPlane;
	Matrix3f rotationMatrix;
	Vector3f translationVector;

	Vector3f normal;

	void removeData();

public:
	PlaneCalculator();
	~PlaneCalculator();

	void setPoints(float**, int*, int);
	void setNormal(Vector3f);

	void toOrigin();
	void rotate();
	void removeYDimension();

	void rotateBack();
	void translateBack();

	float** getPointsOnPlane();
	int getNumberOfPointsOnPlane();

	Matrix3f getRotationMatrix();
	Vector3f getTranslationVector();

};

#endif