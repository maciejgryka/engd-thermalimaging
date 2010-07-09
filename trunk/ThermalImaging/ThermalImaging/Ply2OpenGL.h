#ifndef PLY2OPENGL
#define PLY2OPENGL

#include <iostream>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QString>

#include <QTDebug>

class Ply2OpenGL {

public:
	Ply2OpenGL(){};
	~Ply2OpenGL()
	{
		delete vertices;
		delete indices;
		delete texCoords;
		//delete colors;
		//delete normals;
	};

	bool readPlyFile(QString filename);
	
	float* getVertices() { return vertices;	};

	float* getNormals() { return normals; };
	float* getColors() { return colors; };

	int* getIndices() { return indices; };
	
	float* getTexCoords() { return texCoords; };

	int getNVertices() { return nVertices; };

	int getNFaces() { return nFaces; };

	int getVertexSize() { return vertexSize; };

	int getVerticesPerFace() { return verticesPerFace; };

	int getTexCoordSize() { return texCoordSize; };

	float** toTwoDimensionalArray(float*);

	bool hasTexNums() {return texNum;} ;
	int* getTexNums() {return texNums;} ;

	QVector<QString>* getTextureFiles() {return textureFiles;};

	//float* getColors();
	//float* getNormals();

private:
	float* vertices;
	float* normals;
	float* colors;
	int* indices;
	float* texCoords;
	int* texNums;
	QStringList text;
	//float* colors;
	//float* normals;

	bool texNum;
	QVector<QString>* textureFiles;

	int nVertices;
	int nFaces;
	
	static const int vertexSize = 3;
	static const int verticesPerFace = 3;
	static const int texCoordSize = 2;
	//static const int colorSize = 3;
	//static const int normalSize = 3;

	QString getL(QTextStream &stream);
};

#endif