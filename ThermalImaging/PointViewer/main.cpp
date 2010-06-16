#include "pointviewer.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	PointViewer w;
	w.show();
	return a.exec();
}
