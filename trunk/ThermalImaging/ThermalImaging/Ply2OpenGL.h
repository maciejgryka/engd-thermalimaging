#ifndef PLY2OPENGL
#define PLY2OPENGL

#define NOMINMAX

#include <QFile>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QFileDialog>

#include <iostream>

class Ply2OpenGL {

private:
	int nPoints;
	QString inputFile;
	float *points;
	float *normals;
	float *colors;
	
public:
	Ply2OpenGL();
	~Ply2OpenGL();
	void setUp();
	void setFile();
	int createFiles();
	int getNumberOfPoints();
	float *getPoints();
	float *getNormals();
	float *getColors();
	float **convert2Matrix(float*);

};

#endif