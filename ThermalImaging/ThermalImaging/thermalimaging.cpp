#include "thermalimaging.h"

ThermalImaging::ThermalImaging(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

    ui.legendLabel->setPixmap(QPixmap(QString::fromUtf8("Data\\side_building\\time0\\legend.png")));
	connect(ui.thermalVisualSlider,SIGNAL(valueChanged(int)),ui.mainWid,SLOT(thermalVisualPercent(int)));
	connect(ui.legendCheck,SIGNAL(stateChanged(int)),ui.hotLabel,SLOT(changeVisibility(int)));
	connect(ui.legendCheck,SIGNAL(stateChanged(int)),ui.coldLabel,SLOT(changeVisibility(int)));
	connect(ui.legendCheck,SIGNAL(stateChanged(int)),ui.labelWidget,SLOT(changeVisibility(int)));
	connect(ui.legendCheck,SIGNAL(stateChanged(int)),ui.legendLabel,SLOT(changeVisibility(int)));

	//connect(ui.thermalVisualSlider,SIGNAL(sliderMoved(int)),ui.lcdNumber,SLOT(display(int)));

	//connect(ui.clusterSlider,SIGNAL(sliderMoved(int)),ui.mainWid,SLOT(cluster(int)));
	//connect(ui.clusterSlider,SIGNAL(sliderMoved(int)),ui.clusterView,SLOT(display(int)));

	connect(ui.mainWid,SIGNAL(changeLegendImage(int)), ui.legendLabel,SLOT(changePixmap(int)));

	connect(ui.quadSlider,SIGNAL(sliderMoved(int)),ui.mainWid,SLOT(setMaxLevels(int)));
	connect(ui.quadSlider,SIGNAL(sliderMoved(int)),ui.quadView,SLOT(setNum(int)));
	connect(ui.quadButton,SIGNAL(clicked()),ui.mainWid,SLOT(rerunQuad()));

	connect(ui.radio0,SIGNAL(toggled(bool)),ui.mainWid,SLOT(time0(bool)));
	connect(ui.radio1,SIGNAL(toggled(bool)),ui.mainWid,SLOT(time1(bool)));
	connect(ui.radio2,SIGNAL(toggled(bool)),ui.mainWid,SLOT(time2(bool)));

	

	connect(ui.edgeSlider,SIGNAL(sliderMoved(int)),ui.mainWid,SLOT(setEdgeInlier(int)));
	connect(ui.edgeSlider,SIGNAL(sliderMoved(int)),ui.edgeView,SLOT(setNum(int)));

	connect(ui.chillSlider,SIGNAL(sliderMoved(int)),ui.mainWid,SLOT(setChillPoints(int)));
	connect(ui.chillSlider,SIGNAL(sliderMoved(int)),ui.chillView,SLOT(setNum(int)));
	connect(ui.rpeButton,SIGNAL(clicked()),ui.mainWid,SLOT(rerunRPE()));
	//connect(ui.ransacButton,SIGNAL(clicked()),ui.mainWid,SLOT(executeRansac()));
	ui.thermalVisualSlider->show();
	ui.mainWid->show();
	//ui.GLParent->add
}

ThermalImaging::~ThermalImaging()
{

}
