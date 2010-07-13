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

#ifndef TESTGLWIDGET
#define TESTGLWIDGET

#define NOMINMAX

#include <Eigen/Eigen>
USING_PART_OF_NAMESPACE_EIGEN

#include <QTDebug>
#include <QtGui>
#include <QGLWidget>
//#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

//#include "thermalimaging.h"
#include "Ply2OpenGL.h"
#include "RansacPlane.h"
#include "RandomPointCloud.h"

#include "PlaneLimitFinder.h"
#include "PlaneInfo.h"

#include "PlaneCalculator.h"
#include "Quad.h"
#include "Grid.h"

#include "RansacPlaneEdge.h"
#include "Texturer.h"


class QGLShaderProgram;

class TestGLWidget : public QGLWidget
{
    Q_OBJECT

public:
    TestGLWidget(QWidget *parent = 0, QGLWidget *shareWidget = 0);
    ~TestGLWidget();
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void rotateBy(int xAngle, int yAngle, int zAngle);
    void setClearColor(const QColor &color);

	void runRPE();
	void runQuad();

	float* getBoundary(int);

signals:
    void clicked();
	void changeLegendImage(int);

public slots:
	void setMaxLevels(int);
	void setChillPoints(int);
	void setEdgeInlier(int);
	void rerunQuad();
	void rerunRPE();
	void time0(bool);
	void time1(bool);
	void time2(bool);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
	void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:

	float** points;
	int nPoints;

	int* pointList;

	float** pop;
	int npop;
	int* clusters;
	int nclusters;

	int time;

	//Quad** q;
	//Grid** grid;
	PlaneInfo** planes;

	float** colors;

    QColor clearColor;
    QPoint lastPos;
    int xRot;
    int yRot;
    int zRot;
	float xTrans;
	float yTrans;
	float zTrans;
	float zoomDist;

	RandomPointCloud* rpc;
	Ply2OpenGL* ply;
	RansacPlaneEdge rpe;
	Texturer tx;

	int maxLevels;
	float edgeInlierDistance;
	float chillPoints;

	int noPlanes;

	int ps;
	int o;

	QString directory;

	PlaneCalculator p;

	void mapTextures(Vector3f translationVector, Matrix3f rotationMatrix, const vector<vector<float> > &corners);
	bool* readPlane;
};

#endif
