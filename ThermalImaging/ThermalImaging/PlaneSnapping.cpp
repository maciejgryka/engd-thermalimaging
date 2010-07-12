#include "PlaneSnapping.h"

/* Pseudocode

Go over ever edge of every plane and test if they are parallel in 3D

To test if two lines are parallel we have to test if the components are proportional. Each of the vectors have a x,y and z component, so 
we can simply calculate x2/x1 = x, y2/y1 = y and z2/z1 = z. If x, y and z are approxiamately similar. Here we should set some threshold 
and say the individual values are not allowed to divert by more than 20% or so.

For all the planes that have lines that are approximately similar we first need to test if the lines are those which are on the near side of the plane.
** More detail needed **
If this is the case, we can find the intersection line between the two planes. This is an infinite line and we are going to have to find the point on 
the line that each of the points on the plane projects to, i.e. go along the line between the two points until it hits the intersection line.

If these points is inside the plane we can move both of the closer points on to that plane. Otherwise, we will have to extend both planes so that they 
touch each other.
*/

PlaneSnapping::PlaneSnapping() {
}

PlaneSnapping::~PlaneSnapping() {
}

void PlaneSnapping::addPlane(PlaneInfo* p) {
	// adds a plane to the list of planes, which should be used for snapping
	planes.push_back(p);
}

void PlaneSnapping::snap() {
	// performs the snapping by considering all the planes that have been added.
	// it looks at the angle between different lines and their distance to decide
	// if two planes should be snapped together and how


	// parallelThreshold defines until when two lines are considered to be parallel.
	// this is in terms of the distance between the points on a sphere using normalised vectors
	float parallelThreshold = 0.15f;
	
	// all the edges in the planes taken into account
	vector<Vector3f> edges;
	// the plane number for each of the edges. Should always have the same size as edges
	// this is only to be able to access the correct plane when information from the plane are needed
	vector<int> planeNumber;
	// the corners for every plane
	vector<vector<float>> corners;
	for (int i = 0; i < planes.size(); i++) {
		// get the corners from the plane
		corners = planes.at(i)->getCorners();
		for (int j = 0; j < corners.size(); j++) {
			// this is the direction vector of the edge. the base point can be found using the corners of the plane
			// edge 1's base point of plane 1 will be corner 1 of the plane
			Vector3f v;
			for (int k = 0; k < 3; k++) {
				v(k) = corners[(j+1) % corners.size()][k] - corners[j][k];
			}
			// push the direction vector and the plane number
			edges.push_back(v);
			planeNumber.push_back(i);
		}
	}

	for (int i = 0; i < edges.size(); i++) {
		qDebug() << edges.at(i)[0] << edges.at(i)[1]  << edges.at(i)[2];
	}

	// create 2D array of bools, which says if two edges are parallel
	bool** parallel = new bool*[edges.size()];
	for (int i = 0; i < edges.size(); i++) {
		parallel[i] = new bool[edges.size()];
		for (int j = 0; j < edges.size(); j++) {
			parallel[i][j] = false;
		}
	}

	for (int i = 0; i < edges.size(); i++) {
		for (int j = i+1; j < edges.size(); j++) {
			// find two edges, normalise them and use the parallelThreshold to decide if these two edges can be considered to be parallel
			Vector3f e1 = edges.at(i).normalized();
			Vector3f e2 = edges.at(j).normalized();

			if ((e1(0) - e2(0))*(e1(0) - e2(0))+(e1(1) - e2(1))*(e1(1) - e2(1))+(e1(2) - e2(2))*(e1(2) - e2(2)) < parallelThreshold ||
					(e1(0) + e2(0))*(e1(0) + e2(0))+(e1(1) + e2(1))*(e1(1) + e2(1))+(e1(2) + e2(2))*(e1(2) + e2(2)) < parallelThreshold) {
				parallel[i][j] = true;
				parallel[j][i] = true;
			}

			
		}
	}

	// go through edges and find all parallel ones
	for (int i(0); i < edges.size(); i++)
	{
		for (int j(0); j < edges.size(); j++)
		{
			if (parallel[i][j])
			{
				// get the intersection line
				findIntersectionLine(planeNumber[i], planeNumber[j]);
				// calculate distance from both corners of edge[i] to the intersection line
				// if within a threshold, snap them together (move corners of i and corners of j to the intersection if they're not there (or beyond) yet
			}
		}
	}



	/*MatrixXf** edges = new MatrixXf* [planes.size()];
	MatrixXf m;
	vector<vector<float>> corners;
	for (int i = 0; i < planes.size(); i++) {
		corners = *planes.at(i)->getCorners();
		edges[i] = new MatrixXf(corners.size(),3);
		
		m = *edges[i];

		for (int j = 0; j < corners.size(); j++) {
			for (int k = 0; k < 3; k++) {
				m(j,k) = corners[(j+1) % corners.size()][k] - corners[j][k];
				
			}
		}
	}
	qDebug() << m(0,0) << m(0,1) << m(0,2);
	MatrixXf m2;
	Vector3f edge1, edge2;
	
	for (int p1 = 0; p1 < planes.size(); p1++) {
		qDebug() << m(0,0) << m(0,1) << m(0,2);
		m = *edges[p1];
		//for (int k = 0; k < 3; k++) {
		qDebug() << m(0,0) << m(0,1) << m(0,2);	
		//}
		for (int e1 = 0; e1 < m.cols(); e1++) {
			edge1 = m.row(e1).transpose();
			for (int p2 = 0; p2 < planes.size(); p2++) {
				m2 = *edges[p2];
				for (int e2 = 0; e2 < m2.cols(); e2++) {
					edge2 = m.row(e2).transpose();
				}
			}
		}
	}*/

}

int PlaneSnapping::findIntersectionLine(int a, int b) {

	// finds the intersection line between two planes
	//************************************************
	// it does not return anything yet, but we can pass in references to Vector3f,
	// which can represent the direction and the base of the line
	//************************************************

	// get origins of planes and the normals
	Vector3f o1 = planes.at(a)->getTranslationVector();
	Vector3f n1 = planes.at(a)->getNormal();
	Vector3f o2 = planes.at(b)->getTranslationVector();
	Vector3f n2 = planes.at(b)->getNormal();

	// get direction of the intersection line
	Vector3f l = n1.cross(n2).normalized();
	Vector3f v;

	// test if planes are parallel or are the same plane
	if (l.norm() == 0.0f) {// parallel
		v = o1 - o2;
		if (n1.dot(v) == 0) {
			return 1; // coincide
		} else {
			return 0; // parallel, but disjoint
		}
	}

	// set the dimension with maximum magnitude to 0 and evaluate the others
	int max = 0;
	if (l(0) > l(1) && l(0) > l(2))
		max = 0;
	else if (l(1) > l(2))
		max = 1;
	else
		max = 2;

	float d1 = -n1.dot(o1);
	float d2 = -n2.dot(o2);

	// calculate base point
	switch(max) {
		case 0:
			v(0) = 0.0f;
			v(1) = (d2 * n1(2) - d1 * n2(2)) / l(0);
			v(2) = (d1 * n2(1) - d2 * n1(1)) / l(0);
			break;
		case 1:
			v(0) = (d1 * n2(2) - d2 * n1(2)) / l(1);
			v(1) = 0.0f;
			v(2) = (d2 * n1(0) - d1 * n2(0)) / l(1);
			break;
		case 2:
			v(0) = (d2 * n1(1) - d1 * n2(1)) / l(2);
			v(1) = (d1 * n2(0) - d2 * n1(0)) / l(2);
			v(2) = 0.0f;
			break;

	}

	return 2;

}

// Find the distance from point x0 to line defined by x1 and x2
// formula taken from http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
float PlaneSnapping::pointLineDistance(Vector3f x0, Vector3f x1, Vector3f x2)
{
	return (x0 - x1).cross(x0 - x2).norm()/(x2 - x1).norm();
}