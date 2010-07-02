#ifndef BUNDLECAMERA
#define BUNDELCAMERA

#include "Eigen/Core"
USING_PART_OF_NAMESPACE_EIGEN

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