#ifndef PLANESNAPPING
#define PLANESNAPPING

#define NOMINMAX

#include <QTDebug>

#include <Eigen/Eigen>
USING_PART_OF_NAMESPACE_EIGEN

#include "PlaneInfo.h"
#include <vector>

class PlaneSnapping {

private:
	vector<PlaneInfo*> planes; // contains all the planes
	//MatrixXf** edges;
	
	int findIntersectionLine(int, int); // find the intersection line

public:
	PlaneSnapping();
	~PlaneSnapping();
	void addPlane(PlaneInfo*); // add a plane to the list of planes to consider
	void snap(); // perform snapping
};

#endif