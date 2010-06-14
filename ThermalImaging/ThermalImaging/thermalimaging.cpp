#include "thermalimaging.h"

ThermalImaging::ThermalImaging(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	//connect(ui.thermalVisualSlider,SIGNAL(sliderMoved(int)),ui.mainWid,SLOT(thermalVisualPercent(int)));
	connect(ui.thermalVisualSlider,SIGNAL(sliderMoved(int)),ui.lcdNumber,SLOT(display(int)));
	//connect(ui.ransacButton,SIGNAL(clicked()),ui.mainWid,SLOT(executeRansac()));
	ui.thermalVisualSlider->show();
	ui.mainWid->show();
	//ui.GLParent->add
}

ThermalImaging::~ThermalImaging()
{

}
