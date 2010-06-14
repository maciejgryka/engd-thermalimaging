#include "ply2opengl.h"

Ply2OpenGL::Ply2OpenGL() {}

Ply2OpenGL::~Ply2OpenGL() {
	free(points);
	free(normals);
	free(colors);
}

void Ply2OpenGL::setUp() {
	setFile();
	createFiles();
}

void Ply2OpenGL::setFile() {
	inputFile = QFileDialog::getOpenFileName(0,
     QString("Open PLY Pointcloud"), QString("/home"), QString("PLY Files (*.ply)"));;
}

float* Ply2OpenGL::getPoints() {
	return points;
}

float* Ply2OpenGL::getNormals() {
	return normals;
}

float* Ply2OpenGL::getColors() {
	return colors;
}

int Ply2OpenGL::getNumberOfPoints() {
	return nPoints;
}

float** Ply2OpenGL::convert2Matrix(float* matrix) {
	float** matrix2 = new float*[nPoints];
	for (int i = 0; i < nPoints; i++) {
		matrix2[i] = new float[3];
		matrix2[i][0] = matrix[3*i];
		matrix2[i][1] = matrix[3*i+1];
		matrix2[i][2] = matrix[3*i+2];
	}
	return matrix2;
}

int Ply2OpenGL::createFiles() {

    QFile plyFile(inputFile);

	if (plyFile.open(QIODevice::ReadOnly)) 
	{
		// Read the file
		QTextStream stream(&plyFile);
		QString line;
		QStringList header;

		bool inHeader = true;
		line = stream.readLine();
		if (line.compare("ply"))
		{
			std::cout << "Not a valid .ply file." << std::endl;
			return 0;
		}

		line = stream.readLine();
		if (line.compare("format ascii 1.0"))
		{
			std::cout << "Wrong format: only ascii 1.0 supported." << std::endl;
			return 0;
		}

		while (inHeader)
		{
			line = stream.readLine();
			if (!line.compare("end_header"))
			{
				inHeader = false;
			}
			header += line;
		}

		//QString nP = header[0].split(" ")[2];
		nPoints = header[0].split(" ")[2].toInt();

		points = new float[nPoints*3];
		normals = new float[nPoints*3];
		colors = new float[nPoints*3];

		QStringList pointData;
		int lineCount = 1;
        while (!stream.atEnd()) 
		{
            line = stream.readLine();
			pointData = line.split(" ");

			// save point data
			points[(lineCount-1)*3]     = pointData.at(0).toFloat();
			points[(lineCount-1)*3 + 1] = pointData.at(1).toFloat();
			points[(lineCount-1)*3 + 2] = pointData.at(2).toFloat();

			// save normals data
			normals[(lineCount-1)*3]     = pointData.at(3).toFloat();
			normals[(lineCount-1)*3 + 1] = pointData.at(4).toFloat();
			normals[(lineCount-1)*3 + 2] = pointData.at(5).toFloat();

			// save color data
			colors[(lineCount-1)*3]     = pointData.at(6).toFloat() /256.0f;
			colors[(lineCount-1)*3 + 1] = pointData.at(7).toFloat() /256.0f;
			colors[(lineCount-1)*3 + 2] = pointData.at(8).toFloat() /256.0f;
			
			lineCount++;
        }
        plyFile.close();

		// Write output files

		// write points.txt
		QFile pointsFile("points.txt");
		if (pointsFile.open(QIODevice::WriteOnly))
		{
			QTextStream outStream(&pointsFile);
			for (int i = 0; i < nPoints; i++)
			{
				outStream << points[i*3] << " " << points[i*3 + 1] << " " << points[i*3 + 2] << "\n";
			}
			pointsFile.close();
		} else
		{
			std::cout << "Cannot write points.txt. Terminating... " << std::endl;
			
			delete points;
			delete normals;
			delete colors;
			return 0;
		}

		// write normals.txt
		QFile normalsFile("normals.txt");
		if (normalsFile.open(QIODevice::WriteOnly))
		{
			QTextStream outStream(&normalsFile);
			for (int i = 0; i < nPoints; i++)
			{
				outStream << normals[i*3] << " " << normals[i*3 + 1] << " " << normals[i*3 + 2] << "\n";
			}
			normalsFile.close();
		} else
		{
			std::cout << "Cannot write normals.txt. Terminating... " << std::endl;
			
			delete points;
			delete normals;
			delete colors;
			return 0;
		}

		// write colors.txt
		QFile colorsFile("colors.txt");
		if (colorsFile.open(QIODevice::WriteOnly))
		{
			QTextStream outStream(&colorsFile);
			for (int i = 0; i < nPoints; i++)
			{
				outStream << colors[i*3] << " " << colors[i*3 + 1] << " " << colors[i*3 + 2] << "\n";
			}
			colorsFile.close();
		} else
		{
			std::cout << "Cannot write colors.txt. Terminating... " << std::endl;
			
			delete points;
			delete normals;
			delete colors;
			return 0;
		}
    }
	
	return 1;
}
