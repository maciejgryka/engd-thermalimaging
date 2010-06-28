#ifndef PLANECALCULATOR
#define PLANECALCULATOR

#include <Eigen/Eigen>
USING_PART_OF_NAMESPACE_EIGEN

#include <QtDebug>

#include "PlaneLimitFinder.h"

class PlaneCalculator {

private:
	//PlaneLimitFinder* plf;
	float** points;
	//int* pointsUsed;
	//int numberOfPointsOnPlane;
	//Matrix3f rotationMatrix;
	//Vector3f translationVector;

	//Vector3f normal;

	void removeData();

public:
	PlaneCalculator();
	~PlaneCalculator();

	void setPoints(float**);
	//void setNormal(Vector3f);

	void makeMatrixAndVector(int*, int, Vector3f, Vector3f&, Matrix3f&);

	//void toOrigin();
	void removeDimension(int, int*, int);

	void rotate(int*, int, Matrix3f);
	void rotateBack(int*, int, Matrix3f);
	void translate(int*, int, Vector3f);
	void translateBack(int*, int, Vector3f);

	void rotate(int*, int, int, Matrix3f);
	void rotateBack(int*, int, int, Matrix3f);
	void translate(int*, int, int, Vector3f);
	void translateBack(int*, int, int, Vector3f);

	/*int* getPointsUsed();
	int getNumberOfPointsOnPlane();

	Matrix3f getRotationMatrix();
	Vector3f getTranslationVector();

	void setRotationMatrix(Matrix3f);
	void setTranslationVector(Vector3f);
	*/
};

#endif