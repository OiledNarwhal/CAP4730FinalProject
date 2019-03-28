#include "MySphere.h"
#include "DiamondSquare.h"


//-----------------------------------------------
// Constructor
//-----------------------------------------------
DiamondSquare::DiamondSquare()
{
	heightArray = 0;
	heightArray = new float*[129];

	for(int i = 0; i < 129; i++)
	{
		heightArray[i] = new float[129];
	}
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
	for (int i = 0; i < 129; i++)
	{
		for (int j = 0; j < 129; j++)
		{
			heightArray[i][j] = i * j * j * 0.00001;
			//i * j * j * 0.00001;
		}
	}

	
	heightArray[0][0] = 14.5;
	heightArray[0][4] = 15;
	heightArray[4][0] = 15;
	heightArray[4][4] = 14.3;

	heightArray[2][2] = 14.7;

	heightArray[0][2] = 14.75;
	heightArray[2][0] = 14.75;
	heightArray[2][4] = 14.65;
	heightArray[4][2] = 14.65;

	heightArray[1][1] = 14.675;
	heightArray[1][3] = 14.775;
	heightArray[3][1] = 14.775;
	heightArray[3][3] = 14.575;

	heightArray[0][1] = 14.625;
	heightArray[1][0] = 14.625;
	heightArray[1][2] = 14.725;
	heightArray[2][1] = 14.725;

	heightArray[0][3] = 14.875;
	heightArray[1][4] = 14.825;
	heightArray[2][3] = 14.675;

	heightArray[3][0] = 14.875;
	heightArray[4][1] = 14.825;
	heightArray[3][2] = 14.675;

	heightArray[3][4] = 14.475;
	heightArray[4][3] = 14.475;


	
}
