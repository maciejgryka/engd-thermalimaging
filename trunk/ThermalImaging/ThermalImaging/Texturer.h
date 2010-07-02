#ifndef TEXTURER
#define TEXTURER

#include <QImage>
#include "BundleParser.h"
#include "Eigen/Core"
USING_PART_OF_NAMESPACE_EIGEN

using namespace std;

class Texturer {
	
public:
	Texturer() {};
	~Texturer() {};
	
	bool readFile(QString filename)
	{
		return bp.readFile(filename);
	}

	/// Takes plane normal, position and 3D corner corrdinates and returns a rectangular texture from the best available image
	QImage getTexture(Vector3f planeNormal, Vector3f planeTranslation, const vector<vector<float> > &corners);

private:
	BundleParser bp;

	int findBestCamera(Vector3f planeTranslation, Vector3f planeNormal, const vector<vector<float> > &corners);
};

#endif