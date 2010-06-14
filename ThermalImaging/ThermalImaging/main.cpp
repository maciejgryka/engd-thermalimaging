#include "thermalimaging.h"
#include <QtGui/QApplication>
//#include "WarpImage.h"
//#include "Eigen/Core"

//USING_PART_OF_NAMESPACE_EIGEN

int main(int argc, char *argv[])
{
	//WarpImage warper;
	//QImage im;

	//if (im.load("images//wilkins_hom_2.png"))
	//{
	//	QImage newIm(im.size(), im.format());
	//	newIm.fill(0);

	//	MatrixXf points1(2,5);
	//	MatrixXf points2(2,5);

	//	points1.fill(0.0f);
	//	points2.fill(0.0f);
	//	
	//	points1.col(0) <<  25, 206;
	//	points1.col(1) << 132, 150;
	//	points1.col(2) << 161, 229;
	//	points1.col(3) << 333, 252;
	//	points1.col(4) << 328,  15;

	//	points2.col(0) << 185, 201;
	//	points2.col(1) << 291, 144;
	//	points2.col(2) << 320, 224;
	//	points2.col(3) << 491, 246;
	//	points2.col(4) << 486,   8;

	//	newIm = warper.warp(im, points1, points2);
	//	newIm.save("images//wilkins_hom_2_warped.png");
	//}

	QApplication a(argc, argv);
	ThermalImaging w;
	w.show();
	return a.exec();
}
