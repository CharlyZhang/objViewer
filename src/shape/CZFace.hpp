//
//  CZFace.hpp
//  Application3D
//
//  Created by CharlyZhang on 16/7/4.
//  Copyright © 2016年 CharlyZhang. All rights reserved.
//

#ifndef CZFace_hpp
#define CZFace_hpp

#include "../CZNode.h"
#include "../CZVector.h"
#include <vector>

class CZFace: public CZNode
{
public:
    CZFace();
    ~CZFace();
    
    bool draw(CZShader *pShader, CZMat4 &viewProjMat);
    
    // set rotate around axis ( axis = `to` - `from`)
    void setRotateAroundAxis(float angle, CZVector3D<float > &from, CZVector3D<float > &to);
    void rotateAroundAxis(float angle, CZVector3D<float > &from, CZVector3D<float > &to);
    
    float kd[4];
    unsigned char *indexes;
};

#endif /* CZFace_hpp */
