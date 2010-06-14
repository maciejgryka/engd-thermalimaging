#include "PointExtractor.h"

PointExtractor::PointExtractor() {}

void PointExtractor::collectPoints() {

	//RandomPointCloud* c = new RandomPointCloud();
	//c->makePointCloud(1000);
	//c = new Ply2OpenGL();
	//c->setFile();
	//c->createFiles();

	//points = c->getPoints();
	//colors = c->getColors();
	//normals = c->getNormals();
	//noPoints = c->getNumberOfPoints();

	/*Ransac* r = new Ransac();
	r->setPoints(c->convert2Matrix(c->getPoints()),1000);
	r->setInlierDistance(1.0f);
	r->setIterations(1000);
	int noPoints = 0;*/

	//int* ps = r->findBestPlane(noPoints);
}

float** PointExtractor::getPoints() {
	//return c->convert2Matrix(c->getPoints());
	return NULL;
}

float** PointExtractor::getColors() {
	//return c->convert2Matrix(c->getColors());
	return NULL;
}

float** PointExtractor::getNormals() {
	//return c->convert2Matrix(c->getNormals());
	return NULL;
}

int PointExtractor::getNumberOfPoints() {
	//return c->getNumberOfPoints();
	return NULL;
}