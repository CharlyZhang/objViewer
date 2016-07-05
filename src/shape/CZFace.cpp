//
//  CZFace.cpp
//  Application3D
//
//  Created by CharlyZhang on 16/7/4.
//  Copyright © 2016年 CharlyZhang. All rights reserved.
//

#include "CZFace.hpp"

using namespace std;

CZFace::CZFace()
{
    kd[0] = 1.0f * rand() / RAND_MAX;
    kd[1] = 1.0f * rand() / RAND_MAX;
    kd[2] = 1.0f * rand() / RAND_MAX;
    kd[3] = 1.0f;
    
    indexes = nullptr;
}

CZFace::~CZFace()
{
    if(indexes) delete [] indexes;
}


bool CZFace::draw(CZShader *pShader, CZMat4 &viewProjMat)
{
    if(indexes == nullptr) return false;
    
    CZMat4 modelMat = getTransformMat();
    glUniformMatrix4fv(pShader->getUniformLocation("mvpMat"), 1, GL_FALSE, viewProjMat * modelMat);
    glUniformMatrix4fv(pShader->getUniformLocation("modelMat"), 1, GL_FALSE, modelMat);
    glUniformMatrix4fv(pShader->getUniformLocation("modelInverseTransposeMat"), 1, GL_FALSE, modelMat.GetInverseTranspose());
    
    float ke[4], ka[4], ks[4], Ns = 10.0;
    ka[0] = 0.2;    ka[1] = 0.2;    ka[2] = 0.2;
    ke[0] = 0.0;    ke[1] = 0.0;    ke[2] = 0.0;
    ks[0] = 0.0;    ks[1] = 0.0;    ks[2] = 0.0;
    Ns = 10.0;
    
    glUniform3f(pShader->getUniformLocation("material.kd"), kd[0], kd[1], kd[2]);
    glUniform3f(pShader->getUniformLocation("material.ka"), ka[0], ka[1], ka[2]);
    glUniform3f(pShader->getUniformLocation("material.ke"), ke[0], ke[1], ke[2]);
    glUniform3f(pShader->getUniformLocation("material.ks"), ks[0], ks[1], ks[2]);
    glUniform1f(pShader->getUniformLocation("material.Ns"), Ns);
    glUniform1i(pShader->getUniformLocation("hasTex"), 0);
    glDrawElements(GL_TRIANGLE_STRIP, 4,  GL_UNSIGNED_BYTE, indexes);

    return true;
}

void CZFace::setRotateAroundAxis(float angle, CZVector3D<float > &from, CZVector3D<float > &to)
{
    rotateMat.LoadIdentity();
    rotateAroundAxis(angle, from, to);
}

void CZFace::rotateAroundAxis(float angle, CZVector3D<float > &from, CZVector3D<float > &to)
{
    CZMat4 mat,tempMat;
    CZVector3D<float > l = to - from;
    VECTOR3D axis(l.x, l.y, l.z);
    mat.SetTranslation(from.x, from.y, from.z);
    tempMat.SetRotationAxis(angle, axis);
    mat = mat * tempMat;
    tempMat.SetTranslation(-from.x, -from.y, -from.z);
    mat = mat * tempMat;
    rotateMat = mat * rotateMat;
}

