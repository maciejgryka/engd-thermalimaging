#include "WarpImage.h"

QImage WarpImage::warpIntoTexture(QImage &image, MatrixXf points1, int texSizeX, int texSizeY)
{
	MatrixXf points2(2,5);
	points2.fill(0.0f);

	points2.col(0) << 0, 0;
	points2.col(1) << texSizeX, 0;
	points2.col(2) << texSizeX, texSizeY;
	points2.col(3) << 0, texSizeY;
	points2.col(4) << texSizeX/2, texSizeY/2;

	return warp(image, points1, points2, texSizeX, texSizeY);
}

QImage WarpImage::warp(QImage &image, MatrixXf points1, MatrixXf points2, int outSizeX, int outSizeY)
{
	calculateHomography(points1, points2);
	QImage newImage(outSizeX, outSizeY, image.format());

	newImage.fill(1);
	
	if (image.depth() == 8)
	{
		newImage.setColorTable(image.colorTable());

		for (int y = 0; y < image.height(); y++ )
		{
			for (int x = 0; x < image.width(); x++)
			{
				Vector3f homCoords(x, y, 1), newCoords;		// convert into homogeneous coordinates
				homCoords = homography*homCoords;			// multiply by homography
				
				newCoords[0] = homCoords[0]/homCoords[2];	// convert back into nonhomogenous coordinates
				newCoords[1] = homCoords[1]/homCoords[2];

				// if the new coordinate lies within the image boundary copy this pixel
				if (newImage.valid(newCoords[0], newCoords[1]))// && newCoords[0] > 0 && newCoords[0] < image.width() && newCoords[1] > 0 && newCoords[1] < image.height())
				{
					int xl = int(newCoords[0]);
					int yl = int(newCoords[1]);
					newImage.setPixel(int(newCoords[0]), int(newCoords[1]), image.pixelIndex(x, y));
				}
			}
		}
	} else {
		for (int y = 0; y < image.height(); y++ )
		{
			for (int x = 0; x < image.width(); x++)
			{
				Vector3f homCoords(x, y, 1), newCoords;		// convert into homogeneous coordinates
				homCoords = homography*homCoords;			// multiply by homography
				
				newCoords[0] = homCoords[0]/homCoords[2];	// convert back into nonhomogenous coordinates
				newCoords[1] = homCoords[1]/homCoords[2];

				// if the new coordinate lies within the image boundary copy this pixel
				if (newImage.valid(newCoords[0], newCoords[1]))// && newCoords[0] > 0 && newCoords[0] < image.width() && newCoords[1] > 0 && newCoords[1] < image.height())
				{
					int xl = int(newCoords[0]);
					int yl = int(newCoords[1]);
					newImage.setPixel(int(newCoords[0]), int(newCoords[1]), image.pixel(x, y));
				}
			}
		}
	}
	return newImage;
}

/// Calculates homography from 4 point pairs in two images by DLT algorithm.
Matrix3f* WarpImage::calculateHomography(MatrixXf points1, MatrixXf points2)
{
	homography = Matrix3f::Identity();

	// convert points into homogeneous coordinates
	MatrixXf points1hom(3,5); points1hom.fill(1.0);
	MatrixXf points2hom(3,5); points2hom.fill(1.0);

	MatrixXf A(10,9); A.fill(0.0);

	for (int row = 0; row < points1.rows(); row++)
	{
		for (int col = 0; col < points1.cols(); col++)
		{
			points1hom(row, col) = points1(row,col);
			points2hom(row, col) = points2(row,col);
		}
	}

	// Construct the A matrix
	// TODO: change to use .row() and .col() functions to manipulate blocks of a matrix
	int p = 0;
	A << 0, 0, 0, -points2hom(2,p)*points1hom(0,p), -points2hom(2,p)*points1hom(1,p), -points2hom(2,p)*points1hom(2,p), points2hom(1,p)*points1hom(0,p), points2hom(1,p)*points1hom(1,p), points2hom(1,p)*points1hom(2,p),
		 points2hom(2,p)*points1hom(0,p), points2hom(2,p)*points1hom(1,p), points2hom(2,p)*points1hom(2,p),	0, 0, 0, -points2hom(0,p)*points1hom(0,p), -points2hom(0,p)*points1hom(1,p), -points2hom(0,p)*points1hom(2,p),

		 0, 0, 0, -points2hom(2,p+1)*points1hom(0,p+1), -points2hom(2,p+1)*points1hom(1,p+1), -points2hom(2,p+1)*points1hom(2,p+1), points2hom(1,p+1)*points1hom(0,p+1), points2hom(1,p+1)*points1hom(1,p+1), points2hom(1,p+1)*points1hom(2,p+1),
		 points2hom(2,p+1)*points1hom(0,p+1), points2hom(2,p+1)*points1hom(1,p+1), points2hom(2,p+1)*points1hom(2,p+1),	0, 0, 0, -points2hom(0,p+1)*points1hom(0,p+1), -points2hom(0,p+1)*points1hom(1,p+1), -points2hom(0,p+1)*points1hom(2,p+1),

		 0, 0, 0, -points2hom(2,p+2)*points1hom(0,p+2), -points2hom(2,p+2)*points1hom(1,p+2), -points2hom(2,p+2)*points1hom(2,p+2), points2hom(1,p+2)*points1hom(0,p+2), points2hom(1,p+2)*points1hom(1,p+2), points2hom(1,p+2)*points1hom(2,p+2),
		 points2hom(2,p+2)*points1hom(0,p+2), points2hom(2,p+2)*points1hom(1,p+2), points2hom(2,p+2)*points1hom(2,p+2),	0, 0, 0, -points2hom(0,p+2)*points1hom(0,p+2), -points2hom(0,p+2)*points1hom(1,p+2), -points2hom(0,p+2)*points1hom(2,p+2),

		 0, 0, 0, -points2hom(2,p+3)*points1hom(0,p+3), -points2hom(2,p+3)*points1hom(1,p+3), -points2hom(2,p+3)*points1hom(2,p+3), points2hom(1,p+3)*points1hom(0,p+3), points2hom(1,p+3)*points1hom(1,p+3), points2hom(1,p+3)*points1hom(2,p+3),
		 points2hom(2,p+3)*points1hom(0,p+3), points2hom(2,p+3)*points1hom(1,p+3), points2hom(2,p+3)*points1hom(2,p+3),	0, 0, 0, -points2hom(0,p+3)*points1hom(0,p+3), -points2hom(0,p+3)*points1hom(1,p+3), -points2hom(0,p+3)*points1hom(2,p+3),

		 0, 0, 0, -points2hom(2,p+4)*points1hom(0,p+4), -points2hom(2,p+4)*points1hom(1,p+4), -points2hom(2,p+4)*points1hom(2,p+4), points2hom(1,p+4)*points1hom(0,p+4), points2hom(1,p+4)*points1hom(1,p+4), points2hom(1,p+4)*points1hom(2,p+4),
		 points2hom(2,p+4)*points1hom(0,p+4), points2hom(2,p+4)*points1hom(1,p+4), points2hom(2,p+4)*points1hom(2,p+4),	0, 0, 0, -points2hom(0,p+4)*points1hom(0,p+4), -points2hom(0,p+4)*points1hom(1,p+4), -points2hom(0,p+4)*points1hom(2,p+4);

	solveForHomography(A);
	return &homography;
}

Matrix3f* WarpImage::solveForHomography(MatrixXf A)
{
	VectorXf h = VectorXf::Ones(9);
	MatrixXf v = A.svd().matrixV();

	h = v.col(v.cols()-1);
	homography << h(0), h(1), h(2), h(3), h(4), h(5), h(6), h(7), h(8);

	return &homography;
}