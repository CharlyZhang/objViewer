//
//  CZAnimation.hpp
//  Application3D
//
//  Created by CharlyZhang on 16/6/29.
//  Copyright © 2016年 CharlyZhang. All rights reserved.
//

#ifndef CZAnimation_hpp
#define CZAnimation_hpp

#include "../CZNode.h"
#include <string>

class CZAnimation
{
public:
    // define type
    typedef enum _AnimationType {
        kShapeAnimaiton
    } AnimationType;
    
    CZAnimation(long totalTime);
    
    virtual void update(long time) = 0;
    virtual bool start(std::string &name, long time);
    
    bool isPlaying() {return _isPlaying;}
    
    void setNode(CZNode *n) { ptrNode = n;}
    
protected:
    bool _isPlaying;
    long _totalTime;
    long _startTime;
    std::string _playingName;
    CZNode *ptrNode;
};

#endif /* CZAnimation_hpp */
