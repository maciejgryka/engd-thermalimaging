#ifndef MYEDIT
#define MYEDIT

#include "QLineEdit.h"

class MyEdit : public QLineEdit {

	Q_OBJECT

public:
	MyEdit(QWidget*);
	~MyEdit();

public slots:
	void buttonClicked();

signals:
	void pushPoints(QString);

};

#endif