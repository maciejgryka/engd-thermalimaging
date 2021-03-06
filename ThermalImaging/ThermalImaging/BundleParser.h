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
#include "BundlePoint.h"

// Reads a bundle file and stores whatever it finds (i.e. vertices and cameras)
class BundleParser {

public:
	BundleParser() {};
	~BundleParser() {};

	// returns X and Y coords of points in the specified camera image given X, Y a Z coords of points in space
	MatrixXf getCameraXYPoints(int cameraNo, MatrixXf points3d);
	
	MatrixXf getCameraXYPoints(BundleCamera camera, MatrixXf points3d);
	
	bool readFile(QString filename);
	QStringList readImageList(QString filename);
	QStringList readImageList(QString filename, bool lower);
	
	BundleCamera& getCamera(int cameraIndex)
	{
		return cameras[cameraIndex];
	};

	int getNCameras() { return nCameras; };
	int getNPoints() { return nPoints; };

	BundlePoint getPoint(int pointIndex) { return points[pointIndex]; };

private:
	int nCameras;
	int nPoints;
	QString listFile;
	QString bundleFile;
	BundleCamera *cameras;
	BundlePoint *points;

	QString getL(QTextStream &stream);
	float undistort(Vector2f p, float k1, float k2);
};

#endif