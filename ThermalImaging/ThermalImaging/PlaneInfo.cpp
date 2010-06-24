#include "PlaneInfo.h"

PlaneInfo::PlaneInfo(){
	normal = NULL;
	translationVector = NULL;
	rotationMatrix = NULL;
	color = NULL;
	pointsUsed = NULL;
	quad = NULL;
	grid = NULL;
	numberOfPoints = -1;
	pointsOnPlane = NULL;
}
PlaneInfo::~PlaneInfo(){}

Vector3f* PlaneInfo::getNormal(){
	return normal;
}

Vector3f* PlaneInfo::getTranslationVector(){
	return translationVector;
}

Matrix3f* PlaneInfo::getRotationMatrix(){
	return rotationMatrix;
}

float* PlaneInfo::getColor(){
	return color;
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

vector<float> PlaneInfo::getXBorder() {
	return xBorder;
}

vector<float> PlaneInfo::getYBorder() {
	return yBorder;
}

vector<float> PlaneInfo::getZBorder() {
	return zBorder;
}

vector<vector<float> > PlaneInfo::getCorners() {
	return corners;
}

void PlaneInfo::setNormal(Vector3f* n) {
	normal = n;
}

void PlaneInfo::setTranslationVector(Vector3f* v){
	translationVector = v;
}

void PlaneInfo::setRotationMatrix(Matrix3f* m){
	rotationMatrix = m;
}

void PlaneInfo::setColor(float* c){
	color = c;
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

void PlaneInfo::setXBorder(vector<float> b) {
	xBorder = b;
}

void PlaneInfo::setYBorder(vector<float> b) {
	yBorder = b;
}

void PlaneInfo::setZBorder(vector<float> b) {
	zBorder = b;
}

void PlaneInfo::setCorners(vector<vector<float> > c) {
	corners = c;
}

vector<vector<float> > PlaneInfo::unrotateCorners(vector<vector<float> > corners)
{
	for (int ci(0); ci < corners.size(); ci++)
	{
		Vector3f corner(corners.at(ci).at(0), corners.at(ci).at(1), corners.at(ci).at(2));
		corner = *rotationMatrix * corner;
		corner += *translationVector;
		corners.at(ci).at(0) = corner[0];
		corners.at(ci).at(1) = corner[1];
		corners.at(ci).at(2) = corner[2];
	}
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
	ts << (normal != NULL) << " " << (translationVector != NULL) << " " << (rotationMatrix != NULL) << " " 
		<< (color != NULL) << " " << (pointsUsed != NULL) << " " << (xBorder.size() != 0) << "\r\n";
	if (normal != NULL) {
		Vector3f n = *normal;
		ts << n(0) << " " << n(1) << " " << n(2) << "\r\n";
	}
	if (translationVector != NULL) {
		Vector3f t = *translationVector;
		ts << t(0) << " " << t(1) << " " << t(2) << "\r\n";
	}
	if (rotationMatrix != NULL) {
		Matrix3f m = *rotationMatrix;
		ts << m(0,0) << " " << m(0,1) << " " << m(0,2) << " "
			<< m(1,0) << " " << m(1,1) << " " << m(1,2) << " "
			<< m(2,0) << " " << m(2,1) << " " << m(2,2) << "\r\n";
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
		ts << xBorder.size() ;
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
	int* whatInfo = new int[6];
	QStringList list = ts.readLine().split(" ");
	for (int i = 0; i < 6; i++) {
		whatInfo[i] = list.at(i).toInt();
	}
	if (whatInfo[0] == 1) {
		list = ts.readLine().split(" ");
		if (normal != NULL) {
			delete normal;
		}
		normal = new Vector3f(list.at(0).toFloat(),list.at(1).toFloat(),list.at(2).toFloat());
	}
	if (whatInfo[1] == 1) {
		list = ts.readLine().split(" ");
		if (translationVector != NULL) {
			delete translationVector;
		}
		translationVector = new Vector3f(list.at(0).toFloat(),list.at(1).toFloat(),list.at(2).toFloat());
	}
	if (whatInfo[2] == 1) {
		list = ts.readLine().split(" ");
		if (rotationMatrix != NULL) {
			delete rotationMatrix;
		}
		Matrix3f r;
		for (int i = 0; i < 9; i++) {
			r(i/3,i%3) = list.at(i).toFloat();
		}
		rotationMatrix = &r;
	}
	if (whatInfo[3] == 1) {
		list = ts.readLine().split(" ");
		if (color != NULL) {
			delete color;
		}
		color = new float[3];
		for (int i = 0; i < 3; i++) {
			color[i] = list.at(i).toFloat();
		}
	}
	if (whatInfo[4] == 1) {
		list = ts.readLine().split(" ");
		if (pointsUsed != NULL) {
			delete pointsUsed;
		}
		pointsUsed = new int[numberOfPoints];
		for (int i = 0; i < numberOfPoints; i++) {
			pointsUsed[i] = list.at(i).toInt();
		}
	}
	if (whatInfo[5] == 1) {
		int nBorderElements = ts.readLine().toInt();
		
		vector<float> xb;
		vector<float> yb;
		vector<float> zb;
		list = ts.readLine().split(" ");
		for (int i = 0; i < nBorderElements; i++) {
			xb.push_back(list.at(i).toFloat());
		}
		list = ts.readLine().split(" ");
		for (int i = 0; i < nBorderElements; i++) {
			yb.push_back(list.at(i).toFloat());
		}
		list = ts.readLine().split(" ");
		for (int i = 0; i < nBorderElements; i++) {
			zb.push_back(list.at(i).toFloat());
		}
	}

	file.close();
	
}