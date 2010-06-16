#ifndef POINTVIEWER_H
#define POINTVIEWER_H

#include <QtGui/QMainWindow>
#include "ui_pointviewer.h"

class PointViewer : public QMainWindow
{
	Q_OBJECT

public:
	PointViewer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~PointViewer();

private:
	Ui::PointViewerClass ui;
};

#endif // POINTVIEWER_H
