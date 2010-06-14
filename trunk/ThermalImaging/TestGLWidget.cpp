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

#include "TestGLWidget.h"
#include <iostream>

using namespace std;

TestGLWidget::TestGLWidget(QWidget *parent, QGLWidget *shareWidget)
    : QGLWidget(parent, shareWidget)
{
    clearColor = Qt::black;
    xRot = 0;
    yRot = 0;
    zRot = 0;
}

TestGLWidget::~TestGLWidget()
{
}

QSize TestGLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize TestGLWidget::sizeHint() const
{
    return QSize(200, 200);
}

void TestGLWidget::rotateBy(int xAngle, int yAngle, int zAngle)
{
    xRot += xAngle;
    yRot += yAngle;
    zRot += zAngle;
    updateGL();
}

void TestGLWidget::setClearColor(const QColor &color)
{
    clearColor = color;
    updateGL();
}

void TestGLWidget::initializeGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations							// Enable Light One

	PlaneCalculator* p = new PlaneCalculator();

	int ps = 1000;
	int o = 0;
	RandomPointCloud* rpc = new RandomPointCloud();
	rpc->makePointCloud(1,ps,o);
	float** normal = rpc->getNormals();
	

	int *a = new int[ps];
	for (int i = 0; i < ps; i++) {
		a[i] = i;
	}
			
	p->setPoints(rpc->getPointCloud(),a,ps);
	p->setNormal(Vector3f(normal[0][0],normal[0][1],normal[0][2]));
	p->toOrigin();
	p->rotate();
	p->removeYDimension();
	pop = p->getPointsOnPlane();
	npop = p->getNumberOfPointsOnPlane();

	PlaneLimitFinder *plf = new PlaneLimitFinder();
	
	plf->setPoints(pop, npop);
	plf->findLimits(0);
	clusters = plf->getClusters();
	nclusters = plf->getNumberOfClusters();

	colors = new float*[nclusters];
	for (int i = 0; i < nclusters; i++) {
		colors[i] = new float[3];
		colors[i][0] = (float) rand() / (float) RAND_MAX;
		colors[i][1] = (float) rand() / (float) RAND_MAX;
		colors[i][2] = (float) rand() / (float) RAND_MAX;
	}

	float minX = +1000.0f;
	float maxX = -1000.0f;
	float minZ = +1000.0f;
	float maxZ = -1000.0f;
	float x, z;

	//pop = rpc->getPointCloud();

	for (int i = 0; i < ps; i++) {
		x = pop[i][0];
		z = pop[i][2];
		if (x < minX)
			minX = x;
		if (x > maxX)
			maxX = x;
		if (z < minZ)
			minZ = z;
		if (z > maxZ)
			maxZ = z;
	}
	//qDebug() << minX << maxZ << maxX << minZ;
	float* boundaries = new float[4];
	boundaries[0] = minX;
	boundaries[1] = maxZ;
	boundaries[2] = maxX;
	boundaries[3] = minZ;


	q = new Quad(0,a,ps,pop,boundaries);
	q->subdivide();

	int** g = q->toGrid();
	Grid* grid = new Grid();
	grid->setGrid(g,q->getSize());

	
}

void TestGLWidget::paintGL()
{

    qglClearColor(clearColor);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -10.0f);
    glRotatef(xRot / 16.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot / 16.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot / 16.0f, 0.0f, 0.0f, 1.0f);

	
	q->drawQuad();
	glPointSize(10.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < npop; i++) {
		glColor3fv(colors[clusters[i]-1]);
		glVertex3fv(pop[i]);
	}
	glEnd();


}

void TestGLWidget::resizeGL(int width, int height)
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

void TestGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void TestGLWidget::mouseMoveEvent(QMouseEvent *event)
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

void TestGLWidget::mouseReleaseEvent(QMouseEvent * /* event */)
{
    emit clicked();
}

void TestGLWidget::keyPressEvent( QKeyEvent *e )
{
  updateGL();
}
