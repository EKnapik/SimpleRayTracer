//
//  Geometric.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 2/6/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//

#ifndef Geometric_hpp
#define Geometric_hpp

#include <stdio.h>
#include "glm/vec3.hpp"


class Geometric {
public:
    virtual glm::vec3 getNormal(glm::vec3 pos) = 0;
    virtual float getIntersect(glm::vec3 rayOr, glm::vec3 rayDir) = 0;
    virtual float getDistance(glm::vec3 pos) = 0;
    virtual glm::vec3 getColor(glm::vec3 pos) = 0;
    float timeHit = 0;
};

#endif /* Geometric_hpp */
