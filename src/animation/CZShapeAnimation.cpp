//
//  CZShapeAnimation.cpp
//  Application3D
//
//  Created by CharlyZhang on 16/6/29.
//  Copyright © 2016年 CharlyZhang. All rights reserved.
//

#include "CZShapeAnimation.hpp"
#include "../shape/CZShape.h"

CZShapeAnimation::CZShapeAnimation(double totalTime): CZAnimation(totalTime)
{
    
}

bool CZShapeAnimation::start(std::string &name, double time)
{
    if(CZAnimation::start(name, time) == false) return false;
    
    if(ptrNode == nullptr)
    {
        LOG_ERROR("ptrNode is nullptr!\n");
        return false;
    }
    
    ptrNode->resetMatrix();
    
    return true;
}

void CZShapeAnimation::update(double time)
{
    if(!_isPlaying) return;
    if(time >= _startTime + _totalTime)
    {
        time = _startTime + _totalTime;
        _isPlaying = false;
    }
    
    LOG_DEBUG("animation update time - %0.3fms\n", time);
    float ratio = (time - _startTime) / _totalTime;
    
    if(_playingName == "unfold")
    {
        CZShape *pShape = dynamic_cast<CZShape*>(ptrNode);
        if(pShape) pShape->unFold(ratio);
        else LOG_WARN("ptrNode is not type of CZShape, and cannot be folded!\n");
    }
}


