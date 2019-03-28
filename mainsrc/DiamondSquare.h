#pragma once

#ifndef __DIAMONDSQUARE_H__
#define __DIAMONDSQUARE_H__



#include <stdio.h>
#include <string.h>
#include <map>
#include <map>
#include <vector>
#include "STVector3.h"
#include "STTriangleMesh.h"

class DiamondSquare
{

public:
	DiamondSquare(void); //Constructor
	~DiamondSquare(void); //Destructor

	float** getArray();

	void buildArray();


private:

	float** heightArray;
};



#endif