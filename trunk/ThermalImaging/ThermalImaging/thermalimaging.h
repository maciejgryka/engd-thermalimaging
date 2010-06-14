#ifndef THERMALIMAGING_H
#define THERMALIMAGING_H

#define NOMINMAX

#include <QtGui/QMainWindow>
#include "ui_thermalimaging.h"
#include "TestGLWidget.h"

class ThermalImaging : public QMainWindow
{
	Q_OBJECT

public:
	ThermalImaging(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ThermalImaging();

private:
	Ui::ThermalImagingClass ui;
};

#endif // THERMALIMAGING_H
