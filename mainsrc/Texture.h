#pragma once

#ifndef __TEXTURE_H__
#define __TEXTURE_H__



#include <stdio.h>
#include <string.h>
#include <map>
#include <map>
#include <vector>
#include "STVector3.h"
#include "STTriangleMesh.h"

class Texture
{

public:
	Texture(void); //Constructor
	~Texture(void); //Destructor

	void Apply(MySphere ground, MySphere* character);

private:
};



#endif
