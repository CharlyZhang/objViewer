//
//  CZShape.h
//  Application3D
//
//  Created by CharlyZhang on 16/6/29.
//  Copyright © 2016年 Founder. All rights reserved.
//

#ifndef CZShape_h
#define CZShape_h

#include "../CZNode.h"

class CZShape : public CZNode
{
public:
    CZShape(): CZNode(kShape), isAnimating(false){};
  
    virtual void fold(float ratio) { isAnimating = true; };
    virtual void unFold(float ratio) { isAnimating = true; };
    
    /// properties
    bool isAnimating;
};

#endif /* CZShape_h */
