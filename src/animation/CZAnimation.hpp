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
    
    CZAnimation(double totalTime);
    
    virtual void update(double time) = 0;
    virtual bool start(std::string &name, double time);
    
    bool isPlaying() {return _isPlaying;}
    
    void setNode(CZNode *n) { ptrNode = n;}
    CZNode* getNode() const { return ptrNode; }
    
protected:
    bool _isPlaying;
    double _totalTime;               //< ms
    double _startTime;               //< ms
    std::string _playingName;
    CZNode *ptrNode;
};

#endif /* CZAnimation_hpp */
