//
//  CZNode.cpp
//  Application3D
//
//  Created by CharlyZhang on 16/6/29.
//  Copyright © 2016年 CharlyZhang. All rights reserved.
//

#include "CZNode.h"


void CZNode::resetMatrix()
{
    rotateMat.LoadIdentity();
    translateMat.LoadIdentity();
    scaleMat.LoadIdentity();
}