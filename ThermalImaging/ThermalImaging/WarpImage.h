#ifndef WARPIMAGE
#define WARPIMAGE

#include <math.h>
#include <QImage>
#include "Eigen/Core"
#include "Eigen/SVD"

USING_PART_OF_NAMESPACE_EIGEN

class WarpImage {

public:
	WarpImage(){};
	~WarpImage(){};
	QImage warpIntoTexture(QImage &image, MatrixXf points1, int texSizeX, int texSizeY);
	QImage warp(QImage &image, MatrixXf points1, MatrixXf points2, int outSizeX, int outSizeY);

private:
	Matrix3f homography;
	Matrix3f* calculateHomography(MatrixXf points1, MatrixXf points2);
	Matrix3f* solveForHomography(MatrixXf A);
};

#endif