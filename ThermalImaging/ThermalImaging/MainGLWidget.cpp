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

#include "MainGLWidget.h"
#include <QtGui>
#include <QtOpenGL>
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
}

MainGLWidget::~MainGLWidget()
{
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

	// Load and assigne textures
	if ( !b.load( QString("C:\\Work\\VS2008 Projects\\ThermalImaging\\ThermalImaging\\Data\\Glass.bmp" )) )
	{
		//b = QImage( 16, 16, 32 );
		//b.fill( Qt::green.rgb());
		cout << "Cannot load texture." << endl;
	}
    
	glGenTextures(4, texture);
	
	t = QGLWidget::convertToGLFormat(b);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());

	if ( !b.load( QString("C:\\Work\\VS2008 Projects\\ThermalImaging\\ThermalImaging\\images\\side1.png" )) )
	{
		//b = QImage( 16, 16, 32 );
		//b.fill( Qt::green.rgb());
		cout << "Cannot load texture." << endl;
	}
	
	t = QGLWidget::convertToGLFormat(b);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());

	if (!b.load( QString("C:\\Work\\VS2008 Projects\\ThermalImaging\\ThermalImaging\\images\\test_tex2.jpg" )))
	{
		//b = QImage( 16, 16, 32 );
		//b.fill( Qt::green.rgb());
		cerr << "Cannot load texture." << endl;
	}
    
	t = QGLWidget::convertToGLFormat(b);
	glBindTexture( GL_TEXTURE_2D, texture[2]);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits() );

	if (!b.load( QString("C:\\Work\\VS2008 Projects\\ThermalImaging\\ThermalImaging\\images\\wilkins_tex_top.jpg" )))
	{
		//b = QImage( 16, 16, 32 );
		//b.fill( Qt::green.rgb());
		cerr << "Cannot load texture." << endl;
	}
    
	t = QGLWidget::convertToGLFormat(b);
	glBindTexture( GL_TEXTURE_2D, texture[3]);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits() );

	// Activate and bind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
}

void MainGLWidget::initializeGL()
{
    //glEnable(GL_CULL_FACE);  //###########################

	glewInit();
	LoadGLTextures();								// Jump To Texture Loading Routine ( NEW )
	initializeShaders();

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

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);					// Full Brightness.  100% Alpha
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);	// Set The Blending Function For Translucency
	glAlphaFunc(GL_GREATER,0.1f);


	plyParser.readPlyFile("wilkins3d.ply");
	vertices = plyParser.getVertices();
	indices = plyParser.getIndices();
	texCoords = plyParser.getTexCoords();
}

void MainGLWidget::initializeShaders()
{
	char *vs = NULL,*fs = NULL,*fs2 = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	vs = textFileRead("textureSimple.vert");			// Read shader source files
	fs = textFileRead("textureSimple.frag");

	const char * ff = fs;
	const char * ff2 = fs2;
	const char * vv = vs;

	glShaderSource(v, 1, &vv,NULL);						// Associate source with shader
	glShaderSource(f, 1, &ff,NULL);

	free(vs);
	free(fs);

	glCompileShader(v);									// Compile shaders
	glCompileShader(f);

	p = glCreateProgram();								// Create shader program
	glAttachShader(p,f);								// Attach shader to the program
	glAttachShader(p,v);

	glLinkProgram(p);									// Link and use program (replaces fixed functionality)
	glUseProgram(p);

	const unsigned int buffer_size = 512;
	char buffer[buffer_size];
	memset(buffer, 0, buffer_size);
	GLsizei length = 0;
	glGetShaderInfoLog(v, buffer_size, &length, buffer);	// Check for errors in vertex shader
	if (length > 0)
	{
		cerr << "Shader " << v << " compile error: " << buffer << endl;
	}
	
	glGetShaderInfoLog(f, buffer_size, &length, buffer);	// Check for errors in fragment shader
	if (length > 0)
	{
		cerr << "Shader " << f << " compile error: " << buffer << endl;
	}

	//int texture_location = glGetUniformLocation(p, "tex1");	// Setup communication between this and shaders
 //   glUniform1i(texture_location, texture[2]);

	//texture_location = glGetUniformLocation(p, "tex2");		// Setup communication between this and shaders
 //   glUniform1i(texture_location, texture[1]);
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

	//GLfloat vertices[] = {-1.0, -1.0,  1.0,		// front face
	//					   1.0, -1.0,  1.0,
	//					   1.0,	 1.0,  1.0,
	//					  -1.0,  1.0,  1.0,
	//					  
	//					  -1.0, -1.0, -1.0,		// back face
	//					  -1.0,  1.0, -1.0,
	//					   1.0,  1.0, -1.0,
	//					   1.0, -1.0, -1.0,
	//
	//					  -1.0,  1.0, -1.0,		// top face
	//					  -1.0,  1.0,  1.0,
	//					   1.0,  1.0,  1.0,
	//					   1.0,  1.0, -1.0,

	//				      -1.0, -1.0, -1.0,		// bottom face
	//					   1.0, -1.0, -1.0,
	//					   1.0, -1.0,  1.0,
	//				      -1.0, -1.0,  1.0,

	//					   1.0, -1.0, -1.0,		// right face
	//					   1.0,  1.0, -1.0,
	//					   1.0,  1.0,  1.0,
	//					   1.0, -1.0,  1.0,

	//					  -1.0, -1.0, -1.0,		// left face
	//					  -1.0, -1.0,  1.0,
	//					  -1.0,  1.0,  1.0,
	//					  -1.0,  1.0, -1.0};

	//GLuint indices[] = { 0,  1,  2,  3,
	//					 4,  5,  6,  7,
	//					 8,  9, 10, 11,
	//					12, 13, 14, 15,
	//					16, 17, 18, 19,
	//					20, 21, 22, 23};

	//GLfloat texCoords[] = {0.0, 0.0,
	//					   1.0, 0.0,
	//					   1.0, 1.0,
	//					   0.0, 1.0,
	//					   
	//					   1.0, 0.0,
	//					   1.0, 1.0,
	//					   0.0, 1.0,
	//					   0.0, 0.0,

	//					   0.0, 1.0,
	//					   0.0, 0.0,
	//					   1.0, 0.0,
	//					   1.0, 1.0,

	//					   1.0, 1.0,
	//					   0.0, 1.0,
	//					   0.0, 0.0,
	//					   1.0, 0.0,

	//					   1.0, 0.0,
	//					   1.0, 1.0,
	//					   0.0, 1.0,
	//					   0.0, 0.0,

	//					   0.0, 0.0,
	//					   1.0, 0.0,
	//					   1.0, 1.0,
	//					   0.0, 1.0};

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glVertexPointer(plyParser.getVertexSize(), GL_FLOAT, 0, vertices);
	//glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	//glDrawElements(GL_TRIANGLES, plyParser.getNFaces()*plyParser.getVerticesPerFace(), GL_UNSIGNED_INT, indices);

	//TODO: change to use sizes from plyParser not constants

	int texLoc1 = glGetUniformLocation(p, "tex1");	// Setup communication between this and shaders
	glUniform1i(texLoc1, 0);
	int texLoc2 = glGetUniformLocation(p, "tex2");	// Setup communication between this and shaders
	glUniform1i(texLoc2, 2);
	
	glBegin(GL_TRIANGLES);
		for (int face = 0; face < 12 /*plyParser.getNFaces()*/; face++)
		{
			int texIndex = face * plyParser.getVerticesPerFace() * plyParser.getTexCoordSize();

			glTexCoord2f(texCoords[texIndex], texCoords[texIndex+1]);
			glArrayElement(indices[face*3]);
			glTexCoord2f(texCoords[texIndex+2], texCoords[texIndex+3]);
			glArrayElement(indices[face*3 + 1]);
			glTexCoord2f(texCoords[texIndex+4], texCoords[texIndex+5]);
			glArrayElement(indices[face*3 + 2]);
		}
	glEnd();

	glUniform1i(texLoc1, 1);
	glUniform1i(texLoc2, 3);
	
	//glBindTexture( GL_TEXTURE_2D, texture[1]);
	glBegin(GL_TRIANGLES);
		for (int face = 12; face < plyParser.getNFaces(); face++)
		{
			int texIndex = face * plyParser.getVerticesPerFace() * plyParser.getTexCoordSize();

			glTexCoord2f(texCoords[texIndex], texCoords[texIndex+1]);
			glArrayElement(indices[face*3]);
			glTexCoord2f(texCoords[texIndex+2], texCoords[texIndex+3]);
			glArrayElement(indices[face*3 + 1]);
			glTexCoord2f(texCoords[texIndex+4], texCoords[texIndex+5]);
			glArrayElement(indices[face*3 + 2]);
		}
	glEnd();
	
	int blend_loc = glGetUniformLocation(p, "blend");
	glUniform1f(blend_loc, alpha);
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

  case Qt::Key_Up:
	  alpha -= 0.1f;
	  break;
    
  }

  updateGL();
}

void MainGLWidget::thermalVisualPercent(int p) {
	alpha = (float) p / 100.0f;
	updateGL();
}