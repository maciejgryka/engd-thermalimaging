#include "Texturer.h"

QImage Texturer::getTexture(Vector3f planeNormal, Vector3f planeTranslation, const vector<vector<float> > &corners)
{
	// find the best camera
	

	// project 4 3D corners into 2D camera image (find additional points if necessary to have a minimum of 5)
	// warp the resulting figure into a rectangle of a good size (use interpolation!)
	int bestCamIndex = findBestCamera(planeNormal, planeTranslation, corners);
	
	BundleCamera cam = bp.getCamera(bestCamIndex);
	MatrixXf corners3d(3,5);
	for (int coli(0); coli < corners.size(); coli++)
	{
		corners3d.col(coli) << corners.at(coli).at(0), corners.at(coli).at(1), corners.at(coli).at(2);
	}
	MatrixXf cameraCorners = bp.getCameraXYPoints(cam, corners3d);

	// read list of images corresponding to camera views
	QStringList imgNames = bp.readImageList("Data\\list.txt");


	QImage im("images\\box7\\" + imgNames.at(bestCamIndex));
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

int Texturer::findBestCamera(Vector3f translationVector, Vector3f planeNormal, const vector<vector<float> > &corners)
{
	// go through each corner and find corresponding real points in the point cloud
	vector<int> realCorners;
	for (int cornIndex(0); cornIndex < corners.size(); cornIndex++)
	{
		float minDist(FLT_MAX);
		Vector3f c(corners.at(cornIndex).at(0), corners.at(cornIndex).at(1), corners.at(cornIndex).at(2));
		int closestPoint(0);
		
		for (int pointIndex(0); pointIndex < bp.getNPoints(); pointIndex++)
		{
			float dist = (c - bp.getPoint(pointIndex).getCoords3d()).norm();
			if (dist < minDist)
			{
				minDist = dist;
				closestPoint = pointIndex;
			}
		}
		realCorners.push_back(closestPoint);
	}
	
	// then find a camera, which sees all of these real points
	for (int cameraIndex(0); cameraIndex < bp.getNCameras(); cameraIndex++)
	{
		
	}
	return 0;
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