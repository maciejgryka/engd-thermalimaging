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

#include "LabelGLWidget.h"
#include <QtGui>
#include <QtOpenGL>
#include <iostream>

using namespace std;

LabelGLWidget::LabelGLWidget(QWidget *parent, QGLWidget *shareWidget)
    : QGLWidget(parent, shareWidget)
{
    clearColor = Qt::black;
    /*xRot = 0;
    yRot = 0;
    zRot = 0;
	xTrans = 0.0;
	yTrans = 0.0;
	zTrans = -30.0;
	zoomDist = 0.0;

	texNum = false;

	LightAmbient[0] = 0.5f;
	LightAmbient[1] = 0.5f;
	LightAmbient[2] = 0.5f;
	LightAmbient[3] = 1.0f;
	LightDiffuse[0] = 1.0f;
	LightDiffuse[1] = 1.0f;
	LightDiffuse[2] = 1.0f;
	LightDiffuse[3] = 1.0f;
	LightPosition[0] = 0.0f;
	LightPosition[1] = 0.0f;
	LightPosition[2] = 2.0f;
	LightPosition[3] = 1.0f;

	filter = 0;
	alpha = 1.0f;
	light = false;
	blend = false;*/

	setFocusPolicy(Qt::StrongFocus);
}

LabelGLWidget::~LabelGLWidget()
{
}

QSize LabelGLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize LabelGLWidget::sizeHint() const
{
    return QSize(200, 200);
}

void LabelGLWidget::setClearColor(const QColor &color)
{
    clearColor = color;
    updateGL();
}

void LabelGLWidget::initializeGL()
{


	glewInit();
	//LoadGLTextures();								// Jump To Texture Loading Routine ( NEW )
	//initializeShaders();

	glEnable(GL_TEXTURE_2D);						// Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);						// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);					// Black Background
	glClearDepth(1.0f);							// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);						// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);							// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Really Nice Perspective Calculations

	QImage t;
	QImage b;

	QString file ("images\\tempLegend.jpg");
	if ( !b.load(file) ) {
		cout << "Cannot load texture." << endl;
	}

	t = QGLWidget::convertToGLFormat(b);

	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// Linear Filtering

	glTexImage2D(GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());

	//glBindTexture(GL_TEXTURE_2D, texture[0]);
}

void LabelGLWidget::paintGL()
{
    qglClearColor(clearColor);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glTranslatef(0, 0, -4.8f);

	int a = 2.5;

	GLfloat vertices[] = {-a, -a,  0.0,		// front face
							a, -a,  0.0,
							a,  a,  0.0,
							-a, a,  0.0};

	GLfloat texCoords[] = {0.0, 0.0,
							1.0, 0.0,
							1.0, 1.0,
							0.0, 1.0};

	glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, texture[0] );

	glBegin(GL_QUADS);
	for (int i = 0; i < 4; i++) {
		glTexCoord2f(texCoords[i*2],texCoords[i*2+1]);
		glVertex3f(vertices[i*3],vertices[i*3+1],vertices[i*3+2]);
	}
	glEnd();

}

void LabelGLWidget::resizeGL(int width, int height)
{
	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,1000.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();	

}

void LabelGLWidget::changeVisibility(int state) {
	if (state == Qt::Checked)
		show();
	else
		hide();
}
