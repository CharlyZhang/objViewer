//
//  CZShapeAnimation.cpp
//  Application3D
//
//  Created by CharlyZhang on 16/6/29.
//  Copyright © 2016年 CharlyZhang. All rights reserved.
//

#include "CZShapeAnimation.hpp"
#include "../shape/CZShape.h"

CZShapeAnimation::CZShapeAnimation(long totalTime): CZAnimation(totalTime)
{
    
}

void CZShapeAnimation::update(long time)
{
    if(!_isPlaying) return;
    if(time >= _startTime + _totalTime)
    {
        time = _startTime + _totalTime;
        _isPlaying = false;
    }
    
    float ratio = (time - _startTime) * 1.0f / _totalTime;
    
    if(_playingName == "fold")
    {
        CZShape *pShape = dynamic_cast<CZShape*>(ptrNode);
        if(pShape) pShape->fold(ratio);
        else LOG_WARN("ptrNode is not type of CZShape, and cannot be folded!\n");
    }
}


