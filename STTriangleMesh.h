// STTriangleMesh.h
#ifndef __STTRIANGLEMESH_H__
#define __STTRIANGLEMESH_H__

#include "STPoint2.h"
#include "STPoint3.h"

#include <string>
#include <vector>
#include <iostream>

struct STFace;

struct STVertex{
    STVertex(float x, float y, float z, float u=0, float v=0){
        pt=STPoint3(x,y,z);
        texPos=STPoint2(u,v);
    }
    STVertex(const STPoint3& pt, const STPoint2& texPos=STPoint2(0, 0)){
        this->pt=pt;
        this->texPos=texPos;
        f=0;
    }
    STPoint3 pt;
    STVector3 normal;
    STPoint2 texPos;
    STFace *f;
};
inline std::ostream& operator <<(std::ostream& stream, const STVertex& v);

struct STFace{
    STFace(STVertex* v0, STVertex* v1, STVertex* v2){
        v[0]=v0;
        v[1]=v1;
        v[2]=v2;
        texPos[0] = &(v0->texPos);
        texPos[1] = &(v1->texPos);
        texPos[2] = &(v2->texPos);
    }
    STFace(STVertex* v0, STVertex* v1, STVertex* v2, STVector3* n1, STVector3* n2, STVector3* n3){
        v[0]=v0;
        v[1]=v1;
        v[2]=v2;
        texPos[0] = &(v0->texPos);
        texPos[1] = &(v1->texPos);
        texPos[2] = &(v2->texPos);
        normals[0]=n1;
        normals[1]=n2;
        normals[2]=n3;
    }
    STVertex *v[3];
    STFace *adjF[3];
    STVector3 normal;
    STVector3 *normals[3];
    STPoint2 *texPos[3];
};
inline std::ostream& operator <<(std::ostream& stream, const STFace& f);

/**
* STTriangleMesh use a simple data structure to represent a triangle mesh.
*/
class STTriangleMesh
{
public:
    //
    // Initialization
    //
    STTriangleMesh();
    STTriangleMesh(const std::string& filename);

    //
    // Delete and clean up.
    //
    ~STTriangleMesh();

    bool mSimpleMesh;

    //
    // Draw the triangle mesh to the OpenGL window using GL_TRIANGLES.
    //
    void Draw(bool smooth) const;

    //
    // Read and Write the triangle mesh from/to files.
    //
    bool Read(const std::string& filename);

    bool Write(const std::string& filename);

    unsigned int AddVertex(float x, float y, float z, float u=0, float v=0);

    unsigned int AddVertex(const STPoint3& pt, const STPoint2& texPos=STPoint2(0, 0));

    unsigned int AddFace(unsigned int id0,unsigned int id1,unsigned int id2);
    
    //
    // Build topology and calculate normals for the triangle mesh.
    //
    bool Build();
    bool BuildTopology();
    bool UpdateGeometry();
	bool CalculateTextureCoordinatesViaSphericalProxy();

    STFace* NextAdjFace(STVertex *v, STFace *f);
    STFace* NextAdjFaceReverse(STVertex *v, STFace *f);

    void LoopSubdivide();
    //
    // Local members
    //
    std::vector<STVertex*> mVertices;
    std::vector<STVector3*> mNormals;
    std::vector<STPoint2*> mTexPos;
    std::vector<STFace*> mFaces;
    
    static std::string LoadObj(std::vector<STTriangleMesh*>& output_meshes, const std::string& filename);
    
    float mMaterialAmbient[4];
    float mMaterialDiffuse[4];
    float mMaterialSpecular[4];
    float mShininess;  // # between 1 and 128.
	STImage * mSurfaceColorImg;
	STTexture * mSurfaceColorTex;

    static STPoint3 GetMassCenter(const std::vector<STTriangleMesh*>& input_meshes);
    static std::pair<STPoint3,STPoint3> GetBoundingBox(const std::vector<STTriangleMesh*>& input_meshes);
    void Recenter(const STPoint3& center);
    float mSurfaceArea;
    STPoint3 mMassCenter;
    STPoint3 mBoundingBoxMax;
    STPoint3 mBoundingBoxMin;

    bool mDrawAxis;
    const static float red[];
    const static float green[];
    const static float blue[];
    const static float black[];
    const static float white[];
    static int instance_count;
	static STImage whiteImg;
	static STTexture* whiteTex;

	float** moistureForColor;
};

#endif  // __STTRIANGLEMESH_H__

