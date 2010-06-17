#include "RansacPlaneEdge.h"

bool RansacPlaneEdge::findBestEdge()
{
	int pos1, pos2;
	float P1[2], P2[2];

	int maxInliers = 0;

	for (int iter = 0; iter < iterations; iter++)
	{
		int inlier = 0;
		pos1 = rand() % xBorder.size();
		pos2 = rand() % zBorder.size();

		P1[0] = xBorder[pos1];
		P1[1] = zBorder[pos1];

		P2[0] = xBorder[pos2];
		P2[1] = zBorder[pos2];

		for (int borderPoint = 0; borderPoint < xBorder.size(); borderPoint++)
		{
			float P0[2];
			P0[0] = xBorder[borderPoint];
			P0[1] = zBorder[borderPoint];

			float dist = abs((P2[0] - P1[0])*(P1[1] - P0[1]) - (P1[0] - P0[0])*(P2[1] - P1[1]))/sqrt((P2[0] - P1[0])*(P2[0] - P1[0]) + (P2[1] - P1[1])*(P2[1] - P1[1]));
			if (dist < inlierDistance)
			{
				inlier++;

			}
		}
		if (inlier > maxInliers)
		{
			maxInliers = inlier;
			bestPoints[0] = pos1;
			bestPoints[1] = pos2;
		}
	}
	
	return false;
}
