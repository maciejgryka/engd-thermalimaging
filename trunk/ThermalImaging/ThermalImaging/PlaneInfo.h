#ifndef PLANEINFO
#define PLANEINFO

#define NOMINMAX

#include <Eigen/Eigen>
USING_PART_OF_NAMESPACE_EIGEN

#include "Quad.h"
#include "Grid.h"
#include <vector>

using namespace std;

class PlaneInfo {

private:
	Vector3f normal;
	
	Vector3f translationVector;
	Matrix3f rotationMatrix;

	float** pointsOnPlane;

	float* color;

	int* pointsUsed;
	int numberOfPoints;

	Quad* quad;
	Grid* grid;

	vector<float> xBorder;
	vector<float> yBorder;
	vector<float> zBorder;

public:
	PlaneInfo();
	~PlaneInfo();

	Vector3f getNormal();
	Vector3f getTranslationVector();
	Matrix3f getRotationMatrix();
	float* getColor();
	int* getPointsUsed();
	int getPointNumber();
	Quad* getQuad();
	Grid* getGrid();
	float** getPointsOnPlane();
	vector<float> getXBorder();
	vector<float> getYBorder();
	vector<float> getZBorder();

	void setNormal(Vector3f);
	void setTranslationVector(Vector3f);
	void setRotationMatrix(Matrix3f);
	void setColor(float*);
	void setPointsUsed(int*);
	void setPointNumber(int);
	void setQuad(Quad*);
	void setGrid(Grid*);
	void setPointsOnPlane(float**);
	void setXBorder(vector<float>);
	void setYBorder(vector<float>);
	void setZBorder(vector<float>);

};

#endif