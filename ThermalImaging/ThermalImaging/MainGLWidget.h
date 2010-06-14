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

#ifndef MAINGLWIDGET
#define MAINGLWIDGET

#define NOMINMAX			// Required for Eigen

#include <GL/glew.h>		// GLEW includes gl.h and glu.h
#include <QtGui>
#include <QGLWidget>
#include <QVector3D>
#include <QVector2D>
#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
//#include <gl\gl.h>			// Header File For The OpenGL32 Library
//#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>

#include "Ply2OpenGL.h"
#include "textfile.h"		// utility class for reading textfiles - REPLACE WITH QT

class QGLShaderProgram;

class MainGLWidget : public QGLWidget
{
    Q_OBJECT

public:
    MainGLWidget(QWidget *parent = 0, QGLWidget *shareWidget = 0);
	~MainGLWidget();
	void LoadGLTextures();
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void rotateBy(int xAngle, int yAngle, int zAngle);
    void setClearColor(const QColor &color);

public slots:
	void thermalVisualPercent(int);

signals:
    void clicked();

protected:
    void initializeGL();
	void initializeShaders();
    void paintGL();
    void resizeGL(int width, int height);
	void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:

    QColor clearColor;
    QPoint lastPos;
    int xRot;
    int yRot;
    int zRot;
	GLuint texture[2];
    GLuint texLoc[2];
	GLuint blendLoc;
	GLuint v, f, p; //vector and fragment shaders and shader program
	
	Ply2OpenGL plyParser;
    float* vertices;
	float* texCoords;
    int* indices;

	GLuint	filter;				// Which Filter To Use
	bool	light;				// Lighting ON/OFF
	bool    blend;				// Blending OFF/ON? ( NEW )
	GLfloat alpha;


	GLfloat LightAmbient[4];
	GLfloat LightDiffuse[4];
	GLfloat LightPosition[4];

	

};

#endif
