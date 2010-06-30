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
	//this class contains all the information relevant to a plane
	// it stores all this information together and also allows writing this information to a file or reading it from one

private:
	vector<int> bestPoints;
	bool normalSet;
	Vector3f normal; // the normal of the plane
	bool translationSet;
	Vector3f translationVector; // the translation to the origin
	bool rotationSet;
	Matrix3f rotationMatrix; // the rotation matrix that rotates the plane into the XZ plane

	float** pointsOnPlane; // the xyz coordinates of all the points on the plane

	float* color; // the color of the plane (currently the plane is one color
	float* boundaries;

	int* pointsUsed; // the indices of the points on the plane
	int numberOfPoints; // the number of Points on the plane

	Quad* quad; // the quad, which was used to find the approximate area covered by the plane
	Grid* grid; // the grid obtained from the quad, so that dilation, erosion and border detection can be performed

	vector<float> xBorder; // the x coordinates of the border
	vector<float> yBorder; // the y coordinates of the border
	vector<float> zBorder; // the z coordinates of the border

	vector<vector<float> > corners;

public:
	PlaneInfo();
	~PlaneInfo();

	Vector3f getNormal();
	Vector3f getTranslationVector();
	Matrix3f getRotationMatrix();
	float* getColor();
	float* getBoundaries();
	int* getPointsUsed();
	int getPointNumber();
	Quad* getQuad();
	Grid* getGrid();
	float** getPointsOnPlane();
	vector<float> getXBorder();
	vector<float> getYBorder();
	vector<float> getZBorder();
	vector<vector<float> > getCorners();

	void setNormal(Vector3f);
	void setTranslationVector(Vector3f);
	void setRotationMatrix(Matrix3f);
	void setColor(float*);
	void setBoundaries(float*);
	void setPointsUsed(int*);
	void setPointNumber(int);
	void setQuad(Quad*);
	void setGrid(Grid*);
	void setPointsOnPlane(float**);
	void setXBorder(vector<float>);
	void setYBorder(vector<float>);
	void setZBorder(vector<float>);
	void setCorners(vector<vector<float> >);

	vector<vector<float> > unrotateCorners(vector<vector<float> > corners);

	void writePlane(QString);
	void readPlane(QString);

	vector<int> getBestPoints() { 
		return bestPoints; 
	};


	bool setBestPoints(vector<int> bP)
	{
		bestPoints = bP;
		return true;
	}
};

#endif