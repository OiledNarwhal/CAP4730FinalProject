#include "MySphere.h"
#include "PerlinMap.h"


PerlinMap::PerlinMap(void) {
	width = 129;
	heightMap = 0;
	heightMap = new float*[129];

	for (int i = 0; i < 129; i++)
	{
		heightMap[i] = new float[129];
	}
}

PerlinMap::PerlinMap(int width) {
	this->width = width;
	heightMap = 0;
	heightMap = new float*[width];
	moistMap = new float*[width];

	for (int i = 0; i < width; i++)
	{
		heightMap[i] = new float[width];
		moistMap[i] = new float[width];
	}
}

void PerlinMap::buildHeightMap() {
	float maxHeight = 0;
	float minHeight = 1000000000000000000;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < width; j++) {
			float xSample = j / (float)width - 0.5;
			float ySample = i / (float)width - 0.5;
			float sampleVal1 = myModule.GetValue(xSample, ySample, 0);
			float sampleVal2 = myModule.GetValue(2 * xSample, 2 * ySample, 0);
			float sampleVal3 = myModule.GetValue(4 * xSample, 4 * ySample, 0);
			float sampleVal4 = myModule.GetValue(8 * xSample, 8 * ySample, 0);
			float sampleVal5 = myModule.GetValue(16 * xSample, 16 * ySample, 0);
			float sampleVal6 = myModule.GetValue(32 * xSample, 32 * ySample, 0);
			float x = 3 * (sampleVal1) + 1.5 * (sampleVal2) + 0.75 * (sampleVal3); //+ 0.13 * convertVal(sampleVal4) + 0.06 * convertVal(sampleVal5) + 0.03 * convertVal(sampleVal6);
			//x = x / (1.0 + 0.5 + 0.25 + 0.13 + 0.06 + 0.03);
			//std::cout << "Height: " << pow(x, 3) << "\n";
			//std::cout << sampleVal << std::endl;
			heightMap[i][j] = pow(x, 1.5); // was heightMap[i][j] = pow(x, 3);
		}
	}

	//Get the largest height.
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (maxHeight < heightMap[i][j])
			{
				maxHeight = heightMap[i][j];
			}

			if (minHeight > heightMap[i][j])
			{
				minHeight = heightMap[i][j];
			}
			//std::cout << "Height: " << heightMap[i][j] << "\n";

		}
	}

	/*
	//Normalize each point to be between 0 and 1.
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < width; j++)
		{
			heightMap[i][j] = (heightMap[i][j] - minHeight) / (maxHeight - minHeight);
			//heightMap[i][j] = pow(heightMap[i][j], 3);
		}
	}
	*/
	
}


void PerlinMap::buildMoistMap() {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < width; j++) {
			float xSample = j / (float)width - 0.5;
			float ySample = i / (float)width - 0.5;
			float sampleVal1 = colorModule.GetValue(xSample, ySample, 0);
			float sampleVal2 = colorModule.GetValue(2 * xSample, 2 * ySample, 0);
			float sampleVal3 = colorModule.GetValue(4 * xSample, 4 * ySample, 0);
			float sampleVal4 = colorModule.GetValue(8 * xSample, 8 * ySample, 0);
			float sampleVal5 = colorModule.GetValue(16 * xSample, 16 * ySample, 0);
			float sampleVal6 = colorModule.GetValue(32 * xSample, 32 * ySample, 0);
			float x = convertVal(sampleVal1) + 0.75 * convertVal(sampleVal2) + 0.33 * convertVal(sampleVal3) + 0.33 * convertVal(sampleVal4) + 0.33 * convertVal(sampleVal5) + 0.5 * convertVal(sampleVal6);
			x = x / (1.0 + 0.75 + (3 * .33) + 0.5);
			//std::cout << sampleVal << std::endl;
			moistMap[i][j] = x;
			//std::cout << "Height: " << y << "\n";
			//std::cout << "Moisture: " << moistMap[i][j] << "\n";
		}
	}
}


float** PerlinMap::getHeightMap() {
	return heightMap;
}

float PerlinMap::convertVal(float sampleVal) {
	return sampleVal / 2 + 0.5;
}

float** PerlinMap::getMoistMap()
{
	return moistMap;
}