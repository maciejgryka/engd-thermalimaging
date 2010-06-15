#ifndef PLY2OPENGL
#define PLY2OPENGL

#include <iostream>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>

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

	int* getIndices() { return indices; };
	
	float* getTexCoords() { return texCoords; };

	int getNVertices() { return nVertices; };

	int getNFaces() { return nFaces; };

	int getVertexSize() { return vertexSize; };

	int getVerticesPerFace() { return verticesPerFace; };

	int getTexCoordSize() { return texCoordSize; };

	//float* getColors();
	//float* getNormals();

private:
	float* vertices;
	int* indices;
	float* texCoords;
	//float* colors;
	//float* normals;

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