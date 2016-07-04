//
//  CZAnimation.cpp
//  Application3D
//
//  Created by CharlyZhang on 16/6/29.
//  Copyright © 2016年 CharlyZhang. All rights reserved.
//

#include "CZAnimation.hpp"


CZAnimation::CZAnimation(long totalTime): _isPlaying(false), _totalTime(totalTime)
{
    ptrNode = nullptr;
}

bool CZAnimation::start(std::string &name, long time)
{
    if(ptrNode == nullptr)
    {
        LOG_DEBUG("ptrNode is nullptr!\n");
        return false;
    }
    
    _isPlaying = true;
    _playingName = name;
    _startTime = time;
    return true;
}