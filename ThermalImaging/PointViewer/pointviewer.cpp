#include "pointviewer.h"

PointViewer::PointViewer(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	Q_ASSERT(connect(ui.showButton,SIGNAL(clicked()),ui.pointShow,SLOT(buttonClicked())));
	connect(ui.pointShow,SIGNAL(pushPoints(QString)),ui.mainWid,SLOT(newPoints(QString)));
}

PointViewer::~PointViewer()
{

}
