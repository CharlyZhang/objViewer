//
//  CZNode.cpp
//  Application3D
//
//  Created by CharlyZhang on 16/6/29.
//  Copyright © 2016年 CharlyZhang. All rights reserved.
//

#include "CZNode.h"


CZNode::CZNode(NodeType t /*= kEmpty*/): _type(t)
{
    m_vao = -1;
    m_vboPos = -1;
    m_vboNorm = -1;
    m_vboTexCoord = -1;
}

CZNode::~CZNode()
{
    if(m_vao != -1) GL_DEL_VERTEXARRAY(1, &m_vao);
    if(m_vboPos != -1) glDeleteBuffers(1, &m_vboPos);
    if(m_vboNorm != -1) glDeleteBuffers(1, &m_vboNorm);
    if(m_vboTexCoord != -1) glDeleteBuffers(1, &m_vboTexCoord);
}

void CZNode::resetMatrix()
{
    rotateMat.LoadIdentity();
    translateMat.LoadIdentity();
    scaleMat.LoadIdentity();
}