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

		// y = a1*x + c1
		float a1, c1;

		a1 = (p0[1] - p1[1])/(p0[0] - p1[0]);

		// if a1 is finite (line is non-vertical) we have y = a1*x + c1
		// otherwise the line is vertical and c1 stores its constant x value
		if (isFiniteNumber(a1))
		{
			c1 = p0[1] - a1*p0[0];
		} else
		{
			c1 = p0[0];
		}

		vector<float> tempLine;
		tempLine.push_back(a1);
		tempLine.push_back(c1);
		lineCoeffs.push_back(tempLine);

		// if secondEdge is smaller or equal firstEdge it's either the same edge, or one we've already tested
		for (int secondEdge(firstEdge+2); secondEdge < bestPoints.size()-1; secondEdge += 2)
		{
			//if (secondEdge <= firstEdge) continue;

			p2[0] = xBorder[bestPoints[secondEdge]];
			p2[1] = zBorder[bestPoints[secondEdge]];

			p3[0] = xBorder[bestPoints[secondEdge+1]];
			p3[1] = zBorder[bestPoints[secondEdge+1]];

			float a2, c2;

			a2 = (p2[1] - p3[1])/(p2[0] - p3[0]);

			if (a2 == a1)		// if the slopes are the same, the lines are parallel - don't bother
			{
				continue;
			}

			// if a2 is finite (line is non-vertical) we have y = a2*x + c2
			// otherwise the line is vertical and c2 stores its constant x value
			if (isFiniteNumber(a2))
			{
				c2 = p2[1] - a2*p2[0];
			} else
			{
				c2 = p2[0];
			}

			float interX;
			float interZ;

			if (isFiniteNumber(a1) && isFiniteNumber(a2))	// normal (non-vertical lines) case
			{
				// intersection point of the two lines
				interX = (c2 - c1)/(a1 - a2);
				interZ = a1*interX + c1;
			}
			else if (!isFiniteNumber(a1) && !isFiniteNumber(a2))	// if both lines are vertical, don't bother
			{														// shouldn't ever get to this point, as we're comparing a1 and a2 before...
				continue;
			}
			else if (!isFiniteNumber(a1))					// if line1 is vertical and line2 isn't, calculate when line2 crosses line1
			{
				interX = c1;
				interZ = a2*interX + c2;
			}
			else if (!isFiniteNumber(a2))					// if line2 is vertical and line1 isn't, calculate when line1 crosses line2
			{
				interX = c2;
				interZ = a1*interX + c1;
			}

			// check if the intersection point is within boundaries
			if (interX < boundaries[0] || interX > boundaries[2] || interZ < boundaries[3] || interZ > boundaries[1]) continue;
			
			// check whether we already have a very similar point
			bool pExists(false);
			for (int ci(0); ci < corners.size(); ci++)
			{
				if (sqrt((corners.at(ci).at(0) - interX) * (corners.at(ci).at(0) - interX) + (corners.at(ci).at(2) - interZ) * (corners.at(ci).at(2) - interZ)) < 7.0f) pExists = true;
			}

			if (pExists) continue;

			// save the point
			vector<float> tempCorners;

			tempCorners.push_back(interX);
			tempCorners.push_back(0.0f);
			tempCorners.push_back(interZ);

			corners.push_back(tempCorners);
		}
	}

	if (corners.size() == 0)
		return false;

	// Now that we have all the corners, we need to put ther in a nice order for later drawing.
	int *queue;										// holds corner indices in the right order
	queue = new int[corners.size()];
	int qPos = 0;

	int lastLineIndex = -1;
	bool checkedAll = false;
	int corn(0);
	while (!checkedAll)
	{
		// find right edge line
		int lineIndex = findLineCoeffs(corn, lastLineIndex);	// find line the explains this point and is different to the last line
		
		if (lineIndex == -1)
			break;

		// find another point on this line
		// ASSUMPTIONS: only neighbouring (edge-sharing) points are colinear; 
		//				each edge contains exactly 2 points
		int pointIndex = findPointOnLine(lineIndex, corn);

		// add the right point index to the queue
		queue[qPos] = pointIndex;
		qPos++;
		
		lastLineIndex = lineIndex;
		corn = pointIndex;
		
		//if (pointIndex == 0) checkedAll = true;
		if (qPos == corners.size()) checkedAll = true;
	}
	if (!checkedAll) return false;

	// rearrange the corners vector
	vector<vector<float> > copyCorners = corners;
	for (corn = 0; corn < corners.size(); corn++)
	{
		corners.at(corn) = copyCorners.at(queue[corn]);
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

	// find two random points iterations times
	for (int iter = 0; iter < iterations; iter++)
	{
		int inlier = 0;
		vector<int> iterPointsUsed;

		pos1 = rand() % xBorder.size();
		pos2 = rand() % zBorder.size();

		while (pointList[pos1] == 1 || pointList[pos2] == 1 || pos1 == pos2) {
			pos1 = rand() % xBorder.size();
			pos2 = rand() % zBorder.size();
		}

		P1[0] = xBorder[pos1];
		P1[1] = zBorder[pos1];

		P2[0] = xBorder[pos2];
		P2[1] = zBorder[pos2];

		// for every point in the set compute the distance
		for (int borderPoint = 0; borderPoint < xBorder.size(); borderPoint++)
		{
			// omit any points already used for one of the previous lines found
			if (pointList[borderPoint] == 1)
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
		pointList[currentPointsUsed.at(cpu)] = 1;
	}
	this->bestPoints.push_back(currentBestPoints.at(0));
	this->bestPoints.push_back(currentBestPoints.at(1));
	
	return true;
}

/// Finds a line in lineCoeffs that explains the given point and is different than the last line
int RansacPlaneEdge::findLineCoeffs(int pointIndex, int lastLineIndex)
{
	for (int li(0); li < lineCoeffs.size(); li++)
	{

		if (!isFiniteNumber(lineCoeffs.at(li).at(0)))	// if a is infinite, the line is vertical (x = c), so any point with x = c will be on it
		{
			if (li != lastLineIndex && corners.at(pointIndex).at(0) == lineCoeffs.at(li).at(1))
			{
				return li;
			}
		} else if (abs(corners.at(pointIndex).at(2) - lineCoeffs.at(li).at(0)*corners.at(pointIndex).at(0) - lineCoeffs.at(li).at(1)) < 0.01)
		{
			if (li != lastLineIndex) 
			{
				return li;
			}
		}
	}
	return -1;
}

/// Find a corner that lies on the given line and is different than the previous corner
int RansacPlaneEdge::findPointOnLine(int lineIndex, int currentPointIndex)
{
	for (int pi(0); pi < corners.size(); pi++)
	{
		//if (pi == 4)
		//	return 0;
		
		if (!isFiniteNumber(lineCoeffs.at(lineIndex).at(0)))	// if a is infinite, the line is vertical, so we only need to check if point.x == c
		{
			if (pi != currentPointIndex && corners.at(pi).at(0) == lineCoeffs.at(lineIndex).at(1))
			{
				return pi;
			}
		} else if (abs(corners.at(pi).at(2) - lineCoeffs.at(lineIndex).at(0) * corners.at(pi).at(0) - lineCoeffs.at(lineIndex).at(1)) < 0.01)
		{
			if (pi != currentPointIndex) 
			{
				return pi;
			}
		}
	}
	return -1;
}
			