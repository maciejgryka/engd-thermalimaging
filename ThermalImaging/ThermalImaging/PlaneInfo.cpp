#include "PlaneInfo.h"

PlaneInfo::PlaneInfo(){}
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

void PlaneInfo::setNormal(Vector3f n) {
	normal = n;
}

void PlaneInfo::setTranslationVector(Vector3f v){
	translationVector = v;
}

void PlaneInfo::setRotationMatrix(Matrix3f m){
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