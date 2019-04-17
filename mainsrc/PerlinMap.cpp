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

	for (int i = 0; i < width; i++)
	{
		heightMap[i] = new float[width];
	}
}

void PerlinMap::buildHeightMap() {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < width; j++) {
			float xSample = j / (float)width - 0.5;
			float ySample = i / (float)width - 0.5;
			float sampleVal1 = myModule.GetValue(xSample, ySample, 0);
			float sampleVal2 = myModule.GetValue(2 * xSample, 2 * ySample, 0);
			float sampleVal3 = myModule.GetValue(4 * xSample, 2 * ySample, 0);
			float x = convertVal(sampleVal1) + 0.5 * convertVal(sampleVal2) + 0.25 * convertVal(sampleVal3);
			//std::cout << sampleVal << std::endl;
			heightMap[i][j] = pow(x, 2);
		}
	}
}


float** PerlinMap::getHeightMap() {
	return heightMap;
}

float PerlinMap::convertVal(float sampleVal) {
	return sampleVal / 2 + 0.5;
}