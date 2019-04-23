#pragma once

#ifndef __PERLINMAP_H__
#define __PERLINMAP_H__

#include "noise.h"

using namespace noise;

class PerlinMap {

public:
	PerlinMap(void);
	PerlinMap(int width);
	void buildHeightMap();
	float** getHeightMap();

	float** getMoistMap();
	void buildMoistMap();

private:
	module::Perlin myModule;
	int width;
	float** heightMap;
	float convertVal(float sampleVal);

	//Perlin added for color map
	module::Perlin colorModule;
	float** moistMap;
};

#endif