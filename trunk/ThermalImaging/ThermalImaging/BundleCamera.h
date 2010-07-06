#ifndef BUNDLECAMERA
#define BUNDELCAMERA

#include "Eigen/Core"
USING_PART_OF_NAMESPACE_EIGEN

// Stores camera parameters read from bundle file
// all the stuff made public because I was lazy...
class BundleCamera {

public:
	BundleCamera() {};
	~BundleCamera() {};

	float focal;
	float radialDist[2];
	Matrix3f R;
	Vector3f t;
};

#endif