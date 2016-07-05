//
//  CZAnimation.cpp
//  Application3D
//
//  Created by CharlyZhang on 16/6/29.
//  Copyright © 2016年 CharlyZhang. All rights reserved.
//

#include "CZAnimation.hpp"


CZAnimation::CZAnimation(double totalTime): _isPlaying(false), _totalTime(totalTime)
{
    ptrNode = nullptr;
}

bool CZAnimation::start(std::string &name, double time)
{
    if(ptrNode == nullptr)
    {
        LOG_DEBUG("ptrNode is nullptr!\n");
        return false;
    }
    
    if(_isPlaying) return false;
    
    _isPlaying = true;
    _playingName = name;
    _startTime = time;
    
    LOG_DEBUG("animation start time - %0.3fms\n", _startTime);
    return true;
}