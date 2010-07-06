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
