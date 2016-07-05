//
//  CZCube.hpp
//  Application3D
//
//  Created by CharlyZhang on 16/6/29.
//  Copyright © 2016年 CharlyZhang. All rights reserved.
//

#ifndef CZCube_hpp
#define CZCube_hpp

#include "CZShape.h"
#include "../CZBasic.h"
#include "CZFace.hpp"

#include <vector>

class CZCube : public CZShape
{
public:
    CZCube();
    ~CZCube();
    
    bool draw(CZShader *pShader, CZMat4 &viewProjMat) override;
    
    void create(CZPoint3D &origin, float width, float length, float height);
    void fold(float ratio) override {};
    void unFold(float ratio) override;
    
    void resetMatrix() override;
private:
    static unsigned char indices[];
    std::vector<CZFace* > faces;
    std::vector<CZPoint3D> positions,normals;
};

#endif /* CZCube_hpp */
