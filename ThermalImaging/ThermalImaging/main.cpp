#define NOMINMAX

#include "thermalimaging.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ThermalImaging w;
	w.show();
	return a.exec();
}
