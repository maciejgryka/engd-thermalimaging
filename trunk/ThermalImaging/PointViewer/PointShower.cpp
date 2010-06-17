/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#define NOMINMAX

#include <QtGui>
#include <QtOpenGL>

#include "PointShower.h"
#include <iostream>

using namespace std;

PointShower::PointShower(QWidget *parent, QGLWidget *shareWidget)
    : QGLWidget(parent, shareWidget)
{
    clearColor = Qt::black;
    xRot = 0;
    yRot = 0;
    zRot = 0;

	xTrans = 0.0;
	yTrans = 0.0;
	zTrans = -10.0;
	zoomDist = 0.0;

	setFocusPolicy(Qt::StrongFocus);
}

PointShower::~PointShower()
{
	free(marked);
	marked = NULL;
}

QSize PointShower::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize PointShower::sizeHint() const
{
    return QSize(200, 200);
}

void PointShower::rotateBy(int xAngle, int yAngle, int zAngle)
{
    xRot += xAngle;
    yRot += yAngle;
    zRot += zAngle;
    updateGL();
}

void PointShower::setClearColor(const QColor &color)
{
    clearColor = color;
    updateGL();
}

void PointShower::initializeGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations							// Enable Light One

	ply = new Ply2OpenGL();
	ply->readPlyFile(QFileDialog::getOpenFileName(this,
		tr("Open Ply File"), "C:\\Users\\localadmin\\Desktop", tr("Ply Files (*.ply)")));
	points = ply->toTwoDimensionalArray(ply->getVertices());
	nPoints = ply->getNVertices();
	marked = new bool[nPoints];
	for (int i = 0; i < nPoints; i++) {
		marked[i] = false;
	}
	
}

void PointShower::paintGL()
{

    qglClearColor(clearColor);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glTranslatef(xTrans, yTrans, zTrans-zoomDist);
    glRotatef(xRot / 16.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot / 16.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot / 16.0f, 0.0f, 0.0f, 1.0f);
	glPointSize(1.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < nPoints; i++) {

		if (marked[i]) {
			glEnd();
			glPointSize(5.0f);
			glBegin
				(GL_POINTS);
			glColor3f(1.0f,0.0f,0.0f);
			glVertex3fv(points[i]);
			glEnd();

			glPointSize(1.0f);
			glBegin(GL_POINTS);
		} else {
			glColor3f(1.0f,1.0f,1.0f);
			glVertex3fv(points[i]);
		}
		
	}
	glEnd();

}

void PointShower::resizeGL(int width, int height)
{
	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
	//gluOrtho2D(-10.0f,10.0f,10.0f,-10.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();	

}

void PointShower::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void PointShower::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        rotateBy(8 * dy, 8 * dx, 0);
    } else if (event->buttons() & Qt::RightButton) {
        rotateBy(8 * dy, 0, 8 * dx);
    }
    lastPos = event->pos();
}

void PointShower::mouseReleaseEvent(QMouseEvent * /* event */)
{
    emit clicked();
}

void PointShower::wheelEvent(QWheelEvent *e)
{
	zoomDist -= e->delta()/100;
	updateGL();
}

void PointShower::keyPressEvent(QKeyEvent *e)
{
	float step = 0.3;
  switch( e->key() )
  {
  case Qt::Key_W:
	  yTrans -= step;
	  break;

  case Qt::Key_S:
	  yTrans += step;
	  break;

  case Qt::Key_A:
	  xTrans -= step;
	  break;

  case Qt::Key_D:
	  xTrans += step;
	  break;
  }
  updateGL();
}

void PointShower::newPoints(QString text) {
	for (int i = 0; i < numberList.size(); i++) {
		marked[numberList.at(i)] = false;
	}
	numberList.clear();
	QStringList numberListString = text.split(" ", QString::SkipEmptyParts);
	numberListString.sort();
	for (int i = 0; i < numberListString.size(); i++) {
		numberList.push_back(numberListString.at(i).toInt());
		marked[numberListString.at(i).toInt()] = true;
	}

	updateGL();
}

void PointShower::newPointsFromFile(QString fileName) {
	for (int i = 0; i < numberList.size(); i++) {
		marked[numberList.at(i)] = false;
	}
	numberList.clear();
	QFile file (fileName);
	if (file.exists() && file.open(QIODevice::ReadOnly)) {
		QTextStream stream(&file);
		QString line = stream.readLine();
		QStringList numberListString = line.split(" ", QString::SkipEmptyParts);
		numberListString.sort();
		for (int i = 0; i < numberListString.size(); i++) {
			numberList.push_back(numberListString.at(i).toInt());
			marked[numberListString.at(i).toInt()] = true;
		}

	}

	updateGL();
}

void PointShower::getFile() {
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open Image"),"C:\\Users\\localadmin\\Desktop", tr("Point file (*.txt)"));
	emit enterFileLocation(fileName);
}