#include "RansacPlaneEdge.h"

bool RansacPlaneEdge::findEdges()
{
	// find all fitting edges
	while (xBorder.size() - pointsUsed.size() > int(xBorder.size() * percentOfChillPoints))
	{
		findBestEdge(pointsUsed);
	}

	// extend and connect found seqments
	for (int firstEdge(0); firstEdge < bestPoints.size()-1; firstEdge += 2)
	{
		float p0[2], p1[2], p2[2], p3[2];
		p0[0] = xBorder[bestPoints[firstEdge]];
		p0[1] = zBorder[bestPoints[firstEdge]];

		p1[0] = xBorder[bestPoints[firstEdge+1]];
		p1[1] = zBorder[bestPoints[firstEdge+1]];

		// y = a1*x + b1
		float a1, b1;

		bool vert1 = (p0[0] == p1[0] ? true : false);	// vert1 is true if line1 is vertical (a1 = infinity)

		if (!vert1)
		{
			a1 = (p0[1] - p1[1])/(p0[0] - p1[0]);
			b1 = p0[1] - a1*p0[0];
		}

		for (int secondEdge(0); secondEdge < bestPoints.size()-1; secondEdge += 2)
		{
			// if secondEdge is smaller or equal firstEdge it's either the same edge, or one we've already tested
			if (secondEdge <= firstEdge) continue;

			p2[0] = xBorder[bestPoints[secondEdge]];
			p2[1] = zBorder[bestPoints[secondEdge]];

			p3[0] = xBorder[bestPoints[secondEdge+1]];
			p3[1] = zBorder[bestPoints[secondEdge+1]];

			bool vert2 = (p2[0] == p3[0] ? true : false);	// vert2 is true if line2 is vertical (a2 = infinity)
			if (vert1 && vert2) continue;	// if vert1 and vert2 are true, then lines are parallel - don't bother
			
			// y = a2*x + b2
			float a2, b2;
			a2 = (p2[1] - p3[1])/(p2[0] - p3[0]);
			b2 = p2[1] - a2*p2[0];

			if (a1 == a2) continue;	// two lines are parallel - don't bother

			float interX;
			float interZ;

			if (vert1)				// if line1 is vertical and line2 isn't, simply calculate when line2 crosses line1
			{
				interX = p0[0];
				interZ = a2*interX + b2;
			} else
			{
				// intersection point of the two lines
				interX = (b2 - b1)/(a1 - a2);
				interZ = a1*interX + b1;
			}

			// check if the intersection point is within boundaries
			if (interX < boundaries[0] || interX > boundaries[2] || interZ < boundaries[3] || interZ > boundaries[1]) continue;

			vector<float> tempCorners;
			//vector<vector<float>> ttcorners;

			tempCorners.push_back(interX);
			tempCorners.push_back(0.0f);
			tempCorners.push_back(interZ);

			//ttcorners.push_back(tempCorners);
			corners.push_back(tempCorners);
		}
	}
	return true;
}

bool RansacPlaneEdge::findBestEdge(vector<int> &pointsUsed)
{
	int pos1, pos2;
	float P1[2], P2[2];

	int maxInliers = 0;
	vector<int> currentPointsUsed;
	vector<int> currentBestPoints;
	currentBestPoints.push_back(0);
	currentBestPoints.push_back(0);

	// find two random points int(iterations) times
	for (int iter = 0; iter < iterations; iter++)
	{
		int inlier = 0;
		vector<int> iterPointsUsed;

		pos1 = rand() % xBorder.size();
		pos2 = rand() % zBorder.size();

		P1[0] = xBorder[pos1];
		P1[1] = zBorder[pos1];

		P2[0] = xBorder[pos2];
		P2[1] = zBorder[pos2];

		// for every point in the set compute the distance
		for (int borderPoint = 0; borderPoint < xBorder.size(); borderPoint++)
		{
			// omit any points already used for one of the previous lines found
			if (!pointsUsed.empty() && (find(pointsUsed.begin(), pointsUsed.end(), borderPoint) != pointsUsed.end()))
			{
				continue;
			}
			float P0[2];
			P0[0] = xBorder[borderPoint];
			P0[1] = zBorder[borderPoint];

			// if the distance if within the threshold, add it to the inliers
			float dist = abs((P2[0] - P1[0])*(P1[1] - P0[1]) - (P1[0] - P0[0])*(P2[1] - P1[1]))/sqrt((P2[0] - P1[0])*(P2[0] - P1[0]) + (P2[1] - P1[1])*(P2[1] - P1[1]));
			if (dist < inlierDistance)
			{
				inlier++;
				iterPointsUsed.push_back(borderPoint);
			}
		}

		// if the current two points represent the best line so far (biggest number of inliers), save them
		if (inlier > maxInliers)
		{
			maxInliers = inlier;

			currentBestPoints.at(0) = pos1;
			currentBestPoints.at(1) = pos2;
			currentPointsUsed = iterPointsUsed;
		}
	}

	for (int cpu = 0; cpu < currentPointsUsed.size(); cpu++)
	{
		this->pointsUsed.push_back(currentPointsUsed.at(cpu));
	}
	this->bestPoints.push_back(currentBestPoints.at(0));
	this->bestPoints.push_back(currentBestPoints.at(1));
	
	return false;
}
