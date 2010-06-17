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
	noPlanes = 0;

	xTrans = 0.0;
	yTrans = 0.0;
	zTrans = -10.0;
	zoomDist = 0.0;

	setFocusPolicy(Qt::StrongFocus);
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

	noPlanes = 1;
	ps = 1000;
	o = 0;
	rpc = new RandomPointCloud();
	rpc->makePointCloud(1,ps,o);
	//float** normal = rpc->getNormals();

	Ransac* r = new Ransac();
	r->setInlierDistance(0.2f);
	r->setIterations(1000);
	r->setPoints(rpc->getPointCloud(),noPlanes*ps+o);
	int numberOfPointsOnBestPlane = 0;
	Vector3f norm;
	Vector3f ori;
	int* bestPoints = new int[3];
	q = new Quad*[noPlanes];
	grid = new Grid*[noPlanes];
	planes = new PlaneInfo*[noPlanes];
	for (int k = 0; k < noPlanes; k++) {
		planes[k] = new PlaneInfo();
		int *a = r->findBestPlane(k+1,numberOfPointsOnBestPlane,ori,norm,bestPoints);
		planes[k]->setNormal(norm);
		/*int *a = new int[ps];
		for (int i = 0; i < ps; i++) {
			a[i] = i;
		}*/
		int* b = r->convertToNumberList(a,noPlanes*ps+o,numberOfPointsOnBestPlane, k+1);
		float* cols = new float[3];
		for (int i = 0; i < 3; i++) {
			cols[i] = (float) rand() / (float) RAND_MAX;
		}
		planes[k]->setColor(cols);
				
		planes[k]->setPointsUsed(b);
		p->setPoints(rpc->getPointCloud(),b,numberOfPointsOnBestPlane);
		p->setNormal(norm);
		p->toOrigin();
		p->rotate();
		p->removeYDimension();
		pop = p->getPointsOnPlane();
		npop = p->getNumberOfPointsOnPlane();
		planes[k]->setPointsOnPlane(pop);
		planes[k]->setPointNumber(npop);
		planes[k]->setRotationMatrix(p->getRotationMatrix());
		planes[k]->setTranslationVector(p->getTranslationVector());

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

		for (int i = 0; i < npop; i++) {
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


		q[k] = new Quad(0,b,numberOfPointsOnBestPlane,pop,boundaries);
		q[k]->subdivide();
		planes[k]->setQuad(q[k]);

		int** g = q[k]->toGrid();
		grid[k] = new Grid();
		grid[k]->setGrid(g,q[k]->getSize());
		grid[k]->setBoundaries(boundaries);
		grid[k]->dilateAndErode(3);
		grid[k]->erodeAndDilate(1);
		grid[k]->calculateBorder();
		grid[k]->drawAsPolygon();
		//grid[k]->unrotateBorder(planes[k]->getRotationMatrix(),planes[k]->getTranslationVector());

		planes[k]->setGrid(grid[k]);
		planes[k]->setXBorder(grid[k]->getXBorder());
		planes[k]->setYBorder(grid[k]->getYBorder());
		planes[k]->setZBorder(grid[k]->getZBorder());

	}
	
}

void TestGLWidget::paintGL()
{

    qglClearColor(clearColor);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glTranslatef(xTrans, yTrans, zTrans-zoomDist);
    glRotatef(xRot / 16.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot / 16.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot / 16.0f, 0.0f, 0.0f, 1.0f);

	
	for (int k = 0; k < noPlanes; k++) {
		q[k]->drawQuad();
		planes[k]->getGrid()->drawGrid();
		vector<float> xb = planes[k]->getXBorder();
		vector<float> yb = planes[k]->getYBorder();
		vector<float> zb = planes[k]->getZBorder();
		glBegin(GL_POLYGON);
		glColor3fv(planes[k]->getColor());
		for (int i = 0; i < xb.size(); i++) {
			glVertex3f(xb.at(i),yb.at(i)+0.1f,zb.at(i));
		}
		glEnd();

		glPointSize(5.0f);
		glBegin(GL_POINTS);
		for (int i = 0; i < planes[k]->getPointNumber(); i++) {
			glColor3f(0.0f,0.0f,1.0f);
			glVertex3fv(planes[k]->getPointsOnPlane()[i]);
		}
		glEnd();
	}


	glPointSize(5.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < noPlanes * ps + o; i++) {
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3fv(rpc->getPointCloud()[i]);
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
    } else if (event->buttons() & Qt::MidButton) {
        xTrans += dx/60.0;
		yTrans -= dy/60.0;
		updateGL();
    }
    lastPos = event->pos();
}

void TestGLWidget::mouseReleaseEvent(QMouseEvent * /* event */)
{
    emit clicked();
}

void TestGLWidget::wheelEvent(QWheelEvent *e)
{
	zoomDist -= e->delta()/100;
	updateGL();
}

void TestGLWidget::keyPressEvent(QKeyEvent *e)
{
	float step = 0.3;
  /*switch( e->key() )
  {
  case Qt::Key_W:
	  yTrans += step;
	  break;

  case Qt::Key_S:
	  yTrans -= step;
	  break;

  case Qt::Key_A:
	  xTrans -= step;
	  break;

  case Qt::Key_D:
	  xTrans += step;
	  break;
  }*/
  updateGL();
}