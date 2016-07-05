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

unsigned char CZCube::indices[] = {7,6,3,2,     //< top
                                    0,1,2,3,    //< front
                                    1,5,3,7,    //< right
                                    4,0,6,2,    //< left
                                    5,4,7,6,    //< back
                                    1,0,5,4     //< bottom
};

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

void CZCube::unFold(float ratio)
{
    CZShape::unFold(ratio);
    
    float totalAngle = -90;
    
    // step 1 - top face
    if(ratio <= 0.2f)
    {
        CZVector3D<float > from = positions[faces[0]->indexes[0]];
        CZVector3D<float > to = positions[faces[0]->indexes[1]];
        float angle = (ratio - 0.0f) / 0.2f * totalAngle;
        faces[0]->setRotateAroundAxis(angle, from, to);
    }
    
    // step 2 - front
    else if(ratio <= 0.4f)
    {
        CZVector3D<float > from = positions[faces[1]->indexes[0]];
        CZVector3D<float > to = positions[faces[1]->indexes[1]];
        float angle = (ratio - 0.2f) / 0.2f * totalAngle;
        faces[1]->setRotateAroundAxis(angle, from, to);
    }
    
    // step 3 - left
    else if(ratio <= 0.6f)
    {
        CZVector3D<float > from = positions[faces[2]->indexes[0]];
        CZVector3D<float > to = positions[faces[2]->indexes[1]];
        float angle = (ratio - 0.4f) / 0.2f * totalAngle;
        faces[2]->setRotateAroundAxis(angle, from, to);
    }
    
    // step 4 - right
    else if(ratio <= 0.8f)
    {
        CZVector3D<float > from = positions[faces[3]->indexes[0]];
        CZVector3D<float > to = positions[faces[3]->indexes[1]];
        float angle = (ratio - 0.6f) / 0.2f * totalAngle;
        faces[3]->setRotateAroundAxis(angle, from, to);
    }
    
    // step 5 - front & back
    else if(ratio <= 1.0f)
    {
        CZVector3D<float > from = positions[faces[4]->indexes[0]];
        CZVector3D<float > to = positions[faces[4]->indexes[1]];
        float angle = (ratio - 0.8f) / 0.2f * totalAngle;
        faces[4]->setRotateAroundAxis(angle, from, to);
        
        CZVector3D<float > from1 = positions[faces[0]->indexes[0]];
        CZVector3D<float > to1 = positions[faces[0]->indexes[1]];
        faces[0]->setRotateAroundAxis(-90.f, from1, to1);
        faces[0]->rotateAroundAxis(angle, from, to);
    }
    else
        isAnimating = false;
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

void CZCube::resetMatrix()
{
    CZNode::resetMatrix();
    for(vector<CZFace*>::iterator itr = faces.begin(); itr != faces.end(); itr ++)
    {
        (*itr)->resetMatrix();
    }
}