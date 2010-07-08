#ifndef BUNDLEVIEW
#define BUNDLEVIEW

// View specifies which camera sees a given point, which descriptor it matches and XY coords of the point in this camera image
class BundleView {
public:
	BundleView() {};
	~BundleView() {};

	int getCamera() { return camera; };
	int getKey() { return key; };
	float* getCoordsXY() { return coordsXY; };

	bool setCamera(int cam)
	{
		camera = cam;
		return true;
	};

	bool setKey(int k)
	{
		key = k;
		return true;
	};

	bool setCoordsXY(float x, float y)
	{
		coordsXY[0] = x;
		coordsXY[1] = y;
		return true;
	};

private:
	int camera;
	int key;
	float coordsXY[2];
};

#endif