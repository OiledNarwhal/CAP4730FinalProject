
//------------------------------------------------------------------------------
// Copyright 2017 Corey Toler-Franklin, University of Florida
// MySphere.cpp
// Sphere Object
//------------------------------------------------------------------------------



#include "MySphere.h"
#include "DiamondSquare.h"
#include "PerlinMap.h"


//-----------------------------------------------
// ConStructor
//-----------------------------------------------
MySphere::MySphere()
    : m_globalCount (0),
      m_levels      (1)
{
    // set the output fileneame
    m_pFileName = "../../data/meshes/mysphere.obj";
}



//-----------------------------------------------
// Destructor
//-----------------------------------------------
MySphere::~MySphere()
{
    ClearMesh();
}


char * MySphere::FileName(void)
{
    return(m_pFileName);
}


//-------------------------------------------------
// Initializes the TriangleIndices with vertices a, b and c
//-------------------------------------------------
TriangleIndices MySphere::MakeTIndices(int a, int b, int c)
{
    TriangleIndices T;
    T.i1 = a;
    T.i2 = b;
    T.i3 = c;
    return(T);
}




//Worthless but not changing so we can see how to change values in the vertices.
int MySphere::Offset(int pointIndex, std::vector<STVector3>* vertices)
{
	float pos = (1.0 + sqrtf(33.0)) / 2.0;
	float old = sqrtf(1 + (pos * pos));
	float xsquare = vertices->at(pointIndex).x * vertices->at(pointIndex).x;
	float ysquare = vertices->at(pointIndex).y * vertices->at(pointIndex).y;
	float zsquare = vertices->at(pointIndex).z * vertices->at(pointIndex).z;
	float fraction = sqrtf(xsquare + ysquare + zsquare);
	float ratio = (old / fraction);

	vertices->at(pointIndex).x = vertices->at(pointIndex).x * (ratio);
	vertices->at(pointIndex).y = vertices->at(pointIndex).y * (ratio);
	vertices->at(pointIndex).z = vertices->at(pointIndex).z * (ratio);

	return m_globalCount + 1;
}



//Worthless
int MySphere::MidPoint(int p1, int p2, std::multimap<long, int> *midPointIndices, std::vector<STVector3> *vertices)
{

        int index = 0;

        return(index);
}


//----------------------------------------------------------
// TO DO: Store your mesh data in the triangleMesh
//-----------------------------------------------------------
void MySphere::GenerateMesh(STTriangleMesh  *tmesh, std::vector<TriangleIndices> face, std::vector<STVector3> vertices, int nvert)
{

	for (int j = 0; j < vertices.size(); j++)
	{
		float x = 0;
		float y = 0;
		float z = 0;

		x = vertices.at(j).x;
		y = vertices.at(j).y;
		z = vertices.at(j).z;

		tmesh->AddVertex(x, y, z);
	}
	//std::cout << "Got here\n";
	//Adds the faces to STTrianglesMesh
	for (int i = 0; i < face.size(); i++)
	{
		//std::cout << "i: " << i << "\n";
		int holderOne = 0;
		int holderTwo = 0;
		int holderThree = 0;

		//Gets the position of the faces in the right order.
		holderOne = face.at(i).i1;
		holderTwo = face.at(i).i2;
		holderThree = face.at(i).i3;

		tmesh->AddFace(holderOne, holderTwo, holderThree);

		if (i == 32)
		{
			std::cout << holderOne << " " << holderTwo << " " << holderThree;
		}
	}
}


//Worthless
void MySphere::SubDivideTriangles(int level, std::vector<TriangleIndices> *facesIn, std::vector<TriangleIndices> *facesOut, std::vector<STVector3> *vertices)
{
}


//-------------------------------------------------------
// TO DO: Create faces for the Iscohedron
// Each TriangleIndices stores the counter clocksise vertex indices
// See definition of TriangleIndices and use this structure to set face indices
// Store your faces in m_faces
//-------------------------------------------------------
void MySphere::InitFaces(void)
{
	/* Saving these so we know how to add faces.
	//m_faces is now a vector with 20 triangles.

	//Triangle Fan Top = 0, 1, 2, 3, 32
	m_faces.push_back(MakeTIndices(33, 32, 9));
	m_faces.push_back(MakeTIndices(33, 9, 1));
	m_faces.push_back(MakeTIndices(33, 1, 0));
	m_faces.push_back(MakeTIndices(33, 0, 11));
	m_faces.push_back(MakeTIndices(33, 11, 32));

	//Triangle Fan Bottom = 33, 6, 7, 8, 9
	m_faces.push_back(MakeTIndices(6, 3, 8));
	m_faces.push_back(MakeTIndices(6, 8, 7));
	m_faces.push_back(MakeTIndices(6, 7, 10));
	m_faces.push_back(MakeTIndices(6, 10, 2));
	m_faces.push_back(MakeTIndices(6, 2, 3));

	//Triangle Strip = 10, 11, 12, 13, 132, 133, 16, 17, 18, 19
	m_faces.push_back(MakeTIndices(32, 3, 9));
	m_faces.push_back(MakeTIndices(9, 3, 8));
	m_faces.push_back(MakeTIndices(9, 8, 1));
	m_faces.push_back(MakeTIndices(1, 8, 7));
	m_faces.push_back(MakeTIndices(1, 7, 0));
	m_faces.push_back(MakeTIndices(10, 7, 0));
	m_faces.push_back(MakeTIndices(11, 10, 0));
	m_faces.push_back(MakeTIndices(2, 10, 11));
	m_faces.push_back(MakeTIndices(32, 2, 11));
	m_faces.push_back(MakeTIndices(3, 2, 32));

	//20 faces total.
	*/


	//Formulas to add faces to the plane.
	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			m_faces.push_back(MakeTIndices((33 * i) + j, (33 * i) + j + 1, (33 * (i+1)) + j + 1));
			m_faces.push_back(MakeTIndices((33 * i) + j, (33 * (i + 1)) + j + 1, (33 * (i + 1)) + j));
		}
	}

}


// Creates the vertices for a 33 x 33 vertex square.
void MySphere::InitVertices(void)
{
    m_vertices.clear();

    float pos = (1.0 + sqrtf(33.0))/2.0;

	for (int i = 0; i < 33; i++)
	{
		for (int j = 0; j < 33; j++)
		{
			m_vertices.push_back(STVector3(i * 0.2, 0, j * 0.2)); //Was i * 0.2, 0, j * 0.2
		}
	}
}




void MySphere::Create(int levels)
{
    m_globalCount = 0;

    InitVertices();
	InitFaces();
	std::cout << "Number of Faces: " << m_faces.size() << '\n'; 

	//Creating our DiamondSquare Array
	PerlinMap arrayMan = PerlinMap(33);
	arrayMan.buildHeightMap();
	float ** yArray = arrayMan.getHeightMap();

	//Changing the Y values of the vertices with the DiamondSquare Array
	for (int i = 0; i < 33; i++)
	{
		for (int j = 0; j < 33; j++)
		{
			float temp = yArray[i][j];

			m_vertices.at((33 * i) + j).y = temp;
		}
	}


	STTriangleMesh* dog = new STTriangleMesh();
	GenerateMesh(dog, m_faces, m_vertices, 0);

	m_TriangleMeshes.push_back(dog);
	

    // save the file
    Save(m_pFileName);

}



//----------------------------------------------------------------
// Saves the mesh stored in m_pTriangleMeshes in the file filename
//----------------------------------------------------------------
void MySphere::Save(char *filename)
 {
   // save the result sphere
    for(int id=0; id < (int)m_TriangleMeshes.size(); id++)
        m_TriangleMeshes[id]->Write(filename);
 }



//----------------------------------------------------------------
// Return the triangle mesh list
//----------------------------------------------------------------
std::vector<STTriangleMesh *> MySphere::GetTriangleMesh(void)
{
    return(m_TriangleMeshes);
}



//----------------------------------------------------------------
// Return the triangle mesh at the index id
//----------------------------------------------------------------
STTriangleMesh * MySphere::GetTriangleMesh(int id)
{
    return(m_TriangleMeshes[id]);
}



//----------------------------------------------------------------
// Clear the mesh - Cleanup
//----------------------------------------------------------------
void MySphere::ClearMesh(void)
{
    // remove the mesh
    for(int id=0; id < (int)m_TriangleMeshes.size(); id++)
        delete m_TriangleMeshes[id];

    if(m_TriangleMeshes.size())
        m_TriangleMeshes.clear();
}