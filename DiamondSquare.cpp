#include "MySphere.h"
#include "DiamondSquare.h"


//-----------------------------------------------
// Constructor
//-----------------------------------------------

DiamondSquare::DiamondSquare(void)
{
	width = 129;
	heightArray = 0;
	heightArray = new float*[129];

	for (int i = 0; i < 129; i++)
	{
		heightArray[i] = new float[129];
	}

	heightArray[0][0] = ((float)rand() / RAND_MAX) * 2 - 1;
	heightArray[0][128] = ((float)rand() / RAND_MAX) * 2 - 1;
	heightArray[128][0] = ((float)rand() / RAND_MAX) * 2 - 1;
	heightArray[128][128] = ((float)rand() / RAND_MAX) * 2 - 1;
}

DiamondSquare::DiamondSquare(int width)
{
	this->width = width;
	heightArray = 0;
	heightArray = new float*[width];

	for(int i = 0; i < width; i++)
	{
		heightArray[i] = new float[width];
	}
	heightArray[0][0] = ((float)rand() / RAND_MAX) * 2 - 1;
	heightArray[0][width - 1] = ((float)rand() / RAND_MAX) * 2 - 1;
	heightArray[width - 1][0] = ((float)rand() / RAND_MAX) * 2 - 1;
	heightArray[width - 1][width - 1] = ((float)rand() / RAND_MAX) * 2 - 1;
}

DiamondSquare::~DiamondSquare(void)
{
}

float ** DiamondSquare::getArray()
{
	return heightArray;
}

void DiamondSquare::buildArray()
{
	double squareSize = width - 1;
	float maxHeight = 0;
	float minHeight = 1000000000;

	while (squareSize >= 2) {

		int halfStep = squareSize / 2;
		
		double heightFactor = fmod(rand(), (0.75 - 0.01)) + 0.01;

		for (int y = halfStep; y < width; y += squareSize) {
			for (int x = halfStep; x < width; x += squareSize) {
				squareStep(x, y, halfStep, heightFactor);
			}
		}

		bool colTracker = false;
		
		for (int x = 0; x < width; x += halfStep) {
			
			colTracker = !(colTracker);
			
			if (colTracker) {

				for (int y = halfStep; y < width; y += squareSize) {
					diamondStep(x, y, halfStep, heightFactor);
				}
			}
			else {
				
				for (int y = 0; y < width; y += squareSize) {
					diamondStep(x, y, halfStep, heightFactor);
				}
			}
		}
		squareSize /= 2;
	}

	//Get the largest height.
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (maxHeight < heightArray[i][j])
			{
				maxHeight = heightArray[i][j];
			}

			if (minHeight > heightArray[i][j])
			{
				minHeight = heightArray[i][j];
			}
			//std::cout << "Height: " << heightArray[i][j] << "\n";
		}
	}

	/*
	//Normalize each point to be between 0 and 1.
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < width; j++)
		{
			heightArray[i][j] = ((heightArray[i][j] - minHeight) / (maxHeight - minHeight));
			//std::cout << "DS Heights: " << heightArray[i][j] << "\n";
		}
	}
	*/
}

void DiamondSquare::diamondStep(int x, int y, int radius, double heightFactor) {
	
	float avg;
	float sum = 0;
	int corners = 0;

	if ((x - radius) >= 0) {
		sum += heightArray[x - radius][y];
		corners++;
	}

	if ((x + radius) < width) {
		sum += heightArray[x + radius][y];
		corners++;
	}

	if ((y + radius) < width) {
		sum += heightArray[x][y + radius];
		corners++;
	}

	if ((y - radius) >= 0) {
		sum += heightArray[x][y - radius];
		corners++;
	}

	sum += ((float)rand() / RAND_MAX) * (radius * heightFactor) - radius * (heightFactor / 2);
	avg = sum / corners;
	heightArray[x][y] = avg;
}

void DiamondSquare::squareStep(int x, int y, int radius, double heightFactor) {

	float avg;
	float sum = 0;
	int corners = 0;

	if ((x + radius) < width && (y + radius) < width) {
		sum += heightArray[x + radius][y + radius];
		corners++;
	}

	if ((x - radius) >= 0 && (y + radius) < width) {
		sum += heightArray[x - radius][y + radius];
		corners++;
	}

	if ((x - radius) >= 0 && (y - radius) >= 0) {
		sum += heightArray[x - radius][y - radius];
		corners++;
	}

	if ((x + radius) < width && (y - radius) >= 0) {
		sum += heightArray[x + radius][y - radius];
		corners++;
	}
	sum += ((float)rand() / RAND_MAX) * (radius * heightFactor) - radius * (heightFactor / 2);
	avg = sum / corners;
	heightArray[x][y] = avg;
}
