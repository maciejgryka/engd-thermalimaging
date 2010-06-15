#include "Ply2OpenGL.h"


bool Ply2OpenGL::readPlyFile(QString filename)
{
	QFile plyFile(filename);


	if (plyFile.open(QIODevice::ReadOnly))
	{
		// Read the file
		QTextStream stream(&plyFile);
		QString line;
		QStringList header;

		bool inHeader = true;
		//line = stream.readLine();
		line = getL(stream);

		if (line.compare("ply"))
		{
			std::cout << "Not a valid .ply file." << std::endl;
			return 0;
		}

		line = getL(stream);

		if (line.compare("format ascii 1.0"))
		{
			std::cout << "Wrong format: only ascii 1.0 supported." << std::endl;
			return 0;
		}

		// analyse the PLY header
		while (inHeader)
		{
			line = getL(stream);

			if (!line.compare("end_header"))
			{
				inHeader = false;
			}
			QStringList lineList = line.split(" ");
			// assign memory to store all the information
			if (!lineList.at(0).compare("element") && !lineList.at(1).compare("vertex"))
			{
				nVertices = lineList.at(2).toInt();
				vertices =	new float[nVertices * vertexSize];
				//texCoords = new float[nVertices * texCoordSize];
				//colors =	new float[nVertices * colorSize];
				//normals =	new float[nVertices * normalSize];
			} 
			else if (!lineList.at(0).compare("element") && !lineList.at(1).compare("face"))
			{
				nFaces = lineList.at(2).toInt();
				indices = new int[nFaces * verticesPerFace];
				texCoords = new float[nFaces * verticesPerFace * texCoordSize];
			}
			header += line;
		}

		// assign all the vertex values
		for (int vert = 0; vert < nVertices; vert++)
		{
			line = getL(stream);
			QStringList lineList = line.split(" ");
			// write vertices
			vertices[vert*vertexSize]	  = lineList.at(0).toFloat()/5;
			vertices[vert*vertexSize + 1] = lineList.at(1).toFloat()/5;
			vertices[vert*vertexSize + 2] = lineList.at(2).toFloat()/5;

			//// write texture coordinates
			//texCoords[vert*texCoordSize] = lineList.at(3).toFloat();
			//texCoords[vert*texCoordSize + 1] = lineList.at(4).toFloat();
		}

		// assign all the face values
		for (int face = 0; face < nFaces; face++)
		{
			line = getL(stream);
			QStringList lineList = line.split(" ");
		
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
		}
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