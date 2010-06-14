#define NOMINMAX

#include "thermalimaging.h"
#include "RandomPointCloud.h"
#include "Ransac.h"
#include "PlaneLimitFinder.h"
#include "ply2opengl.h"
#include <QtGui/QApplication>

#include <Eigen/Eigen>
USING_PART_OF_NAMESPACE_EIGEN

#include "PlaneCalculator.h"

#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ThermalImaging w;
	w.show();
	return a.exec();
}
