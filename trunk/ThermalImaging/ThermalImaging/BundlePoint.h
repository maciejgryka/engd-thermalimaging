#ifndef BUNDLEPOINT
#define BUNDLEPOINT

#include <vector>
#include "BundleView.h"
#include "Eigen/Core"
USING_PART_OF_NAMESPACE_EIGEN

using namespace std;

class BundlePoint {
public:
	BundlePoint() {};
	~BundlePoint() {};

	Vector3f& getCoords3d() { return coords3d; };
	Vector3f& getColor() { return color; };

	bool setCoords3d(const Vector3f& c3d)
	{
		coords3d = c3d;
		return true;
	}

	bool setColor(const Vector3f& col)
	{
		color = col;
		return true;
	}

	bool addView(const BundleView &view)
	{
		viewList.push_back(view);
		return true;
	}

private:
	Vector3f coords3d;
	Vector3f color;
	vector<BundleView> viewList;
};

#endif