#include<iostream>
#include<fstream>
#include <iomanip>

using namespace std;

int main(int argc, char *argv[]) {

	if (argc < 3) {
		cout << "Too few input paramters. Need to provide input file and output file" << endl;
		return 0;
	}
 ifstream myReadFile;
 myReadFile.open(argv[1]);

 int noPoints;
 int pointsRemoved = 0;
 int vectorLength;

 if (myReadFile.is_open()) {
	myReadFile >> noPoints;
	myReadFile >> vectorLength;
	float **pointHeader;
	int **features;
	pointHeader = new float*[noPoints];
	features = new int*[noPoints];
	for (int i = 0; i < noPoints; i++) {
		pointHeader[i] = new float[4];
		for (int j = 0; j < 4; j++) {
			myReadFile >> pointHeader[i][j];
		}
		if (pointHeader[i][0] < 130.0f && pointHeader[i][1] < 60.0f) {
			pointsRemoved++;
		}
		features[i] = new int[vectorLength];
		for (int j = 0; j < vectorLength; j++) {
			myReadFile >> features[i][j];
		}		
	}
	myReadFile.close();
	
	ofstream myWriteFile;
	myWriteFile.open(argv[2]);

	myWriteFile << (noPoints - pointsRemoved) << " " << vectorLength << endl;

	for (int i = 0; i < noPoints; i++) {
		if (pointHeader[i][0] >= 130.0f || pointHeader[i][1] >= 60.0f) {
			for (int j = 0; j < 4; j++) {
				if (j !=3)
					myWriteFile << setiosflags(ios::fixed) << setprecision(2) << pointHeader[i][j] << ((j !=3) ? " " : "");
				else
					myWriteFile << setiosflags(ios::fixed) << setprecision(3) << pointHeader[i][j] << ((j !=3) ? " " : "");
			}
			free(pointHeader[i]);
			myWriteFile << endl;
			for (int j = 0; j < vectorLength; j++) {
				if (j % 20 == 0) {
					myWriteFile << " ";
				}
				myWriteFile << features[i][j];
				if ((j + 1) % 20 == 0 && j != vectorLength) {
					myWriteFile << endl;
				} else if (j != vectorLength){
					myWriteFile << " ";
				}
			}
			myWriteFile << endl;
			free(features[i]);
		}
	}
	free(features);
	free(pointHeader);
	myWriteFile.close();
 }
 


return 0;
}
