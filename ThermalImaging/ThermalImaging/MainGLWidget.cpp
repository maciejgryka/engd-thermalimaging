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

#include "MainGLWidget.h"
#include <iostream>

using namespace std;

MainGLWidget::MainGLWidget(QWidget *parent, QGLWidget *shareWidget)
    : QGLWidget(parent, shareWidget)
{
    clearColor = Qt::black;
    xRot = 0;
    yRot = 0;
    zRot = 0;

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
	blend = false;

	setFocusPolicy(Qt::StrongFocus);

	/*extractor = new Ply2OpenGL();
	extractor->setUp();

	nPoints = extractor->getNumberOfPoints();
	points = extractor->convert2Matrix(extractor->getPoints());
	colors = extractor->convert2Matrix(extractor->getColors());*/

	rpc = new RandomPointCloud();
	rpc->makePointCloud(2,1000,1000);
	points = rpc->getPointCloud();
	colors = rpc->getColors();
	nPoints = rpc->getNumberOfElements();

	planesFound = 0;
	bestPointCombinations = new int*[4];

	ranRansac = false;
	ransac = new Ransac();
	ransac->setInlierDistance(0.05);
	ransac->setIterations(10000);
	ransac->setPoints(points, nPoints);

	//plf = new PlaneLimitFinder();
}

MainGLWidget::~MainGLWidget() {
	if (ranRansac) {
		for(int i = 0; i < 4; i++) {
			free(plane[i]);
		}
		free(plane);
	}
	for (int i = nPoints-1; i>=0; i--) {
		free(points[i]);
		free(colors[i]);
	}
	delete(ransac);
	//delete(extractor);
	delete(rpc);
}

QSize MainGLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize MainGLWidget::sizeHint() const
{
    return QSize(200, 200);
}

void MainGLWidget::rotateBy(int xAngle, int yAngle, int zAngle)
{
    xRot += xAngle;
    yRot += yAngle;
    zRot += zAngle;
    updateGL();
}

void MainGLWidget::setClearColor(const QColor &color)
{
    clearColor = color;
    updateGL();
}

void MainGLWidget::LoadGLTextures()									// Load Bitmaps And Convert To Textures
{
  QImage t;
  QImage b;
    
  if ( !b.load( QString("C:\\Users\\localadmin\\Documents\\Visual Studio 2008\\Projects\\ThermalImaging\\ThermalImaging\\Data\\Glass.bmp" )) )
  {
	  int a = 0;
    //b = QImage( 16, 16, 32 );
    //b.fill( Qt::green.rgb());
  }
    
  t = QGLWidget::convertToGLFormat( b );
  glGenTextures( 4, &texture[0] );
  /*glBindTexture( GL_TEXTURE_2D, texture[0] );
  glTexImage2D( GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits() );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );*/

  // Create Nearest Filtered Texture
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());

	// Create Linear Filtered Texture
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());

	// Create MipMapped Texture
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, t.width(), t.height(), GL_RGBA, GL_UNSIGNED_BYTE, t.bits());

	if ( !b.load( QString("C:\\Users\\localadmin\\Documents\\Visual Studio 2008\\Projects\\ThermalImaging\\ThermalImaging\\images\\side1.png" )) )
	  {
		  int a = 0;
		//b = QImage( 16, 16, 32 );
		//b.fill( Qt::green.rgb());
	  }
    
  t = QGLWidget::convertToGLFormat( b );
  glBindTexture( GL_TEXTURE_2D, texture[3] );
  glTexImage2D( GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits() );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

}

void MainGLWidget::initializeGL()
{
    //glEnable(GL_CULL_FACE);  //###########################

	LoadGLTextures();								// Jump To Texture Loading Routine ( NEW )

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
	glEnable(GL_LIGHT1);								// Enable Light One

	glColor4f(1.0f, 1.0f, 1.0f, alpha);					// Full Brightness.  50% Alpha
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);	// Set The Blending Function For Translucency
	glAlphaFunc(GL_GREATER,0.1f);

}

void MainGLWidget::paintGL()
{

    qglClearColor(clearColor);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -10.0f);
    glRotatef(xRot / 16.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot / 16.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot / 16.0f, 0.0f, 0.0f, 1.0f);




	glPointSize(1.0f);
	glBegin(GL_POINTS);
	
		for (int i= 0; i < nPoints; i++) {
			//if (ranRansac && pointsUsed[i] == 1) {
			//	glColor3f((float) rand() / (float) RAND_MAX, (float) rand() / (float) RAND_MAX, (float) rand() / (float) RAND_MAX);
			//} else {
				glColor3fv(colors[i]);
			//}
			glVertex3fv(points[i]);
		}
	glEnd();
	if (ranRansac) {
		for (int i = 0; i < planesFound; i++) {
			glBegin(GL_TRIANGLES);
				glColor3fv(colors[bestPointCombinations[i][0]]);
				glVertex3fv(points[bestPointCombinations[i][0]]);
				glVertex3fv(points[bestPointCombinations[i][1]]);
				glVertex3fv(points[bestPointCombinations[i][2]]);
			glEnd();
		}
	}

	/*glDisable(GL_BLEND);	

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_QUADS);
		// Front Face
		//glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		// Back Face
		//glNormal3f( 0.0f, 0.0f,-1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		// Top Face
		//glNormal3f( 0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		// Bottom Face
		//glNormal3f( 0.0f,-1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		// Right face
		//glNormal3f( 1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		// Left Face
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);

	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, alpha);
	glBindTexture(GL_TEXTURE_2D, texture[filter]);

	if (blend) {
		glEnable(GL_BLEND);
	}

	glBegin(GL_QUADS);
		// Front Face
		//glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.01f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.01f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.01f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.01f);
		// Back Face
		//glNormal3f( 0.0f, 0.0f,-1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.01f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.01f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.01f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.01f);
		// Top Face
		//glNormal3f( 0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.01f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.01f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.01f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.01f, -1.0f);
		// Bottom Face
		//glNormal3f( 0.0f,-1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.01f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.01f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.01f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.01f,  1.0f);
		// Right face
		//glNormal3f( 1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.01f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.01f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.01f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.01f, -1.0f,  1.0f);
		// Left Face
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.01f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.01f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.01f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.01f,  1.0f, -1.0f);

	glEnd();*/


    /*glVertexPointer(3, GL_FLOAT, 0, vertices.constData());
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords.constData());
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);



    for (int i = 0; i < 6; ++i) {
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
    }*/
}

void MainGLWidget::resizeGL(int width, int height)
{
	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();	

}

void MainGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void MainGLWidget::mouseMoveEvent(QMouseEvent *event)
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

void MainGLWidget::mouseReleaseEvent(QMouseEvent * /* event */)
{
    emit clicked();
}

void MainGLWidget::keyPressEvent( QKeyEvent *e )
{
  switch( e->key() )
  {
  case Qt::Key_L:
    light = !light;
      
    if( light )
      glEnable( GL_LIGHTING );
    else    
      glDisable( GL_LIGHTING );
    
    break;
      
  case Qt::Key_F:
    filter++;
    if( filter > 2 )
      filter = 0;
    
    break;
      
  case Qt::Key_B:
    blend = !blend;
	if(blend) {
		glEnable(GL_BLEND);			// Turn Blending On
		glDisable(GL_DEPTH_TEST);	// Turn Depth Testing Off
	} else {
		glDisable(GL_BLEND);		// Turn Blending Off
		glEnable(GL_DEPTH_TEST);	// Turn Depth Testing On
	}

    break;
    
  }

  updateGL();
}

void MainGLWidget::thermalVisualPercent(int p) {
	alpha = (float) p / 100.0f;
	updateGL();
}

void MainGLWidget::executeRansac() {
	if (ranRansac) {
		for(int i = 0; i < 4; i++) {
			free(plane[i]);
		}
		free(plane);
	}
	Vector3f normal;
	Vector3f origin;
	int noPoints;
	int bestCombi[3] = {0,0,0};
	pointsUsed = ransac->findBestPlane(planesFound+1, noPoints, normal, origin, bestCombi);
	if (planesFound < 4) {
		bestPointCombinations[planesFound] = new int[3];
		bestPointCombinations[planesFound][0] = bestCombi[0];
		bestPointCombinations[planesFound][1] = bestCombi[1];
		bestPointCombinations[planesFound][2] = bestCombi[2];
		planesFound++;
	}
	plane = new float*[4];
	Vector3f normal2((-normal(1)-normal(2))/normal(0),1,1);
	normal2 = normal2.normalized();
	Vector3f point;
	for(int i = 0; i < 4; i++) {
		plane[i] = new float[3];
		if (i == 0)
			point = origin + normal2 * 100.0f;
		else if (i == 1)
			point = origin + normal2.cross(normal).normalized() * 100.0f;
		else if (i == 2)
			point = origin - normal2 * 100.0f;
		else
			point = origin - normal2.cross(normal).normalized() * 100.0f;
		for (int j = 0; j < 3; j++) {
			plane[i][j] = point(j);
		}
	}
	ranRansac = true;
	

}