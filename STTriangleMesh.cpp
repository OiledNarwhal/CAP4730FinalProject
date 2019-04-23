// STTriangleMesh.cpp
#include "STTriangleMesh.h"

/* Include-order dependency!
*
* GLEW must be included before the standard GL.h header.
* In this case, it means we must violate the usual design
* principle of always including Foo.h first in Foo.cpp.
*/
#ifdef __APPLE__
#define GLEW_VERSION_2_0 1
#include <OpenGL/gl.h>
#else
#define GLEW_STATIC
#include "GL/glew.h"
#include "GL/gl.h"
#endif

#include "STTexture.h"
#include <iostream>
#include <fstream>
#include <map>
#include <math.h>
#include <string.h>
#include <algorithm>
#define PI 3.14159265

#include <tiny_obj_loader.h>
using namespace tinyobj;

const float STTriangleMesh::red[]  ={1.0f,0.0f,0.0f,1.0f};
const float STTriangleMesh::green[]={0.0f,1.0f,0.0f,1.0f};
const float STTriangleMesh::blue[] ={0.0f,0.0f,1.0f,1.0f};
const float STTriangleMesh::black[]={0.0f,0.0f,0.0f,1.0f};
const float STTriangleMesh::white[]={1.0f,1.0f,1.0f,1.0f};

//Terrain Colors:

//Lowest Colors
const GLfloat ocean[] = {5.0f/255, 2.0f/255, 107.0f/255, 1.0f};
const GLfloat beach[] = { 232.0f / 255, 213.0f / 255, 132.0f / 255, 1.0f };

//High Colors
const GLfloat rock[] = { 33.0f / 255, 32.0f / 255, 32.0f / 255, 1.0f };
const GLfloat wetRock[] = { 68.0f / 255, 68.0f / 255, 66.0f / 255, 1.0f };
const GLfloat snow[] = { 229.0f / 255, 228.0f / 255, 227.0f / 255, 1.0f };

//Middle - high Colors
const GLfloat elevatedGrass[] = { 201.0f / 255, 234.0f / 255, 161.0f / 255, 1.0f };
const GLfloat elevatedForest[] = { 128.0f / 255, 163.0f / 255, 86.0f / 255, 1.0f };
const GLfloat elevatedForestMoist[] = { 82.0f / 255, 114.0f / 255, 43.0f / 255, 1.0f };

//Middle Colors
const GLfloat shrubs[] = { 100.0f / 255, 145.0f / 255, 46.0f / 255, 1.0f };
const GLfloat middleForest[] = { 55.0f / 255, 119.0f / 255, 49.0f / 255, 1.0f };
const GLfloat middleForestDense[] = { 25.0f / 255, 66.0f / 255, 31.0f / 255, 1.0f };

//Low Colors
const GLfloat scorched[] = { 183.0f / 255, 162.0f / 255, 114.0f / 255, 1.0f };
const GLfloat lowForest[] = { 8.0f / 255, 124.0f / 255, 30.0f / 255, 1.0f };
const GLfloat jungle[] = { 1.0f / 255, 51.0f / 255, 10.0f / 255, 1.0f };

/*
Color Names:
High: rock, wetRock, snow
Middle-High: elevatedGrass, elevatedForest, elevatedForestMoist
Middle: shrubs, middleForest, middleForestDense
Low: scorched, lowForest, jungle
Lowest: ocean, beach
*/

int STTriangleMesh::instance_count=0;
STImage STTriangleMesh::whiteImg=STImage(256, 256, STImage::Pixel(255,255,255,255));
STTexture* STTriangleMesh::whiteTex = 0;
//
// Initialization
//
STTriangleMesh::STTriangleMesh()
{
    mSimpleMesh = true;
    for(int i=0;i<3;i++){
        mMaterialAmbient[i]=0.2f;
        mMaterialDiffuse[i]=0.8f;
        mMaterialSpecular[i]=0.1f;
    }
    mMaterialAmbient[3]=1.0f;
    mMaterialDiffuse[3]=1.0f;
    mMaterialSpecular[3]=1.0f;
    mShininess = 1.;  // # between 1 and 128.

    mDrawAxis = true;

    mSurfaceColorImg=&whiteImg;
    if(instance_count==0){
        whiteTex = new STTexture(&whiteImg);
    }
    instance_count++;
    mSurfaceColorTex=whiteTex;
}

STTriangleMesh::STTriangleMesh(const std::string& filename)
{
    STTriangleMesh();
    Read(filename);
    Build();
}

//
// Delete and clean up an existing image.
//
STTriangleMesh::~STTriangleMesh()
{
    for(unsigned int i=0;i<mVertices.size();i++)delete mVertices[i];
    for(unsigned int i=0;i<mTexPos.size();i++)delete mTexPos[i];
    for(unsigned int i=0;i<mNormals.size();i++)delete mNormals[i];
    for(unsigned int i=0;i<mFaces.size();i++)delete mFaces[i];
	if(mSurfaceColorTex!=whiteTex)delete mSurfaceColorTex;
	if(mSurfaceColorImg!=&whiteImg)delete mSurfaceColorImg;

    instance_count--;
    if(instance_count==0){
        delete whiteTex;
    }
}

//
// Draw the triangle mesh to the OpenGL window using GL_TRIANGLES.
//
void STTriangleMesh::Draw(bool smooth) const
{
    if(mDrawAxis){
        glPushAttrib(GL_LIGHTING_BIT);
        glActiveTexture(GL_TEXTURE2);
        whiteTex->Bind();

        glEnable(GL_LIGHTING);
        glLightfv(GL_LIGHT0, GL_AMBIENT,     white);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   black);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  black);

        glPushMatrix();
        STVector3 size_vector=mBoundingBoxMax-mBoundingBoxMin;
        float maxSize=(std::max)((std::max)(size_vector.x,size_vector.y),size_vector.z);
        glScalef(maxSize/200.0f,maxSize/200.0f,maxSize/200.0f);

        GLUquadricObj* cylinder = gluNewQuadric();

        glMaterialfv(GL_FRONT, GL_AMBIENT,   red);
        glPushMatrix();
        glRotatef(90.0f,0,1,0);
        gluCylinder(cylinder, 1.0, 1.0, 200, 10, 1);
        glPopMatrix();

        glMaterialfv(GL_FRONT, GL_AMBIENT,   green);
        glPushMatrix();
        glRotatef(-90.0f,1,0,0);
        gluCylinder(cylinder, 1.0, 1.0, 200, 10, 1);
        glPopMatrix();

        glMaterialfv(GL_FRONT, GL_AMBIENT,   blue);
        glPushMatrix();
        gluCylinder(cylinder, 1.0, 1.0, 200, 10, 1);
        glPopMatrix();

        glPopMatrix();

        glActiveTexture(GL_TEXTURE2);
        whiteTex->UnBind();
        glPopAttrib();
    }

    glActiveTexture(GL_TEXTURE2);
    mSurfaceColorTex->Bind();
	/*
    glMaterialfv(GL_FRONT, GL_AMBIENT,   mMaterialAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mMaterialDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mMaterialSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &mShininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green);

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);// So was this.
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); //This was also added.
	*/
	//glEnable(GL_COLOR_MATERIAL);// So was this.
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); //This was also added.
	glEnable(GL_COLOR_MATERIAL);// So was this.
	

	glActiveTexture(GL_TEXTURE2);
	mSurfaceColorTex->UnBind();
	//glBindTexture(GL_TEXTURE_2D, NULL);
	//glColor3f(1.0f, 0.0f, 0.0f);
	//glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_LIGHT0);
	whiteTex->UnBind();
    glBegin(GL_TRIANGLES);
    for (unsigned int i = 0; i < mFaces.size(); i++) {
        STFace* f=mFaces[i];
        for (unsigned int j = 0; j < 3; j++) {
            if(smooth)
                if(!mSimpleMesh)
                    glNormal3f(f->normals[j]->x,f->normals[j]->y,f->normals[j]->z);
                else 
                    glNormal3f(f->v[j]->normal.x,f->v[j]->normal.y,f->v[j]->normal.z);
            else
                glNormal3f(f->normal.x,f->normal.y,f->normal.z);
            
            glTexCoord2f(f->texPos[j]->x, f->texPos[j]->y);
			
			
			/*
			if (i > 50000)
				glColor3f(1.0f, 0.0f, 0.0f); // This was added
			else
				glColor3f(0.0f, 1.0f, 1.0f);
			*/

			/*
			Color Names:
			High: rock, wetRock, snow
			Middle-High: elevatedGrass, elevatedForest, elevatedForestMoist
			Middle: shrubs, middleForest, middleForestDense
			Low: scorched, lowForest, jungle
			Lowest: ocean, beach
			*/
			float x1 = f->v[j]->pt.x;
			float y = f->v[j]->pt.y;
			float z1 = f->v[j]->pt.z;

			int x = (x1 * 8 * 1025);
			int z = (z1 * 8 * 1025);

			//std::cout << "Height: " << y << "\n";
			std::cout << "X1: " << x1 << "\n";
			//std::cout << " STTriangleMesh Moisture: " << moistureForColor[x][z] << "\n";


			if (y <= .15)
				glColor3f(ocean[0], ocean[1], ocean[2]);
			else if(y <= .155)
				glColor3f(beach[0], beach[1], beach[2]);
			else if (y <= .5)
			{
				//std::cout << "Y >.1 \n";
				if (moistureForColor[x][z] < .3)
				{
					glColor3f(scorched[0], scorched[1], scorched[2]);
				}
				else if (moistureForColor[x][z] < .6)
				{
					glColor3f(lowForest[0], lowForest[1], lowForest[2]);
				}
				else
				{
					glColor3f(jungle[0], jungle[1], jungle[2]);
				}
			}
			else if (y <= .65)
			{
				if (moistureForColor[x][z] < .3)
				{
					glColor3f(shrubs[0], shrubs[1], shrubs[2]);
				}
				else if (moistureForColor[x][z] < .6)
				{
					glColor3f(middleForest[0], middleForest[1], middleForest[2]);
				}
				else
				{
					glColor3f(middleForestDense[0], middleForestDense[1], middleForestDense[2]);
				}
			}
			else if (y <= .8)
			{
				if (moistureForColor[x][z] < .3)
				{
					glColor3f(elevatedGrass[0], elevatedGrass[1], elevatedGrass[2]);
				}
				else if (moistureForColor[x][z] < .6)
				{
					glColor3f(elevatedForest[0], elevatedForest[1], elevatedForest[2]);
				}
				else
				{
					glColor3f(elevatedForestMoist[0], elevatedForestMoist[1], elevatedForestMoist[2]);
				}
			}
			else
			{
				if (moistureForColor[x][z] < .3)
				{
					glColor3f(rock[0], rock[1], rock[2]);
				}
				else if (moistureForColor[x][z] < .6)
				{
					glColor3f(wetRock[0], wetRock[1], wetRock[2]);
				}
				else
				{
					glColor3f(snow[0], snow[1], snow[2]);
				}
			}
				


			//glColor3f(0.5, 0.5, 0.5); //This makes it gray.
			//std::cout << "Hello\n";
			
            glVertex3f(f->v[j]->pt.x,f->v[j]->pt.y,f->v[j]->pt.z);
        }
    }
    glEnd();
	glEnable(GL_TEXTURE_2D);

    glActiveTexture(GL_TEXTURE2);
    mSurfaceColorTex->UnBind();
}

//
// Read the triangle mesh from files.
//
bool STTriangleMesh::Read(const std::string& filename)
{
    // Determine the right routine based on the file's extension.
    // The format-specific subroutines are each implemented in
    // a different file.
    std::string ext = STGetExtension( filename );
    if (ext.compare("OBJ") == 0){
        std::ifstream in( filename.c_str(), std::ios::in );

        if( !in ){
            std::cout << "cannot open file" << filename << std::endl;
            return false;
        }

        for(unsigned int i=0;i<mVertices.size();i++)delete mVertices[i];
        mVertices.clear();
        for(unsigned int i=0;i<mTexPos.size();i++)delete mTexPos[i];
        mVertices.clear();
        for(unsigned int i=0;i<mNormals.size();i++)delete mNormals[i];
        mNormals.clear();
        for(unsigned int i=0;i<mFaces.size();i++)delete mFaces[i];
        mFaces.clear();

        //std::string comments;
        //std::string token;
        char comments[256];
        char token[128];
        float x,y,z;
        int p1,p2,p3;
        int n1,n2,n3;
        while(in>>token){
            if(strcmp(token,"#")==0){
                in.getline(comments,256);
            }
            else if(strcmp(token,"v")==0){
                in>>x>>y>>z;
                mVertices.push_back(new STVertex(x,y,z));
            }
            else if(strcmp(token,"vn")==0){
                in>>x>>y>>z;
                mNormals.push_back(new STVector3(x,y,z));
                mSimpleMesh=false;
            }
            else if(strcmp(token,"f")==0){
                if(mSimpleMesh){
                    in>>p1; in.ignore(100, ' ');
                    in>>p2; in.ignore(100, ' ');
                    in>>p3; 
                    mFaces.push_back(new STFace(mVertices[p1-1],mVertices[p2-1],mVertices[p3-1]));
                }
                else{
                    in>>p1; in.ignore(100, '/'); in.ignore(100, '/'); in>>n1;
                    in>>p2; in.ignore(100, '/'); in.ignore(100, '/'); in>>n2;
                    in>>p3; in.ignore(100, '/'); in.ignore(100, '/'); in>>n3;
                    mFaces.push_back(new STFace(mVertices[p1-1],mVertices[p2-1],mVertices[p3-1],
                                mNormals[n1-1],mNormals[n2-1],mNormals[n3-1]));
                }
            }
        }
        return true;
    }
    else {
        fprintf(stderr,
            "STTriangleMesh::STTriangleMesh() - Unknown file type \"%s\".\n",
            filename.c_str());
        return false;
    }
}

//
// Write the triangle mesh to files.
//
unsigned int getIndex(std::vector<STVertex*>& mVertices, const STVertex* v){
    return std::find(mVertices.begin(),mVertices.end(),v)-mVertices.begin()+1;
}

bool STTriangleMesh::Write(const std::string& filename)
{
    // Determine the right routine based on the file's extension.
    // The format-specific subroutines are each implemented in
    // a different file.
    std::string ext = STGetExtension( filename );
    if (ext.compare("OBJ") == 0){
        std::ofstream out( filename.c_str(), std::ios::out );

        if( !out ){
            std::cout << "cannot open file" << filename << std::endl;
            return false;
        }

        for(unsigned int i=0;i<mVertices.size();i++){
            out<<"v "<<mVertices[i]->pt.x<<" "<<mVertices[i]->pt.y<<" "<<mVertices[i]->pt.z<<" "<<std::endl;
        }
        for(unsigned int i=0;i<mFaces.size();i++){
            out<<"f "<<getIndex(mVertices,mFaces[i]->v[0])<<" "<<getIndex(mVertices,mFaces[i]->v[1])<<" "<<getIndex(mVertices,mFaces[i]->v[2])<<" "<<std::endl;
        }

        return true;
    }
    else {
        fprintf(stderr,
            "STTriangleMesh::STTriangleMesh() - Unknown file type \"%s\".\n",
            filename.c_str());
        return false;
    }
}

//
// Build topology  and calculate normals for the triangle mesh.
//
bool STTriangleMesh::Build()
{
    BuildTopology();
    UpdateGeometry();
    return true;
}


bool STTriangleMesh::BuildTopology()
{
    // this function only works for maniford mesh
    if(!mSimpleMesh) return false;
    //Build topology
    std::map<std::pair<STVertex*,STVertex*>,STFace*> he2f;
    for(unsigned int i=0;i<mFaces.size();i++){
        for(unsigned int j=0;j<3;j++){
            mFaces[i]->v[j]->f=mFaces[i];
            he2f.insert(std::pair<std::pair<STVertex*,STVertex*>,STFace*>(std::pair<STVertex*,STVertex*>(mFaces[i]->v[j],mFaces[i]->v[(j+1)%3]),mFaces[i]));
        }
    }
    for(unsigned int i=0;i<mFaces.size();i++){
        for(unsigned int j=0;j<3;j++){
            std::map<std::pair<STVertex*,STVertex*>,STFace*>::iterator itr=he2f.find(std::pair<STVertex*,STVertex*>(mFaces[i]->v[(j+2)%3],mFaces[i]->v[(j+1)%3]));
            if(itr!=he2f.end())
                mFaces[i]->adjF[j]=itr->second;
            else
                mFaces[i]->adjF[j]=0;
        }
    }
    return true;
}

bool STTriangleMesh::UpdateGeometry()
{
    mMassCenter = STPoint3(0.0f,0.0f,0.0f);
    mSurfaceArea = 0.0f;
    if(mVertices.size()>0){
        mBoundingBoxMin=mBoundingBoxMax=mVertices[0]->pt;
        for(unsigned int i=1;i<mVertices.size();i++){
            mBoundingBoxMin=STPoint3::Min(mBoundingBoxMin,mVertices[i]->pt);
            mBoundingBoxMax=STPoint3::Max(mBoundingBoxMax,mVertices[i]->pt);
        }
    }
    else{
        mBoundingBoxMin=STPoint3(0.0f,0.0f,0.0f);
        mBoundingBoxMax=STPoint3(1.0f,1.0f,1.0f);
    }
    for(unsigned int i=0;i<mFaces.size();i++){
        STFace* face=mFaces[i];
        face->normal=STVector3::Cross(face->v[0]->pt-face->v[1]->pt,face->v[0]->pt-face->v[2]->pt);
        float area=face->normal.Length();
        mSurfaceArea+=area;
        mMassCenter=mMassCenter+(face->v[0]->pt+face->v[1]->pt+face->v[2]->pt)*(area/3.0f);
        if(mSimpleMesh)
            for(unsigned int j=0;j<3;j++)
                face->v[j]->normal+=face->normal;
    }
    mMassCenter=mMassCenter/mSurfaceArea;
    for(unsigned int i=0;i<mFaces.size();i++){
        mFaces[i]->normal.Normalize();
    }
    if(mSimpleMesh){
        for(unsigned int i=0;i<mVertices.size();i++){
            mVertices[i]->normal.Normalize();
        }
    }
    return true;
}

bool STTriangleMesh::CalculateTextureCoordinatesViaSphericalProxy()
{
	for(unsigned int i=0;i<mFaces.size();i++){
		STFace* face=mFaces[i];		
		for(int v=0;v<3;v++){
			STPoint3 point=face->v[v]->pt;
			float r=sqrt(pow(point.x,2)+pow(point.y,2)+pow(point.z,2));
			float theta=(float)PI-acos(point.z/r);
			float phi=atan2(point.y,point.x)+(float)PI;
			face->texPos[v]->x=phi/(float)2/(float)PI;
			face->texPos[v]->y=theta/PI;
		}
		if(face->texPos[1]->x-face->texPos[0]->x>.5) face->texPos[1]->x-=1;
		else if(face->texPos[1]->x-face->texPos[0]->x<-.5) face->texPos[1]->x+=1;

		if(face->texPos[2]->x-face->texPos[0]->x>.5) face->texPos[2]->x-=1;
		else if(face->texPos[2]->x-face->texPos[0]->x<-.5) face->texPos[2]->x+=1;
	}
	return true;
}


STFace* STTriangleMesh::NextAdjFace(STVertex *v, STFace *f)
{
    if( v == f->v[0] ) 
        return f->adjF[1];
    else if( v == f->v[1] ) 
        return f->adjF[2];
    else if( v == f->v[2] ) 
        return f->adjF[0];
    else 
        return 0;
}

STFace* STTriangleMesh::NextAdjFaceReverse(STVertex *v, STFace *f)
{
    if( v == f->v[0] ) 
        return f->adjF[2];
    else if( v == f->v[1] ) 
        return f->adjF[0];
    else if( v == f->v[2] ) 
        return f->adjF[1];
    else 
        return 0;
}

void STTriangleMesh::LoopSubdivide()
{
    if(!mSimpleMesh) return;
    unsigned int newVerticesStart=mVertices.size();

    // Add Odd Vertices
    std::map<STFace*,std::vector<STVertex *> > oldFaces2newVertices;
    std::vector<STVertex *> empty_vertices_3;for(unsigned int i=0;i<3;i++)empty_vertices_3.push_back(0);
    for(unsigned int i=0;i<mFaces.size();i++){
        oldFaces2newVertices.insert(std::pair<STFace*,std::vector<STVertex *> >(mFaces[i],empty_vertices_3) );
    }
    for(unsigned int i=0;i<mFaces.size();i++){
        for(unsigned int j=0;j<3;j++){
            if(oldFaces2newVertices[mFaces[i]][j]==0){
                if(mFaces[i]->adjF[j]!=0){
                    int adjF_j;
                    for(unsigned int k=0;k<3;k++){
                        if(mFaces[i]->adjF[j]->adjF[k]==mFaces[i]){
                            adjF_j=k;
                            break;
                        }
                    }
                    STPoint3 newPoint=(mFaces[i]->v[(j+1)%3]->pt+mFaces[i]->v[(j+2)%3]->pt)*0.375f
                        +(mFaces[i]->v[j]->pt+mFaces[i]->adjF[j]->v[adjF_j]->pt)*0.125f;
                    STPoint2 newTexPos=(mFaces[i]->v[(j+1)%3]->texPos+mFaces[i]->v[(j+2)%3]->texPos)*0.5f;
                    STVertex* newVertex=new STVertex(newPoint,newTexPos);
                    mVertices.push_back(newVertex);
                    oldFaces2newVertices[mFaces[i]][j]=oldFaces2newVertices[mFaces[i]->adjF[j]][adjF_j]=newVertex;
                }
                else{
                    STPoint3 newPoint=(mFaces[i]->v[(j+1)%3]->pt+mFaces[i]->v[(j+2)%3]->pt)*0.5f;
                    STPoint2 newTexPos=(mFaces[i]->v[(j+1)%3]->texPos+mFaces[i]->v[(j+2)%3]->texPos)*0.5f;
                    STVertex* newVertex=new STVertex(newPoint,newTexPos);
                    mVertices.push_back(newVertex);
                    oldFaces2newVertices[mFaces[i]][j]=newVertex;
                }
            }
        }
    }

    std::vector<STPoint3> newEvenVerticesPoints;newEvenVerticesPoints.resize(newVerticesStart);
    // Adjust Even Vertices
    for(unsigned int i=0;i<newVerticesStart;i++){
        STVertex* vertex=mVertices[i];
        STFace* nextface=vertex->f;
        std::vector<STPoint3> neighborPoints;
        bool boundary=false;
        do {
            if(nextface==0){
                boundary=true;
                break;
            }
            for(int j=0;j<3;j++){
                if(nextface->v[j]==vertex){
                    neighborPoints.push_back(nextface->v[(j+2)%3]->pt);
                    break;
                }
            }
        } while((nextface=NextAdjFace(vertex,nextface))!=vertex->f);

        if(boundary){
            STPoint3 temp=neighborPoints.back();
            neighborPoints.clear();
            neighborPoints.push_back(temp);
            nextface=vertex->f;
            do {
                if(nextface==0)
                    break;
                for(int j=0;j<3;j++){
                    if(nextface->v[j]==vertex){
                        temp=nextface->v[(j+1)%3]->pt;
                        break;
                    }
                }
            } while((nextface=NextAdjFaceReverse(vertex,nextface))!=vertex->f);
            neighborPoints.push_back(temp);
        }

        if(neighborPoints.size()>3){
            float weight=3.0f/8.0f/(float)neighborPoints.size();
            newEvenVerticesPoints[i]=vertex->pt*(5.0f/8.0f);
            for(unsigned j=0;j<neighborPoints.size();j++)
                newEvenVerticesPoints[i]=newEvenVerticesPoints[i]+neighborPoints[j]*weight;
        }
        else if(neighborPoints.size()==3){
            float weight=3.0f/16.0f;
            newEvenVerticesPoints[i]=vertex->pt*(7.0f/16.0f);
            for(unsigned j=0;j<neighborPoints.size();j++)
                newEvenVerticesPoints[i]=newEvenVerticesPoints[i]+neighborPoints[j]*weight;
        }
        else{ // assert(neighborPoints.size()==2) boundary vertex
            newEvenVerticesPoints[i]=vertex->pt*0.75f+neighborPoints[0]*0.125f+neighborPoints[1]*0.125f;
        }
    }
    for(unsigned int i=0;i<newVerticesStart;i++)
        mVertices[i]->pt=newEvenVerticesPoints[i];

    // Rebuild faces
    std::vector<STFace*> newFaces;
    for(unsigned int i=0;i<mFaces.size();i++){
        for(unsigned int j=0;j<3;j++){
            STFace* newFace=new STFace(mFaces[i]->v[j],oldFaces2newVertices[mFaces[i]][(j+2)%3],oldFaces2newVertices[mFaces[i]][(j+1)%3]);
            newFaces.push_back(newFace);
        }
        STFace* newFace=new STFace(oldFaces2newVertices[mFaces[i]][0],oldFaces2newVertices[mFaces[i]][1],oldFaces2newVertices[mFaces[i]][2]);
        newFaces.push_back(newFace);
    }
    std::swap(mFaces,newFaces);
    for(unsigned int i=0;i<newFaces.size();i++)delete newFaces[i];
    
    Build();
}

unsigned int STTriangleMesh::AddVertex(float x, float y, float z, float u, float v)
{
    mVertices.push_back(new STVertex(x,y,z,u,v));
    return mVertices.size()-1;
}

unsigned int STTriangleMesh::AddVertex(const STPoint3& pt, const STPoint2& texPos)
{
    mVertices.push_back(new STVertex(pt,texPos));
    return mVertices.size()-1;
}

unsigned int STTriangleMesh::AddFace(unsigned int id0,unsigned int id1,unsigned int id2)
{
    STFace* newFace = new STFace(mVertices[id0],mVertices[id1],mVertices[id2]);
    mFaces.push_back(newFace);
    return mFaces.size()-1;
}

std::ostream& operator <<(std::ostream& stream, const STVertex& v) {
    stream<<"["<<v.pt.x<<" "<<v.pt.y<<" "<<v.pt.z<<"]";
    return stream;
}

std::ostream& operator <<(std::ostream& stream, const STFace& f) {
    stream<<"{"<<*(f.v[0])<<" "<<*(f.v[1])<<" "<<*(f.v[2])<<"}";
    return stream;
}

std::string STTriangleMesh::LoadObj(std::vector<STTriangleMesh*>& output_meshes, const std::string& filename){
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
	std::string base;
	size_t l;
	if ((l = filename.find_last_of('/')) != std::string::npos)
		base = filename.substr(0, l+1);
	else if ((l = filename.find_last_of('\\')) != std::string::npos)
		base = filename.substr(0, l+1);
	std::string err = tinyobj::LoadObj(shapes, materials, filename.c_str(), base.c_str());
    
    std::cout<<"#shapes="<<shapes.size()<<" #materials="<<materials.size()<<std::endl;
    

    for(unsigned int mesh_id=0; mesh_id<shapes.size(); mesh_id++)
    {
        tinyobj::mesh_t& mesh=shapes[mesh_id].mesh;
        STTriangleMesh* stmesh = new STTriangleMesh();
        for(unsigned int vertex_id=0; vertex_id<mesh.positions.size()/3; vertex_id++)
            stmesh->mVertices.push_back(new STVertex(mesh.positions[vertex_id*3],
                                                     mesh.positions[vertex_id*3+1],
                                                     mesh.positions[vertex_id*3+2]));
        for(unsigned int face_id=0; face_id<mesh.indices.size()/3; face_id++){
            int indices[3];for(int j=0;j<3;j++)indices[j]=mesh.indices[face_id*3+j];
            stmesh->mFaces.push_back(new STFace(stmesh->mVertices[indices[0]],
                                                stmesh->mVertices[indices[1]],
                                                stmesh->mVertices[indices[2]]));
        }
        if(mesh.normals.size()>0){
            stmesh->mSimpleMesh=false;
            for(unsigned int normal_id=0; normal_id<mesh.normals.size()/3; normal_id++)
                stmesh->mNormals.push_back(new STVector3(mesh.normals[normal_id*3],
                                                         mesh.normals[normal_id*3+1],
                                                         mesh.normals[normal_id*3+2]));
            for(unsigned int face_id=0; face_id<mesh.indices.size()/3; face_id++){
                for(int j=0;j<3;j++)
                    stmesh->mFaces[face_id]->normals[j]=stmesh->mNormals[mesh.indices[face_id*3+j]];
            }
        }
        if(mesh.texcoords.size()>0){
            for(unsigned int texpos_id=0; texpos_id<mesh.texcoords.size()/2; texpos_id++)
                stmesh->mTexPos.push_back(new STPoint2(mesh.texcoords[texpos_id*2],
                                                         mesh.texcoords[texpos_id*2+1]));
            for(unsigned int face_id=0; face_id<mesh.indices.size()/3; face_id++){
                for(int j=0;j<3;j++)
                    stmesh->mFaces[face_id]->texPos[j]=stmesh->mTexPos[mesh.indices[face_id*3+j]];
            }
        }
        stmesh->Build();
		if(/*mesh.material_ids.size()>0&&*/mesh.material_ids[0]>=0){
            tinyobj::material_t& material=materials[mesh.material_ids[0]];
            for(int i=0;i<3;i++){
                stmesh->mMaterialAmbient[i]=material.ambient[i];
                stmesh->mMaterialDiffuse[i]=material.diffuse[i];
                stmesh->mMaterialSpecular[i]=material.specular[i];
            }
			std::string colorMap = material.diffuse_texname;
			if (colorMap != "") {
				stmesh->mSurfaceColorImg = new STImage(base+colorMap);
		        stmesh->mSurfaceColorTex = new STTexture(stmesh->mSurfaceColorImg,STTexture::kNone);
			}
  		
            stmesh->mShininess = 8.;  // # between 1 and 128.
        }

        output_meshes.push_back(stmesh);
    }

    return err;
}




STPoint3 STTriangleMesh::GetMassCenter(const std::vector<STTriangleMesh*>& input_meshes)
{
    STPoint3 massCenter=STPoint3(0.0,0.0,0.0);
    float totalArea=0.0f;
    for(unsigned int i = 0; i<input_meshes.size(); i++){
        massCenter = massCenter + input_meshes[i]->mMassCenter * input_meshes[i]->mSurfaceArea;
        totalArea+=input_meshes[i]->mSurfaceArea;
    }
    return massCenter/totalArea;
}

std::pair<STPoint3,STPoint3> STTriangleMesh::GetBoundingBox(const std::vector<STTriangleMesh*>& input_meshes)
{
    if(input_meshes.size()>0){
        STPoint3 boundingMin=input_meshes[0]->mBoundingBoxMin;
        STPoint3 boundingMax=input_meshes[0]->mBoundingBoxMax;
        for(unsigned int i = 0; i<input_meshes.size(); i++){
            boundingMin = STPoint3::Min(boundingMin,input_meshes[i]->mBoundingBoxMin);
            boundingMax = STPoint3::Max(boundingMin,input_meshes[i]->mBoundingBoxMax);
        }
        return std::pair<STPoint3,STPoint3>(boundingMin,boundingMax);
    }
    else
        return std::pair<STPoint3,STPoint3>(STPoint3(0.0,0.0,0.0),STPoint3(1.0,1.0,1.0));
}

void STTriangleMesh::Recenter(const STPoint3& center)
{
    STVector3 translate = STPoint3::Origin - center;
    for(unsigned int i=0;i<mVertices.size();i++){
        mVertices[i]->pt+=translate;
    }
    mMassCenter+=translate;
    mBoundingBoxMax+=translate;
    mBoundingBoxMin+=translate;
}
