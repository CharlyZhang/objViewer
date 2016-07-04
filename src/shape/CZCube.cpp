//
//  CZCube.cpp
//  Application3D
//
//  Created by CharlyZhang on 16/6/29.
//  Copyright © 2016年 CharlyZhang. All rights reserved.
//

#include "CZCube.hpp"
#include "../CZLog.h"
#include "../CZMat4.h"

using namespace std;

unsigned char CZCube::indices[] = {0,1,2,3,
                                    4,0,6,2,
                                    5,4,7,6,
                                    1,5,3,7,
                                    2,3,6,7,
                                    1,0,5,4};

CZCube::CZCube()
{
}

CZCube::~CZCube()
{
    for(vector<CZFace *>::iterator itr = faces.begin(); itr != faces.end(); itr ++)
        delete  *itr;
    
    positions.clear();
    normals.clear();
}

void CZCube::create(CZPoint3D &origin, float width, float length, float height)
{
    positions.clear();
    normals.clear();
    
    /// create original data
    for(int i = 0; i < 8; i ++)
    {
        int w = i & 1;
        int l = (i & 2) >> 1;
        int h = (i & 4) >> 2;
        
        // points' position and normal
        CZPoint3D offset(width*((float)w-0.5f),length*((float)l-0.5f),height*((float)h-0.5f));
        CZPoint3D p = origin + offset;
        positions.push_back(p);
    
        offset.normalize();
        normals.push_back(offset);
    }
    
    for(auto i = 0; i < 6; i++)
    {
        CZFace *pFace = new CZFace();
        
        pFace->indexes = new unsigned char[4];
        memcpy(pFace->indexes, &indices[i*4], sizeof(unsigned char)*4);
        
        /// treat face as non-subnode, for draw correctly
        faces.push_back(pFace);
        pFace->parentNode = this;
    }
    
    /// transfer to graphic card
    // vao
    GL_GEN_VERTEXARRAY(1, &m_vao);
    GL_BIND_VERTEXARRAY(m_vao);
    
    // vertex
    glGenBuffers(1, &m_vboPos);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboPos);
    glBufferData(GL_ARRAY_BUFFER,positions.size() * 3 * sizeof(GLfloat), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    CZCheckGLError();
    
    // normal
    glGenBuffers(1, &m_vboNorm);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboNorm);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * 3 * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    CZCheckGLError();
    
    GL_BIND_VERTEXARRAY(0);
}

void CZCube::fold(float ratio)
{
    LOG_DEBUG("rotating - %f\n",ratio);
    float totalAngle = -90;
    for(auto i = 0; i < 4; i ++)
    {
        CZMat4 mat,tempMat;
        CZVector3D<float > v0 = positions[faces[i]->indexes[0]];
        CZVector3D<float > v1 = positions[faces[i]->indexes[1]];
        CZVector3D<float > l = v1 - v0;
        VECTOR3D axis(l.x, l.y, l.z);
        mat.SetTranslation(v0.x, v0.y, v0.z);
        tempMat.SetRotationAxis(totalAngle * ratio, axis);
        mat = mat * tempMat;
        tempMat.SetTranslation(-v0.x, -v0.y, -v0.z);
        mat = mat * tempMat;
        faces[i]-> rotateMat = mat;
    }
}

bool CZCube::draw(CZShader *pShader, CZMat4 &viewProjMat)
{
    if(CZNode::draw(pShader, viewProjMat) != true) return false;        ///< render subnodes

    GL_BIND_VERTEXARRAY(m_vao);
    
    for(vector<CZFace* >::iterator itr = faces.begin(); itr != faces.end(); itr ++)
        (*itr)->draw(pShader, viewProjMat);
    
    GL_BIND_VERTEXARRAY(0);
    
    CZCheckGLError();
    
    return true;
}