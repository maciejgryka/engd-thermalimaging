#include "BundleParser.h"

MatrixXf BundleParser::getCameraXYPoints(int cameraNo, MatrixXf points3d)
{
	return getCameraXYPoints(cameras[cameraNo], points3d);
}

// taken from http://phototour.cs.washington.edu/bundler/bundler-v0.4-manual.html
MatrixXf BundleParser::getCameraXYPoints(BundleCamera camera, MatrixXf points3d)
{
	MatrixXf points2d(2, points3d.cols());
	
	for (int colIndex(0); colIndex < points3d.cols(); colIndex++)
	{
		Vector3f p = camera.R * points3d.col(colIndex) + camera.t;
		p[0] = -p[0]/p[2];
		p[1] = p[1]/p[2];

		points2d.col(colIndex)[0] = camera.focal * undistort(Vector2f(p[0], p[1]), camera.radialDist[0], camera.radialDist[1]) * p[0];
		points2d.col(colIndex)[1] = camera.focal * undistort(Vector2f(p[0], p[1]), camera.radialDist[0], camera.radialDist[1]) * p[1];
	}

	return points2d;
}

// Reads bundle file and saves values
// if confused refer to http://phototour.cs.washington.edu/bundler/bundler-v0.4-manual.html
bool BundleParser::readFile(QString filename)
{
	QFile bundleFile(filename);

	if (bundleFile.open(QIODevice::ReadOnly))
	{
		// Read the file
		QTextStream stream(&bundleFile);

		QStringList text;
		QString textAsLine = stream.readAll();
		text = textAsLine.split("\r\n");
		if (text.size() == 1) {
			text = textAsLine.split("\n");
		}
		int *comments = new int[text.size()];

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
		}
		int position = 0;

		if (!text[position].startsWith("# Bundle file"))
		{
			std::cout << "Not a valid bundle file." << std::endl;
			return 0;
		}

		position += comments[position];

		nCameras = text[position].split(" ")[0].toInt();
		nPoints = text[position].split(" ")[1].toInt();

		cameras = new BundleCamera[nCameras];
		// iterate over cameras
		for (int cam = 0; cam < nCameras; cam++)
		{
			position += comments[position];
			QStringList lineList = text[position].split(" ");
	
			cameras[cam].focal = lineList.at(0).toFloat();
			cameras[cam].radialDist[0] = lineList.at(1).toFloat();
			cameras[cam].radialDist[1] = lineList.at(2).toFloat();

			for (int rowR = 0; rowR < 3; rowR++)
			{
				position += comments[position];
				lineList = text[position].split(" ");
				cameras[cam].R.row(rowR) << lineList.at(0).toFloat(), lineList.at(1).toFloat(), lineList.at(2).toFloat();
			}

			position += comments[position];
			lineList = text[position].split(" ");
			cameras[cam].t << lineList.at(0).toFloat(), lineList.at(1).toFloat(), lineList.at(2).toFloat();
		}

		points = new BundlePoint[nPoints];
		// iterate over points
		for (int pointIndex(0); pointIndex < nPoints; pointIndex++)
		{
			// read position
			position += comments[position];
			QStringList lineList = text[position].split(" ");
			points[pointIndex].setCoords3d(Eigen::Vector3f(lineList.at(0).toFloat(), lineList.at(1).toFloat(), lineList.at(2).toFloat()));

			// read color
			position += comments[position];
			lineList = text[position].split(" ");
			points[pointIndex].setColor(Eigen::Vector3f(lineList.at(0).toFloat(), lineList.at(1).toFloat(), lineList.at(2).toFloat()));

			// read view list
			position += comments[position];
			lineList = text[position].split(" ");
			int nViews = lineList.at(0).toInt();
			for (int viewIndex(1); viewIndex < nViews; viewIndex++)
			{
				BundleView bv;
				bv.setCamera(lineList.at(viewIndex).toInt());
				bv.setKey(lineList.at(viewIndex+1).toInt());
				bv.setCoordsXY(lineList.at(viewIndex+2).toFloat(), lineList.at(viewIndex+3).toFloat());
				
				points[pointIndex].addView(bv);
			}
		}
	}
	return true;
}

QStringList BundleParser::readImageList(QString filename)
{
	QFile listFile(filename);
	QStringList imgPaths;
	QStringList imgNames;

	if (listFile.open(QIODevice::ReadOnly))
	{
		QTextStream stream(&listFile);
		QString line;

		line = getL(stream);
		while (line.compare(""))
		{
			imgPaths << line.split(" ")[0];
			line = getL(stream);
		}

		for (int pathIndex(0); pathIndex < imgPaths.size(); pathIndex++)
		{
			QStringList splitPath = imgPaths.at(pathIndex).split("/");
			imgNames << splitPath.at(splitPath.size()-1);
		}
	}
	return imgNames;
}

/// Computes a scaling factor to undo the radial distortion
float BundleParser::undistort(Vector2f p, float k1, float k2)
{
	return k1*p.norm() + k2*p.norm()*p.norm();
}

/// Get next line from the stream ignoring comments
QString BundleParser::getL(QTextStream &stream)
{
	QString line(stream.readLine());
	if (line.split(" ")[0].compare("#"))
	{
		return line;
	} else
	{
		return getL(stream);
	}
}