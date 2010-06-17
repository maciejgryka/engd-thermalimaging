#include "pointviewer.h"

PointViewer::PointViewer(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	Q_ASSERT(connect(ui.pointButton,SIGNAL(clicked()),ui.pointEdit,SLOT(buttonClicked())));
	Q_ASSERT(connect(ui.pointEdit,SIGNAL(pushPoints(QString)),ui.mainWid,SLOT(newPoints(QString))));
	Q_ASSERT(connect(ui.pointFileEdit,SIGNAL(pushFile(QString)),ui.mainWid,SLOT(newPointsFromFile(QString))));
	Q_ASSERT(connect(ui.findPointFile,SIGNAL(clicked()),ui.mainWid,SLOT(getFile())));
	Q_ASSERT(connect(ui.mainWid,SIGNAL(enterFileLocation(QString)),ui.pointFileEdit,SLOT(setText(QString))));
	Q_ASSERT(connect(ui.pointFileButton,SIGNAL(clicked()),ui.pointFileEdit,SLOT(requestedFile())));
}

PointViewer::~PointViewer()
{

}
