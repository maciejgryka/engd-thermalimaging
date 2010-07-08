#include "PlaneInfo.h"

PlaneInfo::PlaneInfo(){
	color = NULL;
	boundaries = NULL;
	pointsUsed = NULL;
	quad = NULL;
	grid = NULL;
	numberOfPoints = -1;
	pointsOnPlane = NULL;

	normalSet = false;
	translationSet = false;
	rotationSet = false;
}
PlaneInfo::~PlaneInfo(){}

Vector3f PlaneInfo::getNormal(){
	return normal;
}

Vector3f PlaneInfo::getTranslationVector(){
	return translationVector;
}

Matrix3f PlaneInfo::getRotationMatrix(){
	return rotationMatrix;
}

float* PlaneInfo::getColor(){
	return color;
}

float* PlaneInfo::getBoundaries(){
	return boundaries;
}

int* PlaneInfo::getPointsUsed(){
	return pointsUsed;
}

int PlaneInfo::getPointNumber(){
	return numberOfPoints;
}

Quad* PlaneInfo::getQuad() {
	return quad;
}

Grid* PlaneInfo::getGrid() {
	return grid;
}

float** PlaneInfo::getPointsOnPlane() {
	return pointsOnPlane;
}

vector<float>& PlaneInfo::getXBorder() {
	return xBorder;
}

vector<float>& PlaneInfo::getYBorder() {
	return yBorder;
}

vector<float>& PlaneInfo::getZBorder() {
	return zBorder;
}

vector<vector<float> >& PlaneInfo::getCorners() {
	return corners;
}

void PlaneInfo::setNormal(Vector3f n) {
	normalSet = true;
	normal = n;
}

void PlaneInfo::setTranslationVector(Vector3f v){
	translationSet = true;
	translationVector = v;
}

void PlaneInfo::setRotationMatrix(Matrix3f m){
	rotationSet = true;
	rotationMatrix = m;
}

void PlaneInfo::setColor(float* c){
	color = c;
}

void PlaneInfo::setBoundaries(float* b){
	boundaries = b;
}

void PlaneInfo::setPointsUsed(int* ps){
	pointsUsed = ps;
}

void PlaneInfo::setPointNumber(int n){
	numberOfPoints = n;
}

void PlaneInfo::setQuad(Quad* q) {
	quad = q;
}

void PlaneInfo::setGrid(Grid* g) {
	grid = g;
}

void PlaneInfo::setPointsOnPlane(float** p) {
	pointsOnPlane = p;
}

void PlaneInfo::setXBorder(const vector<float>& b) {
	xBorder = b;
}

void PlaneInfo::setYBorder(const vector<float>& b) {
	yBorder = b;
}

void PlaneInfo::setZBorder(const vector<float>& b) {
	zBorder = b;
}

void PlaneInfo::setCorners(const vector<vector<float> >& c) {
	corners = c;
}

vector<vector<float> >& PlaneInfo::unrotateCorners(vector<vector<float> >& corners)
{
	rotationMatrix = rotationMatrix.inverse();
	for (int ci(0); ci < corners.size(); ci++)
	{
		Vector3f corner(corners.at(ci).at(0), corners.at(ci).at(1), corners.at(ci).at(2));
		corner = rotationMatrix * corner;
		corner += translationVector;
		corners.at(ci).at(0) = corner[0];
		corners.at(ci).at(1) = corner[1];
		corners.at(ci).at(2) = corner[2];
	}
	rotationMatrix = rotationMatrix.inverse();
	return corners;
}

void PlaneInfo::writePlane(QString fileName) {
	// writes the plane information to the file with the name that is passed into the method
	// the information are set in the following way
	// <plane number 1> <number of points on plane 1>
	// <information included 6 (set to 1 if included, 0 otherwise) e.g. when everything is included except the border this will be set to 1 1 1 1 1 0
	// <normal 3>
	// <translation vector 3>
	// <rotation matrix 9> (from left to write and top to bottom)
	// <color of the plane (float) 3>
	// <indices of points used on the plane (as many as there are points on the plane)>
	// <number of elements on the border 1 (b)>
	// <xBorder (b)>
	// <yBorder (b)>
	// <zBorder (b)>
	// sometimes not all the information are saved in that case 

	QFile file (fileName);
	// try to open file, return if failed
	if (!file.open(QIODevice::WriteOnly)) {
		return;
	}
	QTextStream ts (&file);
	ts << "0" << " " << numberOfPoints << "\r\n";
	ts << (normalSet) << " " << (translationSet) << " " << (rotationSet) << " " 
		<< (color != NULL) << " " << (pointsUsed != NULL) << " " << (xBorder.size() != 0)
		<< " " << (corners.size() != 0) << " " << (boundaries != NULL) << "\r\n";
	if (normalSet) {
		ts << normal(0) << " " << normal(1) << " " << normal(2) << "\r\n";
	}
	if (translationSet) {
		ts << translationVector(0) << " " << translationVector(1) << " " << translationVector(2) << "\r\n";
	}
	if (rotationSet) {
		ts << rotationMatrix(0,0) << " " << rotationMatrix(0,1) << " " << rotationMatrix(0,2) << " "
			<< rotationMatrix(1,0) << " " << rotationMatrix(1,1) << " " << rotationMatrix(1,2) << " "
			<< rotationMatrix(2,0) << " " << rotationMatrix(2,1) << " " << rotationMatrix(2,2) << "\r\n";
	}
	if (color != NULL) {
		ts << color[0] << " " << color[1] << " " << color[2] << "\r\n";
	}
	if (pointsUsed != NULL) {
		for (int i = 0; i < numberOfPoints; i++) {
			ts << pointsUsed[i] << " ";
		}
		ts << "\r\n";
	}
	if (xBorder.size() != 0) {
		ts << xBorder.size() << "\r\n";
		for (int i = 0; i < xBorder.size(); i++) {
			ts << xBorder.at(i) << " ";
		}
		ts << "\r\n";
		for (int i = 0; i < xBorder.size(); i++) {
			ts << yBorder.at(i) << " ";
		}
		ts << "\r\n";
		for (int i = 0; i < xBorder.size(); i++) {
			ts << zBorder.at(i) << " ";
		}
		ts << "\r\n";
	}
	if (corners.size() != 0) {
		ts << corners.size() << "\r\n";
		for (int i = 0; i < corners.size(); i++) {
			ts << corners.at(i).at(0) << " " << corners.at(i).at(1) << " " << corners.at(i).at(2) << "\r\n";
		}
	}
	if (boundaries != NULL) {
		ts << boundaries[0] << " " << boundaries[1] << " "<< boundaries[2] << " " << boundaries[3] << "\r\n";
	}
	file.close();

	
}

void PlaneInfo::readPlane(QString fileName) {
	// reads the plane information to the file with the name that is passed into the method
	// the information are set in the following way
	// <plane number 1> <number of points on plane 1>
	// <information included 6 (set to 1 if included, 0 otherwise) e.g. when everything is included except the border this will be set to 1 1 1 1 1 0
	// <normal 3>
	// <translation vector 3>
	// <rotation matrix 9> (from left to write and top to bottom)
	// <color of the plane (float) 3>
	// <indices of points used on the plane (as many as there are points on the plane)>
	// <number of elements on the border 1 (b)>
	// <xBorder (b)>
	// <yBorder (b)>
	// <zBorder (b)>
	// sometimes not all the information are saved in that case 

	QFile file (fileName);
	// try to open file, return if failed
	if (!file.open(QIODevice::ReadOnly)) {
		return;
	}
	QTextStream ts (&file);
	QString line = ts.readLine();
	numberOfPoints = line.split(" ")[1].toInt();
	int* whatInfo = new int[8];
	QStringList list = ts.readLine().split(" ");
	for (int i = 0; i < 8; i++) {
		whatInfo[i] = list.at(i).toInt();
	}
	if (whatInfo[0] == 1) {
		normalSet = true;
		list = ts.readLine().split(" ");
		for (int i = 0; i < 3; i++) {
			normal(i) =list.at(i).toFloat();
		}
	}
	if (whatInfo[1] == 1) {
		translationSet = true;
		list = ts.readLine().split(" ");
		for (int i = 0; i < 3; i++) {
			translationVector(i) =list.at(i).toFloat();
		}
	}
	if (whatInfo[2] == 1) {
		rotationSet = true;
		list = ts.readLine().split(" ");
		for (int i = 0; i < 9; i++) {
			rotationMatrix(i/3,i%3) = list.at(i).toFloat();
		}
	}
	if (whatInfo[3] == 1) {
		list = ts.readLine().split(" ");
		if (color != NULL) {
			delete[] color;
		}
		color = new float[3];
		for (int i = 0; i < 3; i++) {
			color[i] = list.at(i).toFloat();
		}
	}
	if (whatInfo[4] == 1) {
		list = ts.readLine().split(" ");
		if (pointsUsed != NULL) {
			delete[] pointsUsed;
		}
		pointsUsed = new int[numberOfPoints];
		for (int i = 0; i < numberOfPoints; i++) {
			pointsUsed[i] = list.at(i).toInt();
		}
	}
	if (whatInfo[5] == 1) {
		int nBorderElements = ts.readLine().toInt();
		
		xBorder.clear();
		yBorder.clear();
		zBorder.clear();
		list = ts.readLine().split(" ");
		for (int i = 0; i < nBorderElements; i++) {
			xBorder.push_back(list.at(i).toFloat());
		}
		list = ts.readLine().split(" ");
		for (int i = 0; i < nBorderElements; i++) {
			yBorder.push_back(list.at(i).toFloat());
		}
		list = ts.readLine().split(" ");
		for (int i = 0; i < nBorderElements; i++) {
			zBorder.push_back(list.at(i).toFloat());
		}
	}
	if (whatInfo[6] == 1) {
		int nCorners = ts.readLine().toInt();
		for (int i = 0; i < corners.size(); i++) {
			corners.at(i).clear();
		}
		corners.clear();
		for (int i = 0; i < nCorners; i++) {
			list = ts.readLine().split(" ");
			vector<float> v;
			v.push_back(list.at(0).toFloat());
			v.push_back(list.at(1).toFloat());
			v.push_back(list.at(2).toFloat());
			corners.push_back(v);
			//corners.at(i).push_back(list.at(0).toFloat());
			//corners.at(i).push_back(list.at(1).toFloat());
			//corners.at(i).push_back(list.at(2).toFloat());
		}
	}
	if (whatInfo[7] == 1) {
		list = ts.readLine().split(" ");
		if (boundaries != NULL) {
			delete[] boundaries;
		}
		boundaries = new float[4];
		for (int i = 0; i < 4; i++) {
			boundaries[i] = list.at(i).toFloat();
		}
	}

	file.close();
	
}