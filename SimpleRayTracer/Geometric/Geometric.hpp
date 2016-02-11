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
#include "Ray.hpp"


class Geometric {
public:
    virtual glm::vec3 getNormal(glm::vec3 pos) = 0;
    virtual float getIntersect(Ray *ray) = 0;
    virtual float getDistance(glm::vec3 pos) = 0;
    virtual glm::vec3 getColor(glm::vec3 pos) = 0;
    float timeHit = -1.0;
    bool reflective = false;
};

#endif /* Geometric_hpp */
