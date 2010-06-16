#include "MyEdit.h"

MyEdit::MyEdit(QWidget* parent) : QLineEdit(parent) {
}

MyEdit::~MyEdit() {}

void MyEdit::buttonClicked() {
	emit pushPoints(text());
}
