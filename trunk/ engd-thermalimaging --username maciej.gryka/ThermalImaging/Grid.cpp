#include "Grid.h"

Grid::Grid() {
	boundaries = NULL;
	grid = NULL;
	gridSize = 0;
}

Grid::~Grid() {
	free(boundaries);
	clearData();
}

void Grid::clearData() {
	for (int i = 0; i < gridSize; i++) {
		free(grid[i]);
		grid[i] = NULL;
	}
	free(grid);
	grid = NULL;
}

void Grid::setGrid(int** g, int gS) {
	grid = g;
	gridSize = gS;
}

void Grid::setBoundaries(float* bs) {
	boundaries = bs;
}

int** Grid::getGrid() {
	return grid;
}

void Grid::dilate() {

}

void Grid::erode() {

}

void Grid::drawGrid() {
	if (boundaries != NULL) {
		float xStepSize = (boundaries[2] - boundaries[0]) / (float) gridSize;
		float zStepSize = (boundaries[3] - boundaries[1]) / (float) gridSize;
		for (int i = 0; i < gridSize; i++) {
			for (int j = 0; j < gridSize; j++) {
				glBegin(GL_QUADS);
				glColor3f(1.0f,1.0f,1.0f);
				glVertex3f(boundaries[0] + xStepSize * i,0.0f,boundaries[3] + zStepSize * (i + 1));
				glVertex3f(boundaries[0] + xStepSize * (i + 1),0.0f,boundaries[3] + zStepSize * (i + 1));
				glVertex3f(boundaries[0] + xStepSize * (i + 1),0.0f,boundaries[3] + zStepSize * i);
				glVertex3f(boundaries[0] + xStepSize * i,0.0f,boundaries[3] + zStepSize * i);
				//qDebug() << boundaries[0] << boundaries[1] << boundaries[2] << boundaries[3];
				glEnd();
			}
		}
	}

}
