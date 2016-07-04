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
    
    float kd[4];
    unsigned char *indexes;
};

#endif /* CZFace_hpp */
