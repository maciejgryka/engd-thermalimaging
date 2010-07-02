#ifndef BUNDLEVIEW
#define BUNDLEVIEW

class BundleView {
public:
	BundleView() {};
	~BundleView() {};

	int getCamera() { return camera; }
	int getKey() { return key; }
	float* getCoordsXY() { return coordsXY; };

	bool setCamera(int cam)
	{
		if (camera = cam)
			return true;
		else
			return false;
	}

	bool setKey(int k)
	{
		if (key = k)
			return true;
		else
			return false;
	}

	bool setCoordsXY(float x, float y)
	{
		if ((coordsXY[0] = x) && (coordsXY[1] = y))
			return true;
		else
			return false;
	}

private:
	int camera;
	int key;
	float coordsXY[2];
};

#endif