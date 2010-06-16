#ifndef BUNDLE_PARSER
#define BUNDEL_PARSER

#include <iostream>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include "Eigen/Core"
USING_PART_OF_NAMESPACE_EIGEN

#include "BundleCamera.h"

class BundleParser {

public:
	BundleParser() {};
	~BundleParser() {};

	// returns X and Y coords of 5 points in the specified camera image given X, Y a Z coords of 5 points in space
	MatrixXf getCameraXYPoints(int cameraNo, MatrixXf points3d);
	bool readFile(QString filename);

private:
	int nCameras;
	int nPoints;
	QString listFile;
	QString bundleFile;
	BundleCamera *cameras;

	QString getL(QTextStream &stream);
};

#endif