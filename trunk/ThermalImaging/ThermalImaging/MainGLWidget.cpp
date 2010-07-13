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

void MainGLWidget::loadTexture(int i, int v) {

	QImage t;
	QImage b;

	//qDebug() << textureFiles->size() << textureFiles->last() << textureFiles->first();

	QString file ("images\\");
	if (v == 1) {
		file.append("t");
	}
	file.append(textureFiles->at(i));
	if ( !b.load(file) ) {
		cout << "Cannot load texture." << endl;
	}
	
	t = QGLWidget::convertToGLFormat(b);
	glBindTexture(GL_TEXTURE_2D, texture[i*2 + v]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());
	//qDebug() << file << i*2 + v << texture[i*2 + v];

	

}

void MainGLWidget::LoadGLTextures()									// Load Bitmaps And Convert To Textures
{
	// Load and assign textures
	
	texture = new GLuint[textureFiles->size() * 2];
	texLoc = new GLuint[textureFiles->size() * 2];

	glGenTextures(textureFiles->size() * 2, texture);
	
	for (int i = 0; i < textureFiles->size() ; i++) {
		
		loadTexture(i,0);
		loadTexture(i,1);
	}

	for (int i = 0; i < textureFiles->size() * 2; i++) {
		switch(i) {
			case 0: glActiveTexture(GL_TEXTURE0); break;
			case 1: glActiveTexture(GL_TEXTURE1); break;
			case 2: glActiveTexture(GL_TEXTURE2); break;
			case 3: glActiveTexture(GL_TEXTURE3); break;
			case 4: glActiveTexture(GL_TEXTURE4); break;
			case 5: glActiveTexture(GL_TEXTURE5); break;
			case 6: glActiveTexture(GL_TEXTURE6); break;
			case 7: glActiveTexture(GL_TEXTURE7); break;
			case 8: glActiveTexture(GL_TEXTURE8); break;
			case 9: glActiveTexture(GL_TEXTURE9); break;
			case 10: glActiveTexture(GL_TEXTURE10); break;
			case 11: glActiveTexture(GL_TEXTURE11); break;
			case 12: glActiveTexture(GL_TEXTURE12); break;
			case 13: glActiveTexture(GL_TEXTURE13); break;
			case 14: glActiveTexture(GL_TEXTURE14); break;
			case 15: glActiveTexture(GL_TEXTURE15); break;
			case 16: glActiveTexture(GL_TEXTURE16); break;
			case 17: glActiveTexture(GL_TEXTURE17); break;
			case 18: glActiveTexture(GL_TEXTURE18); break;
			case 19: glActiveTexture(GL_TEXTURE19); break;
			case 20: glActiveTexture(GL_TEXTURE20); break;
			case 21: glActiveTexture(GL_TEXTURE21); break;
			case 22: glActiveTexture(GL_TEXTURE22); break;
			case 23: glActiveTexture(GL_TEXTURE23); break;
			case 24: glActiveTexture(GL_TEXTURE24); break;
			case 25: glActiveTexture(GL_TEXTURE25); break;
			case 26: glActiveTexture(GL_TEXTURE26); break;
			case 27: glActiveTexture(GL_TEXTURE27); break;
			case 28: glActiveTexture(GL_TEXTURE28); break;
			case 29: glActiveTexture(GL_TEXTURE29); break;
			case 30: glActiveTexture(GL_TEXTURE30); break;
			case 31: glActiveTexture(GL_TEXTURE31); break;
		}
		glBindTexture(GL_TEXTURE_2D, texture[i]);

	}
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

	// Setup communication between this and shaders
	texLoc[0] = glGetUniformLocation(p, "tex1");
	texLoc[1] = glGetUniformLocation(p, "tex2");
	blendLoc = glGetUniformLocation(p, "blend");
}

void MainGLWidget::initializeGL()
{
    //glEnable(GL_CULL_FACE);  //###########################

	plyParser.readPlyFile("Data\\wilkins3d2.ply");
	vertices = plyParser.getVertices();
	indices = plyParser.getIndices();
	texCoords = plyParser.getTexCoords();
	texNum = plyParser.hasTexNums();
	if (texNum) {
		texNums = plyParser.getTexNums();
	}
	textureFiles = plyParser.getTextureFiles();

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
}

void MainGLWidget::paintGL()
{
    qglClearColor(clearColor);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glTranslatef(xTrans, yTrans, zTrans - zoomDist);
    glRotatef(xRot / 16.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot / 16.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot / 16.0f, 0.0f, 0.0f, 1.0f);	

	/*GLfloat vertices[] = {-1.0, -1.0,  1.0,		// front face
							1.0, -1.0,  1.0,
							1.0,  1.0,  1.0,
							-1.0, 1.0,  1.0,
							1.0, 1.0, 1.0,		// side face
							3.0,  1.0, 1.0,
							3.0,  3.0, 1.0,
							1.0, 3.0, 1.0};
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

	GLuint indices[] = { 0, 1, 2,
						0, 2, 3,
						4, 5, 6,  
						4, 6, 7};
	//					 8,  9, 10, 11,
	//					12, 13, 14, 15,
	//					16, 17, 18, 19,
	//					20, 21, 22, 23};

	GLfloat texCoords[] = {0.0, 0.0,
							1.0, 0.0,
							1.0, 1.0,
							0.0, 0.0,
							1.0, 1.0,
							0.0, 1.0,
							0.0, 0.0,
							1.0, 0.0,
							1.0, 1.0,
							0.0, 0.0,
							1.0, 1.0,
							0.0, 1.0};*/
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

	/*glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 4; i++) {
		glVertex3fv(vertices1[i]);
		glTexCoord2fv(texCoords1[i]);
	}
	glEnd();
	glBegin(GL_POLYGON);
	for (int i = 0; i < 4; i++) {
		glVertex3fv(vertices2[i]);
		glTexCoord2fv(texCoords1[i]);
	}
	glEnd();*/
	// working
	glEnableClientState(GL_VERTEX_ARRAY);
	
	glVertexPointer(plyParser.getVertexSize(), GL_FLOAT, 0, vertices);

	glUniform1f(blendLoc, alpha);

	for (int i = 0; i < textureFiles->size(); i++) {
		glUniform1i(texLoc[0], i*2);
		glUniform1i(texLoc[1], i*2+1);
		glBegin(GL_TRIANGLES);
			//for (int face = 0; face < 2; face++)
			for (int face = 0; face < plyParser.getNFaces(); face++)
			{
				if (texNum && texNums[face] == i || !texNum) {

					int texIndex = face * plyParser.getVerticesPerFace() * plyParser.getTexCoordSize();
					//int texIndex = face * 3 * 2;
					glTexCoord2f(texCoords[texIndex], texCoords[texIndex+1]);
					glArrayElement(indices[face*3]);
					glTexCoord2f(texCoords[texIndex+2], texCoords[texIndex+3]);
					glArrayElement(indices[face*3 + 1]);
					glTexCoord2f(texCoords[texIndex+4], texCoords[texIndex+5]);
					glArrayElement(indices[face*3 + 2]);
					/*qDebug() << texCoords[texIndex] << texCoords[texIndex+1] << texCoords[texIndex+2] << texCoords[texIndex+3] << texCoords[texIndex+4] << texCoords[texIndex+5];
					qDebug() << indices[face*3+0] << indices[face*3+1] << indices[face*3+2];
					qDebug() << face << i;*/
				}
			}
		glEnd();

	}


	/*glUniform1i(texLoc[0], 2);
	glUniform1i(texLoc[1], 3);
	glBegin(GL_TRIANGLES);
		for (int face = 2; face < 4; face++)
			//for (int face = 0; face < plyParser.getNFaces(); face++)
		{
			//int texIndex = face * plyParser.getVerticesPerFace() * plyParser.getTexCoordSize();
			int texIndex = face * 3 * 2;
			glTexCoord2f(texCoords[texIndex], texCoords[texIndex+1]);
			glArrayElement(indices[face*3]);
			glTexCoord2f(texCoords[texIndex+2], texCoords[texIndex+3]);
			glArrayElement(indices[face*3 + 1]);
			glTexCoord2f(texCoords[texIndex+4], texCoords[texIndex+5]);
			glArrayElement(indices[face*3 + 2]);
		}
	glEnd();*/
}

void MainGLWidget::resizeGL(int width, int height)
{
	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,1000.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();	

}

void MainGLWidget::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void MainGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->x() - lastPos.x();
    int dy = e->y() - lastPos.y();

    if (e->buttons() & Qt::LeftButton) {
        rotateBy(8 * dy, 8 * dx, 0);
    } else if (e->buttons() & Qt::RightButton) {
        rotateBy(8 * dy, 0, 8 * dx);
    } else if (e->buttons() & Qt::MidButton) {

		// 0.4142 = tan(22.5 deg) (2.0f * 0.4142135623f *
        xTrans -= dx * (zTrans - zoomDist) / 1000.0f;
        yTrans += dy * (zTrans - zoomDist) / 1000.0f;
        updateGL();
    }

    lastPos = e->pos();
}

void MainGLWidget::mouseReleaseEvent(QMouseEvent * /* event */)
{
    emit clicked();
}

void MainGLWidget::wheelEvent(QWheelEvent *e)
{
	if (zTrans-zoomDist < -1.0f || zTrans-zoomDist > -1.0f && e->delta() < 0)
		zoomDist += (float) e->delta()/2000.0f * (zTrans - zoomDist);
	updateGL();
}

void MainGLWidget::keyPressEvent(QKeyEvent *e)
{
	float step = 0.3;
 /* switch( e->key() )
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
  }*/
  updateGL();
}

void MainGLWidget::thermalVisualPercent(int p) {
	alpha = (float) p / 100.0f;
	updateGL();
}