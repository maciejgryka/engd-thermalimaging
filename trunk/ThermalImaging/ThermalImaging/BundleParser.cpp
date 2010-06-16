#include "BundleParser.h"

MatrixXf BundleParser::getCameraXYPoints(int cameraNo, MatrixXf points3d)
{
	return points3d;
}

bool BundleParser::readFile(QString filename)
{
	filename = "Data//box7//bundler//bundle//bundle.out";

	QFile bundleFile(filename);

	if (bundleFile.open(QIODevice::ReadOnly))
	{
		QTextStream stream(&bundleFile);
		QString line;

		line = getL(stream);

		nCameras = line.split(" ")[0].toInt();
		nPoints = line.split(" ")[1].toInt();

		cameras = new BundleCamera[nCameras];
		for (int cam = 0; cam < nCameras; cam++)
		{
			line = getL(stream);
			QStringList lineList = line.split(" ");
			
			cameras[cam].focal = lineList.at(0).toFloat();
			cameras[cam].radialDist[0] = lineList.at(1).toFloat();
			cameras[cam].radialDist[1] = lineList.at(2).toFloat();

			for (int colR = 0; colR < 3; colR++)
			{
				line = getL(stream);
				lineList = line.split(" ");
				cameras[cam].R.col(colR) << lineList.at(0).toFloat(), lineList.at(1).toFloat(), lineList.at(2).toFloat();
			}

			line = getL(stream);
			lineList = line.split(" ");
			cameras[cam].t << lineList.at(0).toFloat(), lineList.at(1).toFloat(), lineList.at(2).toFloat();
		}
	}
	return true;
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