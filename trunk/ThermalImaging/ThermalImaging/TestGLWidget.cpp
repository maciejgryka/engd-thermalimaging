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
	xTrans = 0.0;
	yTrans = 0.0;
	zTrans = -10.0;
	zoomDist = 0.0;
    noPlanes = 0;

    xTrans = 0.0;
    yTrans = 0.0;
    zTrans = -10.0;
    zoomDist = 0.0;

    maxLevels = 6;
    edgeInlierDistance = 0.1f;
    chillPoints = 0.1f;

    setFocusPolicy(Qt::StrongFocus);
}

TestGLWidget::~TestGLWidget()
{
	delete[] readPlane;
	readPlane = NULL;
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);                           // Black Background
    glClearDepth(1.0f);                                                                     // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                                                        // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                                                         // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);      // Really Nice Perspective Calculations                                                 // Enable Light One

    ply = new Ply2OpenGL();
    ply->readPlyFile("Data\\union3.ply");
    //ply->readPlyFile("C:\\Users\\localadmin\\Desktop\\union.ply");
    noPlanes = 2;
    ps = 1000;
    o = 0;
    
    rpc = new RandomPointCloud();
    rpc->makePointCloud(noPlanes,ps,o);
    //points = rpc->getPointCloud();
	points = ply->toTwoDimensionalArray(ply->getVertices());

    float** normal = rpc->getNormals();

    //nPoints = noPlanes*ps+o;
	nPoints = ply->getNVertices();
    //nPoints = ply->getNVertices();
    //points = ply->toTwoDimensionalArray(ply->getVertices());

	Ransac* r = new Ransac();
	r->setInlierDistance(0.1f);
	r->setIterations(10000);
	//points = ply->toTwoDimensionalArray(ply->getVertices());
	r->setPoints(points,nPoints);
	int numberOfPointsOnBestPlane = 0;
	Vector3f norm;
	Vector3f ori;
	int* bestPoints = new int[3];
	planes = new PlaneInfo*[noPlanes];
	readPlane = new bool[noPlanes];
	for (int k = 0; k < noPlanes; k++) {
		planes[k] = NULL;
		readPlane[k] = false;
 		QFile file (QString("Data\\plane%1.txt").arg(k+1));
		if (file.exists() && file.open(QIODevice::ReadOnly)) {
			planes[k] = new PlaneInfo();
			planes[k]->readPlane(QString("Data\\plane%1.txt").arg(k+1));
			readPlane[k] = true;
		}
	}
	pointList = new int[nPoints];
	for (int i = 0; i < nPoints; i++) {
		pointList[i] = 0;
	}
	for (int k = 0; k < noPlanes; k++) {
		int* b;
		if (!readPlane[k]) {
			
			planes[k] = new PlaneInfo();
			r->findBestPlane(k+1,numberOfPointsOnBestPlane,ori,norm,bestPoints, pointList);
			planes[k]->setNormal(norm);

			/*for (int i = 0; i < nPoints / 10 - 1; i++) {
				qDebug() << pointList[i * 10 + 0]  << pointList[i * 10 + 1]  << pointList[i * 10 + 2]  << pointList[i * 10 + 3]  << pointList[i * 10 + 4]
				 << pointList[i * 10 + 5]  << pointList[i * 10 + 6]  << pointList[i * 10 + 7]  << pointList[i * 10 + 8]  << pointList[i * 10 + 9];	
			}*/

			/*int *a = new int[ps];
			for (int i = 0; i < ps; i++) {
				a[i] = i;
			}*/
			b = r->convertToNumberList(pointList,nPoints,numberOfPointsOnBestPlane, k+1);
			//r->printToFile(QString("C:\\Users\\localadmin\\Desktop\\data%1.txt").arg(k),b,numberOfPointsOnBestPlane);

            float* cols = new float[3];
            for (int i = 0; i < 3; i++) {
                    cols[i] = (float) rand() / (float) RAND_MAX;
            }
            planes[k]->setColor(cols);
                            
            p.setPoints(points);
            //p->setNormal(norm);
            Vector3f v;
            Matrix3f r;
            p.makeMatrixAndVector(b, numberOfPointsOnBestPlane, norm, v , r);
            
            p.translate(b, numberOfPointsOnBestPlane, v);
            p.rotate(b, numberOfPointsOnBestPlane, r);
            p.removeDimension(1, b, numberOfPointsOnBestPlane);
            
            //p->translate(pointList, 0, nPoints, v);
            //p->rotate(pointList, 0, nPoints, r);
            //p->translate(pointList,1, nPoints, v);
            //p->rotate(pointList, 1, nPoints, r);
            
            planes[k]->setPointsUsed(b);
            planes[k]->setPointNumber(numberOfPointsOnBestPlane);
            planes[k]->setRotationMatrix(r);
            planes[k]->setTranslationVector(v);

        //if (planes[k]->getXBorder().size() == 0) {
            PlaneLimitFinder *plf = new PlaneLimitFinder();
            
            plf->setPoints(points);
            plf->setPointsUsed(planes[k]->getPointsUsed(), numberOfPointsOnBestPlane);
            plf->findLimits(0);
            clusters = plf->getClusters();
            nclusters = plf->getNumberOfClusters();
            b = plf->findBiggestCluster(npop, pointList, "C:\\Users\\localadmin\\Desktop\\bigcluster.txt");
            planes[k]->setPointsUsed(b);
            planes[k]->setPointNumber(plf->getNumberOfPoints());
            p.rotateBack(pointList, -1, nPoints, planes[k]->getRotationMatrix());
            p.translateBack(pointList, -1, nPoints, planes[k]->getTranslationVector());
            for (int i = 0; i < nPoints; i++) {
                if (pointList[i] == -1)
                    pointList[i] = 0;
            }

            float minX = +1000.0f;
            float maxX = -1000.0f;
            float minZ = +1000.0f;
            float maxZ = -1000.0f;
            float x, z;

            //pop = rpc->getPointCloud();

            for (int i = 0; i < planes[k]->getPointNumber(); i++) {
                x = points[b[i]][0];
                z = points[b[i]][2];
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

			planes[k]->setBoundaries(boundaries);

            p.rotateBack(planes[k]->getPointsUsed(), planes[k]->getPointNumber(),planes[k]->getRotationMatrix());
            p.translateBack(planes[k]->getPointsUsed(), planes[k]->getPointNumber(),planes[k]->getTranslationVector());
            


        //}
                
        }
	}
    runQuad();
        
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
        //q[k]->drawQuad();
        //planes[k]->getGrid()->drawGrid();
		
		//TODO: memory leak?
		/*Grid* unrotatedGrid = new Grid;
		
		*unrotatedGrid = *planes[k]->getGrid();
		unrotatedGrid->unrotateBorder(planes[k]->getRotationMatrix(), planes[k]->getTranslationVector());
		vector<float> xb = unrotatedGrid->getXBorder();
        vector<float> yb = unrotatedGrid->getYBorder();
        vector<float> zb = unrotatedGrid->getZBorder();
        glPointSize(2.0f);

		// Draw border
		glBegin(GL_POINTS);
        glColor3fv(planes[k]->getColor());
        for (int i = 0; i < xb.size(); i++) {
                glVertex3f(xb.at(i),yb.at(i),zb.at(i));
        }
        glEnd();*/

  //      // draw point cloud
		//glPointSize(2.0f);
  //      glBegin(GL_POINTS);
  //      int* pU = planes[k]->getPointsUsed();
  //      for (int i = 0; i < planes[k]->getPointNumber(); i++) {
  //              
  //              glColor3f(0.0f,0.0f,1.0f);
  //              glVertex3fv(points[pU[i]]);
  //      }
  //      glEnd();

		// draw planes
        glColor3fv(planes[k]->getColor());
        glBegin(GL_POLYGON);
		vector<vector<float> > corners = planes[k]->getCorners();
        for (int corn(0); corn < planes[k]->getCorners().size(); corn++)
        {
                glVertex3f(corners.at(corn).at(0), corners.at(corn).at(1), corners.at(corn).at(2));
        }
        glEnd();

		// draw corners
        glPointSize(5.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_POINTS);
        for (int corn(0); corn < planes[k]->getCorners().size(); corn++)
        {
                glVertex3f(corners.at(corn).at(0), corners.at(corn).at(1), corners.at(corn).at(2));
        }
        glEnd();

		// draw bestPoints
        glPointSize(5.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_POINTS);
		vector<float> xb = planes[k]->getXBorder();
		vector<float> yb = planes[k]->getYBorder();
		vector<float> zb = planes[k]->getZBorder();
        for (int corn(0); corn < planes[k]->getBestPoints().size(); corn++)
        {
                glVertex3f(xb.at(planes[k]->getBestPoints().at(corn)), yb.at(planes[k]->getBestPoints().at(corn)), zb.at(planes[k]->getBestPoints().at(corn)));
        }
        glEnd();

		// draw axes
        glLineWidth(1.0f);
        glBegin(GL_LINES);
                // X axis
                glColor3f(1.0f, 0.0f, 0.0f);
                glVertex3f(-100.0f, 0.0f, 0.0f);
                glVertex3f(100.0f, 0.0f, 0.0f);
                // Y axis
                glColor3f(0.0f, 1.0f, 0.0f);
                glVertex3f(0.0f, -100.0f, 0.0f);
                glVertex3f(0.0f, 100.0f, 0.0f);
                // Z axis
                glColor3f(0.0f, 0.0f, 1.0f);
                glVertex3f(0.0f, 0.0f, -100.0f);
                glVertex3f(0.0f, 0.0f, 100.0f);
        glEnd();
	}

	glPointSize(1.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < nPoints; i++) {
			if (pointList[i] == 0)
					glColor3f(1.0f,1.0f,1.0f);
			//if (pointList[i] >= 1 && pointList[i] <= noPlanes)
			//      glColor3f(0.0f,1.0f,0.0f);
			//if (pointList[i] == -1)
			//      glColor3f(0.0f,1.0f,1.0f);
			glVertex3fv(points[i]);
	}
	glEnd();
}

void TestGLWidget::resizeGL(int width, int height)
{
        glViewport(0,0,width,height);                                           // Reset The Current Viewport

        glMatrixMode(GL_PROJECTION);                                            // Select The Projection Matrix
        glLoadIdentity();                                                                       // Reset The Projection Matrix

        // Calculate The Aspect Ratio Of The Window
        gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,1000.0f);
        //gluOrtho2D(-10.0f,10.0f,10.0f,-10.0f);

        glMatrixMode(GL_MODELVIEW);                                                     // Select The Modelview Matrix
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
        zoomDist -= (float) e->delta()/100.0f;
        updateGL();
}

void TestGLWidget::keyPressEvent(QKeyEvent *e)
{
	float step = 0.3;
	updateGL();
}

void TestGLWidget::setMaxLevels(int lvls) {
        maxLevels = lvls;
}

void TestGLWidget::setEdgeInlier(int edgeInliers) {
        edgeInlierDistance = (float) edgeInliers / 100.0f;
}

void TestGLWidget::setChillPoints(int chillPts) {
        chillPoints = (float) chillPts / 100.0f;
}

void TestGLWidget::rerunQuad() {
        runQuad();
}

void TestGLWidget::runQuad() {
        for (int k = 0; k < noPlanes; k++) {
				if (readPlane[k])
					continue;
                p.translate(planes[k]->getPointsUsed(), planes[k]->getPointNumber(), planes[k]->getTranslationVector());
                p.rotate(planes[k]->getPointsUsed(), planes[k]->getPointNumber(), planes[k]->getRotationMatrix());
                p.removeDimension(1, planes[k]->getPointsUsed(), planes[k]->getPointNumber());

                Quad* q = planes[k]->getQuad();
                Grid* grid = planes[k]->getGrid();
                if (q != NULL) {
                        delete q;
                        delete grid;
                }

                q = new Quad(0, maxLevels, planes[k]->getPointsUsed(), planes[k]->getPointNumber(),points, planes[k]->getBoundaries());
                q->subdivide();
                planes[k]->setQuad(q);

                int** g = q->toGrid();
                grid = new Grid();
                grid->setGrid(g,q->getSize());
                grid->setBoundaries(planes[k]->getBoundaries());
                grid->dilateAndErode(3);
                grid->erodeAndDilate(1);
                grid->calculateBorder();
                grid->drawAsPolygon();
				//grid->unrotateBorder(planes[k]->getRotationMatrix(),planes[k]->getTranslationVector());
                planes[k]->setGrid(grid);
                planes[k]->setXBorder(grid->getXBorder());
                planes[k]->setYBorder(grid->getYBorder());
                planes[k]->setZBorder(grid->getZBorder());

				p.rotateBack(planes[k]->getXBorder(),planes[k]->getYBorder(),planes[k]->getZBorder(),planes[k]->getRotationMatrix(), planes[k]->getTranslationVector());

                p.rotateBack(planes[k]->getPointsUsed(), planes[k]->getPointNumber(),planes[k]->getRotationMatrix());
                p.translateBack(planes[k]->getPointsUsed(), planes[k]->getPointNumber(),planes[k]->getTranslationVector());
        }

        runRPE();
}

void TestGLWidget::rerunRPE() {
        runRPE();
}

void TestGLWidget::runRPE() {
    Grid* grid;

    for (int k = 0; k < noPlanes; k++) {
			
		if (readPlane[k]) {
			readPlane[k] = false;
			continue;
		}

        p.translate(planes[k]->getPointsUsed(), planes[k]->getPointNumber(), planes[k]->getTranslationVector());
        p.rotate(planes[k]->getPointsUsed(), planes[k]->getPointNumber(), planes[k]->getRotationMatrix());
        p.removeDimension(1, planes[k]->getPointsUsed(), planes[k]->getPointNumber());

        

        rpe.reset();
        rpe.setInlierDistance(edgeInlierDistance);
        rpe.setIterations(1000);
        rpe.setPercentOfChillPoints(chillPoints);
		p.rotate(planes[k]->getXBorder(),planes[k]->getYBorder(),planes[k]->getZBorder(),planes[k]->getRotationMatrix(), planes[k]->getTranslationVector());
        rpe.setXYZBorders(planes[k]->getXBorder(),planes[k]->getYBorder(),planes[k]->getZBorder());
        rpe.setBoundaries(planes[k]->getBoundaries());
        rpe.findEdges();

		p.rotateBack(planes[k]->getXBorder(),planes[k]->getYBorder(),planes[k]->getZBorder(),planes[k]->getRotationMatrix(), planes[k]->getTranslationVector());

        //grid[k]->unrotateBorder(planes[k]->getRotationMatrix(),planes[k]->getTranslationVector())
		

        planes[k]->setCorners(planes[k]->unrotateCorners(rpe.getCorners()));
        planes[k]->setBestPoints(rpe.getBestPoints());

        p.rotateBack(planes[k]->getPointsUsed(), planes[k]->getPointNumber(),planes[k]->getRotationMatrix());
        p.translateBack(planes[k]->getPointsUsed(), planes[k]->getPointNumber(),planes[k]->getTranslationVector());
		planes[k]->writePlane(QString("Data\\plane%1.txt").arg((k+1)));
    }
                        
}