#include "Ply2OpenGL.h"


bool Ply2OpenGL::readPlyFile(QString filename)
{
	QFile plyFile(filename);


	if (plyFile.open(QIODevice::ReadOnly))
	{
		// Read the file
		QTextStream stream(&plyFile);
		//QStringList
		QString textAsLine = stream.readAll();
		text = textAsLine.split("\r\n");
		if (text.size() == 1) {
			text = textAsLine.split("\n");
		}
		/*int *comments = new int[text.size()];

		int pos = 0;
		int nComments = 0;
		for (int i = 0; i < text.size(); i++) {
			if (!text.at(i).startsWith("comment")) {
				pos = i;
				nComments = 0;
			}
			else {
				nComments++;
			}
			comments[pos] = nComments + 1; 
		}*/
		int position = 0;

		if (text[position].compare("ply"))
		{
			std::cout << "Not a valid .ply file." << std::endl;
			return 0;
		}

		position++;

		if (text.at(position).compare("format ascii 1.0"))
		{
			std::cout << "Wrong format: only ascii 1.0 supported." << std::endl;
			return 0;
		}

		QStringList lineList;
		bool inHeader = true;
		texNum = false;
		textureFiles = new QVector<QString>();
		// analyse the PLY header
		while (inHeader)
		{
			position++;

			if (!text.at(position).compare("end_header"))
			{
				inHeader = false;
				break;
			}

			lineList = text.at(position).split(" ");
			// assign memory to store all the information
			if (!lineList.at(0).compare("element") && !lineList.at(1).compare("vertex"))
			{
				nVertices = lineList.at(2).toInt();
				vertices =	new float[nVertices * vertexSize];
				normals =	new float[nVertices * vertexSize];
				colors =	new float[nVertices * vertexSize];
				//texCoords = new float[nVertices * texCoordSize];
				//colors =	new float[nVertices * colorSize];
				//normals =	new float[nVertices * normalSize];
			} 
			else if (!lineList.at(0).compare("element") && !lineList.at(1).compare("face"))
			{
				nFaces = lineList.at(2).toInt();
				indices = new int[nFaces * verticesPerFace];
				texCoords = new float[nFaces * verticesPerFace * texCoordSize];
			} else if (!lineList.at(0).compare("property") && !lineList.at(1).compare("int") && !lineList.at(2).compare("texnumber")) {
				texNum = true;
			} else if (!lineList.at(0).compare("comment") && !lineList.at(1).compare("TextureFile")) {
				QString s = lineList.at(2);
				textureFiles->append(s);
			}
			//header += line;
		}

		bool nor = false;
		bool col = false;
		// assign all the vertex values
		for (int vert = 0; vert < nVertices; vert++) {
			position++;
			if (text.at(position).startsWith("comment")) {
				vert--;
				continue;
			}
			
			lineList = text.at(position).split(" ");
			// write vertices
			vertices[vert*vertexSize]	  = lineList.at(0).toFloat();
			vertices[vert*vertexSize + 1] = lineList.at(1).toFloat();
			vertices[vert*vertexSize + 2] = lineList.at(2).toFloat();
			if (lineList.size() >= 6) {
				normals[vert*vertexSize]	  = lineList.at(3).toFloat();
				normals[vert*vertexSize + 1] = lineList.at(4).toFloat();
				normals[vert*vertexSize + 2] = lineList.at(5).toFloat();
				nor = true;
			}
			if (lineList.size() >= 9) {
				colors[vert*vertexSize]	  = lineList.at(6).toFloat() / 255.0f;
				colors[vert*vertexSize + 1] = lineList.at(7).toFloat() / 255.0f;
				colors[vert*vertexSize + 2] = lineList.at(8).toFloat() / 255.0f;
				col = true;
			}

			//// write texture coordinates
			//texCoords[vert*texCoordSize] = lineList.at(3).toFloat();
			//texCoords[vert*texCoordSize + 1] = lineList.at(4).toFloat();
		}
		if (!nor)
			delete[] normals;
		if (!col)
			delete[] colors;

		// assign all the face values
		if (texNum)
			texNums = new int[nFaces];
		for (int face = 0; face < nFaces; face++)
		{
			position++;
			if (text.at(position).startsWith("comment")) {
				face--;
				continue;
			}
			lineList = text.at(position).split(" ");
		
			if (lineList.at(0).toInt() != verticesPerFace)
			{
				std::cerr << "Error: number of vertices per face must be 3 (traingles only)." << std::endl;
				return false;

			}

			// write face index values
			indices[face*verticesPerFace]	  = lineList.at(1).toInt();
			indices[face*verticesPerFace + 1] = lineList.at(2).toInt();
			indices[face*verticesPerFace + 2] = lineList.at(3).toInt();

			// write texture coordinates
			texCoords[face*verticesPerFace*texCoordSize] = lineList.at(5).toFloat();
			texCoords[face*verticesPerFace*texCoordSize + 1] = lineList.at(6).toFloat();
			texCoords[face*verticesPerFace*texCoordSize + 2] = lineList.at(7).toFloat();
			texCoords[face*verticesPerFace*texCoordSize + 3] = lineList.at(8).toFloat();
			texCoords[face*verticesPerFace*texCoordSize + 4] = lineList.at(9).toFloat();
			texCoords[face*verticesPerFace*texCoordSize + 5] = lineList.at(10).toFloat();

			// write face index values
			indices[face*verticesPerFace]	  = lineList.at(1).toInt();
			indices[face*verticesPerFace + 1] = lineList.at(2).toInt();
			indices[face*verticesPerFace + 2] = lineList.at(3).toInt();

			// write texture coordinates
			texCoords[face*verticesPerFace*texCoordSize] = lineList.at(5).toFloat();
			texCoords[face*verticesPerFace*texCoordSize + 1] = lineList.at( 6).toFloat();
			texCoords[face*verticesPerFace*texCoordSize + 2] = lineList.at( 7).toFloat();
			texCoords[face*verticesPerFace*texCoordSize + 3] = lineList.at( 8).toFloat();
			texCoords[face*verticesPerFace*texCoordSize + 4] = lineList.at( 9).toFloat();
			texCoords[face*verticesPerFace*texCoordSize + 5] = lineList.at(10).toFloat();

			if (texNum) {
				texNums[face] = lineList.at(11).toInt();
			}
		}
		//delete[] comments;
		//comments = NULL;
	}
	return true;
}

/// Get next line from the stream ignoring comments
QString Ply2OpenGL::getL(QTextStream &stream)
{
	QString line(stream.readLine());
	if (line.split(" ")[0].compare("comment"))
	{
		return line;
	} else
	{
		return getL(stream);
	}
}

float** Ply2OpenGL::toTwoDimensionalArray(float* in) {
	float** out = new float*[nVertices];
	for (int i = 0; i < nVertices; i++) {
		out[i] = new float[vertexSize];
		for(int j = 0; j < vertexSize; j++) {
			out[i][j] = in[i*vertexSize+j];
		}
	}
	return out;
}