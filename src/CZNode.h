//
//  CZNode.h
//  Application3D
//
//  Created by CharlyZhang on 16/6/29.
//  Copyright © 2016年 Founder. All rights reserved.
//

#ifndef CZNode_h
#define CZNode_h

#include "CZMat4.h"
#include "CZShader.h"

class CZNode
{
public:
    // define type
    typedef enum _NodeType {
        kObjModel,                  ///< obj mode
        kShape                      ///< shape
    } NodeType;
    
    CZNode(NodeType t): _type(t){};
    
    void resetMatrix();
    
    NodeType getType(){ return _type;}
    
    virtual void draw(CZShader *pShader) = 0;
    
    //// properties
    CZMat4 rotateMat, translateMat, scaleMat;

private:
    NodeType _type;
};

#endif /* CZNode_h */
