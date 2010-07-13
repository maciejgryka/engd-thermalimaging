#ifndef VISIBILITYLABEL
#define VISIBILITYLABEL

#include "QLabel.h"

class VisibilityLabel : public QLabel {
	Q_OBJECT

public:
	VisibilityLabel(QWidget*);
	~VisibilityLabel();

public slots:
	void changeVisibility(int);
	void changePixmap(int);
};

#endif