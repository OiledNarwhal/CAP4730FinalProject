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

private:
	module::Perlin myModule;
	int width;
	float** heightMap;
	float convertVal(float sampleVal);
};

#endif