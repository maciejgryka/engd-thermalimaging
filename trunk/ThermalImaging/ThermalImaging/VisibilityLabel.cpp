#include "VisibilityLabel.h"

VisibilityLabel::VisibilityLabel(QWidget* parent) : QLabel(parent) {
}

VisibilityLabel::~VisibilityLabel() {}

void VisibilityLabel::changeVisibility(int state) {
	if (state == Qt::Checked)
		show();
	else
		hide();
}

void VisibilityLabel::changePixmap(int time) {
	setPixmap(QPixmap(QString::fromUtf8("Data\\side_building\\time%1\\legend.png").arg(time)));
}