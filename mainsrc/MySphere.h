//------------------------------------------------------------------------------
// Copyright 2017 Corey Toler-Franklin, University of Florida
// MySphere.h
// Sphere Object
//------------------------------------------------------------------------------

#ifndef __MYSPHERE_H__
#define __MYSPHERE_H__



#include <stdio.h>
#include <string.h>
#include <map>
#include <map>
#include <vector>
#include "STVector3.h"
#include "STTriangleMesh.h"


// triangle indices
typedef struct {
  int i1;
  int i2;
  int i3;
} TriangleIndices;


class MySphere
{


public:
                                     MySphere                   (void); // contructor
                                    ~MySphere                   (void); // destructor

    void                            Create                      (int levels); // creates the initial sphere
    std::vector<STTriangleMesh *>   GetTriangleMesh             (void); // returns the triangle mesh
    STTriangleMesh *                GetTriangleMesh             (int id); // returns the triangle mesh at the index

    char  *                         FileName                    (void); // return the file name
    


private:


    char                            *m_pFileName;           // file name for the ouput mesh obj file
    std::vector<STVector3>          m_vertices;             // current vetex list
    std::vector<TriangleIndices>    m_faces;                // current facelist stored as indices
    std::vector<STTriangleMesh *>   m_TriangleMeshes;       // triangle meshes for this sphere
    int                             m_globalCount;          // global counter, for tracking offsets to sphere surface
    int                             m_levels;               // subdivision levels
	int								numVerts;		/*ADDED FOR TERRAIN GENERATION*/



 
    void                            InitVertices                (void);
    void                            InitFaces                   (void);
 
    void                            SubDivideTriangles          (int level, std::vector<TriangleIndices> *facesIn,  std::vector<TriangleIndices> *facesOut, std::vector<STVector3> *vertices);
    void                            GenerateMesh                (STTriangleMesh  *tmesh, std::vector<TriangleIndices> face, std::vector<STVector3> vertices, int nvert);


    int                             MidPoint                    (int p1, int p2, std::multimap<long, int> *midPointIndices, std::vector<STVector3> *vertices);
    int                             Offset                      (int p, std::vector<STVector3>* vertices);
    TriangleIndices                 MakeTIndices                (int a, int b, int c);

    
    
    void                            ClearMesh                   (void);


    void                            Save                        (char *filename);
};


#endif //__MYSPHERE_H__
