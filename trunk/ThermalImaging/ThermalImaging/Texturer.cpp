#include "Texturer.h"

QImage Texturer::getTexture(Vector3f planeNormal, Vector3f planeTranslation, const vector<vector<float> > &corners, QString& directoryName, int time)
{
	// find the best camera
	
	
	// read list of images corresponding to camera views
	QStringList imgNames = bp.readImageList(QString("Data\\%1\\%1_list.txt").arg(directoryName), true);

	QDir directory = QDir(QString("Data\\%1\\time%2").arg(directoryName).arg(time));
	QStringList files;
	QString fileName ("*.jpg");
	files = directory.entryList(QStringList(fileName),
                                 QDir::Files | QDir::NoSymLinks);
	vector<int> cameraIndices;

	for (int i = 0; i < files.size(); i++) {
		QString s (files.at(i).toLower());
		int camNumber = imgNames.indexOf(s);
		if (camNumber != -1)
			cameraIndices.push_back(camNumber);
	}


	

	// project 4 3D corners into 2D camera image (find additional points if necessary to have a minimum of 5)
	// warp the resulting figure into a rectangle of a good size (use interpolation!)
	vector<int> bestCamInde = findBestCamera(planeNormal, planeTranslation, corners, cameraIndices);
	int bestCamIndex = 0;
	BundleCamera cam = bp.getCamera(bestCamIndex);
	MatrixXf corners3d(3,corners.size());
	for (int coli(0); coli < corners.size(); coli++)
	{
		corners3d.col(coli) << corners.at(coli).at(0), corners.at(coli).at(1), corners.at(coli).at(2);
	}
	MatrixXf cameraCorners = bp.getCameraXYPoints(cam, corners3d);

	// read list of images corresponding to camera views
	//QStringList imgNames = bp.readImageList(QString("Data\\%1\\%1_list.txt").arg(directory));


	QImage im(QString("Data\\%1\\time%2\\%3").arg(directoryName).arg(time).arg(imgNames.at(bestCamIndex)));
	for (int coli(0); coli < cameraCorners.cols(); coli++)
	{
		cameraCorners.col(coli) += Vector2f(im.width()/2, im.height()/2);
	}
	bool outsideCoords = false;
	for (int cornerIndex(0); cornerIndex < cameraCorners.cols(); cornerIndex++)
	{
		if (cameraCorners.col(cornerIndex)[0] < 0 || cameraCorners.col(cornerIndex)[0] > im.width() ||
			cameraCorners.col(cornerIndex)[1] < 0 || cameraCorners.col(cornerIndex)[1] > im.height())
		{
			outsideCoords = true;
		}
		
	}
	return im;
}

vector<int> Texturer::findBestCamera(Vector3f translationVector, Vector3f planeNormal, const vector<vector<float> > &corners, const vector<int>& cameraIndices)
{
	/*float minDist(5.0f);
	// go through each corner and find corresponding real points in the point cloud
	vector<vector<int> > realCorners;
	for (int cornIndex(0); cornIndex < corners.size(); cornIndex++)
	{
		float minDist(0.5f);
		Vector3f c(corners.at(cornIndex).at(0), corners.at(cornIndex).at(1), corners.at(cornIndex).at(2));
		
		vector<int> closePoints;
		for (int pointIndex(0); pointIndex < bp.getNPoints(); pointIndex++)
		{
			//int closestPoint(0);
			float dist = (c - bp.getPoint(pointIndex).getCoords3d()).norm();
			if (dist <= minDist)
			{
				closePoints.push_back(pointIndex);
			}
		}
		realCorners.push_back(closePoints);
	}
	
	int* cameraScore;
	cameraScore = new int[bp.getNCameras()];
	for (int cSI(0); cSI < bp.getNCameras(); cSI++)
	{
		cameraScore[cSI] = 0;
	}
	// then find a camera, which sees all of these real points
	int maxScore = 0;
	vector<int> bestCams;
	for (int cameraIndex(0); cameraIndex < bp.getNCameras(); cameraIndex++)
	{
		
		for (int realCornIndex(0); realCornIndex < realCorners.size(); realCornIndex++)
		{
			bool stop = false;
			for (int closePoint(0); closePoint < realCorners.at(realCornIndex).size() && !stop; closePoint++)
			{
				if (isCameraInViewList(cameraIndex, bp.getPoint(realCorners.at(realCornIndex).at(closePoint)).getViewList()))
				{
					cameraScore[cameraIndex]++;
					stop = true;
					//break;
				}
			}
		}
		if (cameraScore[cameraIndex] > maxScore)
		{
			maxScore = cameraScore[cameraIndex];
			bestCams.clear();
			bestCams.push_back(cameraIndex);
		}
	}

	float minX = +1000.0f;
	float maxX = -1000.0f;
	float minY = +1000.0f;
	float maxY = -1000.0f;
	float minZ = +1000.0f;
	float maxZ = -1000.0f;
	float x,y,z;

	//pop = rpc->getPointCloud();


	for (int i = 0; i < corners.size(); i++) {
		x = corners.at(i).at(0);
		y = corners.at(i).at(1);
		z = corners.at(i).at(2);
		if (x < minX)
			minX = x;
		if (x > maxX)
			 maxX = x;
		if (y < minY)
			 minY = y;
		if (y > maxY)
			 maxY = y;
		if (z < minZ)
			 minZ = z;
		if (z > maxZ)
			 maxZ = z;
	}


	Vector3f centroid((maxX + minX) / 2.0f, (maxY + minY) / 2.0f, (maxZ + minZ) / 2.0f);*/

	float score = 0.0f;
	vector<int> bestCams;

	for (int cameraIndex(0); cameraIndex < cameraIndices.size(); cameraIndex++) {
		// find normal of camera
		BundleCamera cam = bp.getCamera(cameraIndices.at(cameraIndex));
		Vector3f viewDirection = cam.R.row(2).normalized();

		// compare normal with with normal of plane and give score
		Vector3f pN = planeNormal.normalized();
		float dotP = viewDirection.dot(pN);

		// find offset of camera and center of plane
		//Vector3f closestPointOnLine = cam.t + ((centroid-cam.t).dot(viewDirection)) / viewDirection.dot(viewDirection) * viewDirection;
		//float dis = (closestPointOnLine-centroid).norm();
		
		if (0.6 < abs(dotP)) {
			score = abs(dotP);
			bestCams.push_back(cameraIndices.at(cameraIndex));
		}
			

		
	}
	
	return bestCams;
}

bool Texturer::isCameraInViewList(int camera, const vector<BundleView>& viewList)
{
	for (int viewIndex(0); viewIndex < viewList.size(); viewIndex++)
	{
		BundleView v = viewList.at(viewIndex);
		if (v.getCamera() == camera)
		{
			return true;
		}
	}
	return false;
}

//int Texturer::findBestCamera(Vector3f translationVector, Vector3f planeNormal, const vector<vector<float> > &corners)
//{
//	int bestCam(-1);
//	// minimum cost of choosing a camera (depends on the camera direction and position relative to the plane)
//	float minCost(FLT_MAX);
//
//	// get two points on the normal coming out from the centre of the plane
//	Vector3f p1, p2;
//	float maxX = 0, maxY = 0, maxZ = 0;
//	float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
//
//	// find min and maxx coords to determine plane centre
//	for (int cornIndex(0); cornIndex < corners.size(); cornIndex++)
//	{
//		if (corners.at(cornIndex).at(0) > maxX) maxX = corners.at(cornIndex).at(0);
//		if (corners.at(cornIndex).at(0) < minX) minX = corners.at(cornIndex).at(0);
//
//		if (corners.at(cornIndex).at(1) > maxY) maxX = corners.at(cornIndex).at(1);
//		if (corners.at(cornIndex).at(1) < minY) minY = corners.at(cornIndex).at(1);
//
//		if (corners.at(cornIndex).at(2) > maxZ) maxX = corners.at(cornIndex).at(2);
//		if (corners.at(cornIndex).at(2) < minZ) minZ = corners.at(cornIndex).at(2);
//	}
//
//	p1[0] = minX + (minX + maxX)/2;
//	p1[1] = minY + (minY + maxY)/2;
//	p1[2] = minZ + (minZ + maxZ)/2;
//
//	p2[0] = p1[0] + planeNormal[0];
//	p2[1] = p1[1] + planeNormal[1];
//	p2[2] = p1[2] + planeNormal[2];
//
//	for (int camIndex(0); camIndex < bp.getNCameras(); camIndex++)
//	{
//		BundleCamera currentCam = bp.getCamera(camIndex);
//		
//		// how similar is plane normal to the opposite of the viewing direction
//		// (the last row of the camera's rotation matrix is it's viewing direction)
//		float normDist = (planeNormal + currentCam.R.row(2).transpose()).norm();
//
//		// get distance of the camera from the line specified by plane's normal
//		float linDist = (currentCam.t - p1).cross(currentCam.t- p2).norm()/(p2 - p1).norm();
//
//		//TODO: revise the weights
//		if ((0.5 * linDist +  0.5 * normDist) < minCost)
//		{
//			bestCam = camIndex;
//			minCost = 0.5 * linDist +  0.5 * normDist;
//		}
//	}
//	return bestCam;
//}