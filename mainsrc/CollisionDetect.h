#pragma once

#ifndef __COLLISIONDETECT_H__
#define __COLLISIONDETECT_H__



#include <stdio.h>
#include <string.h>
#include <map>
#include <map>
#include <vector>
#include "STVector3.h"
#include "STTriangleMesh.h"

class CollisionDetect
{

public:
	CollisionDetect(void); //Constructor
	~CollisionDetect(void); //Destructor

	void detectCollision(MySphere ground, MySphere* character);

private:
};



#endif