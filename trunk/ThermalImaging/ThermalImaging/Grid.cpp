#include "Grid.h"

Grid::Grid() {
	boundaries = NULL;
	grid = NULL;
	gridSize = 0;
	border = NULL;
}

Grid::~Grid() {
	free(boundaries);
	clearData();
}

void Grid::clearData() {
	for (int i = 0; i < gridSize; i++) {
		free(grid[i]);
		grid[i] = NULL;
		if (border != NULL) {
			free(border[i]);
			border[i] = NULL;
		}
	}
	free(grid);
	grid = NULL;
	free(border);
	border = NULL;
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

int Grid::getGridSize() {
	return gridSize;
}

void Grid::dilateErode(int k, int d, bool eightNeighbour) {
	/*qDebug() << "first";
	for (int i = 0; i < gridSize; i++) {
		qDebug() << grid[i][0] << grid[i][1] << grid[i][2] << grid[i][3] << grid[i][4] << grid[i][5] << grid[i][6] << grid[i][7] << grid[i][8] << grid[i][9]
		<< grid[i][10] << grid[i][11] << grid[i][12] << grid[i][13] << grid[i][14] << grid[i][15];
	}*/
	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			if (grid[i][j] == d) {
				grid[i][j] = 0;
			} else {
				grid[i][j] = 2 * gridSize;
				if (i > 0)
					grid[i][j] = grid[i][j] < grid[i-1][j]+1 ? grid[i][j] : grid[i-1][j]+1;
				if (j > 0)
					grid[i][j] = grid[i][j] < grid[i][j-1]+1 ? grid[i][j] : grid[i][j-1]+1;
				if (eightNeighbour && i > 0 && j > 0)
					grid[i][j] = grid[i][j] < grid[i-1][j-1]+1 ? grid[i][j] : grid[i-1][j-1]+1;
				if (eightNeighbour && i > 0 && j < gridSize - 1)
					grid[i][j] = grid[i][j] < grid[i-1][j+1]+1 ? grid[i][j] : grid[i-1][j+1]+1;
			}
		}
	}
	/*qDebug() << "next1";
	for (int i = 0; i < gridSize; i++) {
		qDebug() << grid[i][0] << grid[i][1] << grid[i][2] << grid[i][3] << grid[i][4] << grid[i][5] << grid[i][6] << grid[i][7] << grid[i][8] << grid[i][9]
		<< grid[i][10] << grid[i][11] << grid[i][12] << grid[i][13] << grid[i][14] << grid[i][15] << grid[i][16] << grid[i][17] << grid[i][18] << grid[i][19];
	}*/
	for (int i = gridSize -1; i >= 0; i--) {
		for (int j = gridSize -1; j >= 0 ; j--) {

			if (i < gridSize -1)
				grid[i][j] = grid[i][j] < grid[i+1][j]+1 ? grid[i][j] : grid[i+1][j]+1;
			if (j < gridSize - 1)
				grid[i][j] = grid[i][j] < grid[i][j+1]+1 ? grid[i][j] : grid[i][j+1]+1;
			if (eightNeighbour && i < gridSize -1 && j < gridSize - 1)
				grid[i][j] = grid[i][j] < grid[i+1][j+1]+1 ? grid[i][j] : grid[i+1][j+1]+1;
			if (eightNeighbour && i < gridSize - 1 && j > 0)
				grid[i][j] = grid[i][j] < grid[i+1][j-1]+1 ? grid[i][j] : grid[i+1][j-1]+1;
		}
	}
	/*qDebug() << "next2";
	for (int i = 0; i < gridSize; i++) {
		qDebug() << grid[i][0] << grid[i][1] << grid[i][2] << grid[i][3] << grid[i][4] << grid[i][5] << grid[i][6] << grid[i][7] << grid[i][8] << grid[i][9]
		<< grid[i][10] << grid[i][11] << grid[i][12] << grid[i][13] << grid[i][14] << grid[i][15] << grid[i][16] << grid[i][17] << grid[i][18] << grid[i][19];
	}*/
	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			grid[i][j] = grid[i][j] <= k ? d : 1-d;
		}
	}
	/*qDebug() << "out";
	for (int i = 0; i < gridSize; i++) {
		qDebug() << grid[i][0] << grid[i][1] << grid[i][2] << grid[i][3] << grid[i][4] << grid[i][5] << grid[i][6] << grid[i][7] << grid[i][8] << grid[i][9]
		<< grid[i][10] << grid[i][11] << grid[i][12] << grid[i][13] << grid[i][14] << grid[i][15];
	}*/

}

void Grid::dilate(int k) {
	dilateErode(k,1, true);
}

void Grid::dilate(int k, bool eight) {
	dilateErode(k,1, eight);
}

void Grid::erode(int k) {
	dilateErode(k,0, true);
}

void Grid::erode(int k, bool eight) {
	dilateErode(k,0, eight);
}

void Grid::dilateAndErode(int k) {
	dilateAndErode(k,true);
}

void Grid::dilateAndErode(int k, bool eight) {
	int tmpGridSize = gridSize;

	int** tmp = new int*[gridSize + 2*(k+1)];
	for (int i = 0; i< gridSize + 2*(k+1); i++) {
		tmp[i] = new int[gridSize + 2*(k+1)];
		for(int j = 0; j < gridSize + 2*(k+1); j++) {
			tmp[i][j] = 0;
			if (i > k && i <= k + gridSize && j > k && j <= k + gridSize) {
				tmp[i][j] = grid[i - k - 1][j - k -1];
			}
		}
	}
	gridSize = gridSize + 2*(k+1);

	int** tmp2 = grid;
	grid = tmp;

	dilate(k, eight);
	erode(k, eight);
	gridSize = tmpGridSize;

	for (int i = 0; i< gridSize; i++) {
		for(int j = 0; j < gridSize; j++) {
			tmp2[i][j] = grid[i+k+1][j+k+1];
		}
	}
	grid = tmp2;

	/*qDebug() << "end";
	for (int i = 0; i < gridSize; i++) {
		qDebug() << grid[i][0] << grid[i][1] << grid[i][2] << grid[i][3] << grid[i][4] << grid[i][5] << grid[i][6] << grid[i][7] << grid[i][8] << grid[i][9]
		<< grid[i][10] << grid[i][11] << grid[i][12] << grid[i][13] << grid[i][14] << grid[i][15];
	}*/

	for (int i = 0; i < gridSize + 2*(k+1); i++) {
		free(tmp[i]);
		tmp[i] = NULL;
	}
	free(tmp);
	tmp = NULL;
	grid = tmp2;

}

void Grid::erodeAndDilate(int k) {
	erodeAndDilate(k, true);
}

void Grid::erodeAndDilate(int k, bool eight) {
	erode(k, eight);
	dilate(k, eight);
}

void Grid::drawGrid() {
	if (boundaries != NULL) {
		float xStepSize = (boundaries[2] - boundaries[0]) / (float) gridSize;
		float zStepSize = (boundaries[1] - boundaries[3]) / (float) gridSize;
		for (int i = 0; i < gridSize; i++) {
			for (int j = 0; j < gridSize; j++) {
				if (grid[i][j] == 1) {
					glBegin(GL_QUADS);
					glColor3f(1.0f,1.0f,1.0f);
					glVertex3f(boundaries[0] + xStepSize * i,0.0f,boundaries[3] + zStepSize * (j + 1));
					glVertex3f(boundaries[0] + xStepSize * (i + 1),0.0f,boundaries[3] + zStepSize * (j + 1));
					glVertex3f(boundaries[0] + xStepSize * (i + 1),0.0f,boundaries[3] + zStepSize * j);
					glVertex3f(boundaries[0] + xStepSize * i,0.0f,boundaries[3] + zStepSize * j);
					//qDebug() << boundaries[0] << boundaries[1] << boundaries[2] << boundaries[3];
					glEnd();
				}
			}
		}
	}

}

void Grid::calculateBorder() {
	if (border == NULL) {
		border = new int*[gridSize];
		for (int i = 0; i < gridSize; i++) {
			border[i] = new int[gridSize];
			for (int j = 0; j < gridSize; j++) {
				border[i][j] = grid[i][j];
			}
		}
	}
	int** tmp = grid;
	grid = border;
	erode(1,false);
	border = grid;
	grid = tmp;

	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			if (i == 0 || i == gridSize - 1 || j == 0 || j == gridSize - 1) {
				border[i][j] = 0;
			}
			border[i][j] = grid[i][j] != border[i][j];
		}
	}

	qDebug() << "border";
	for (int i = 0; i < gridSize; i++) {
		qDebug() << border[i][0] << border[i][1] << border[i][2] << border[i][3] << border[i][4] << border[i][5] << border[i][6] << border[i][7] << border[i][8] << border[i][9]
		<< border[i][10] << border[i][11] << border[i][12] << border[i][13] << border[i][14] << border[i][15];
	}
	
	
}


void Grid::drawAsPolygon() {
	if (border == NULL) {
		calculateBorder();
	}
	if (boundaries != NULL) {
		float xStepSize = (boundaries[2] - boundaries[0])/ (float) gridSize;
		float zStepSize = (boundaries[1] - boundaries[3])/ (float) gridSize;
		int startI = 0;
		int startJ = 0;
		bool done = false;
		for (int i = 0; i < gridSize && !done; i++) {
			for (int j = 0; j < gridSize && !done; j++) {
				if (grid[i][j] == 1) {
					done = true;
					startI = i;
					startJ = j;
				}
			}
		}
		glBegin(GL_POLYGON);
		int currI = startI;
		int currJ = startJ;
		glColor3f(1.0f,1.0f,1.0f);
		glVertex3f(boundaries[0] + ((float) currI + 0.5f) * xStepSize, 0.0f, boundaries[3] + ((float) currJ + 0.5f) * zStepSize);
		int prevMov = -1;
		int steps = 0;

		while (!(currI == startI && currJ == startJ && steps != 0)) {
			int d;
			bool found = false;
			for (d = 0; d < 8 && !found; d++) {
				if (d == 0 && d + prevMov != 7 && currI -1 >= 0 && currJ + 1 < gridSize && border[currI-1][currJ+1] == 1) {
					currI--;
					currJ++;
					found = true;
				} else if(d == 1 && d + prevMov != 7 && currJ + 1 < gridSize && border[currI][currJ+1] == 1) {
					currJ++;
					found = true;
				} else if(d == 2 && d + prevMov != 7 && currI + 1 < gridSize  && currJ + 1 < gridSize && border[currI+1][currJ+1] == 1) {
					currI++;
					currJ++;
					found = true;
				} else if(d == 3 && d + prevMov != 7 && currI -1 >= 0 && border[currI-1][currJ] == 1) {
					currI--;
					found = true;
				} else if(d == 4 && d + prevMov != 7 && currI + 1 < gridSize && border[currI+1][currJ] == 1) {
					currI++;
					found = true;
				}  else if(d == 5 && d + prevMov != 7 && currI -1 >= 0 && currJ -1 >= 0 && border[currI-1][currJ-1] == 1) {
					currI--;
					currJ--;
					found = true;
				} else if(d == 6 && d + prevMov != 7 && currJ -1 >= 0 && border[currI][currJ-1] == 1) {
					currJ--;
					found = true;
				} else if(d == 7 && d + prevMov != 7 && currI + 1 < gridSize && currJ -1 >= 0 && border[currI+1][currJ-1] == 1) {
					currI++;
					currJ--;
					found = true;
				} 
			}
			border[currI][currJ] = 8;
			/*qDebug() << "next";
			for (int i = 0; i < gridSize; i++) {
				qDebug() << border[i][0] << border[i][1] << border[i][2] << border[i][3] << border[i][4] << border[i][5] << border[i][6] << border[i][7] << border[i][8] << border[i][9]
				<< border[i][10] << border[i][11] << border[i][12] << border[i][13] << border[i][14] << border[i][15];
			}*/
			border[currI][currJ] = 1;
			d--;
			qDebug() << d;
			prevMov = d;
			
			glColor3f(1.0f,1.0f,1.0f);
			glVertex3f(boundaries[0] + ((float) currI + 0.5f) * xStepSize, 0.0f, boundaries[3] + ((float) currJ + 0.5f) * zStepSize);

			steps++;
		}
		glEnd();
	}
}
