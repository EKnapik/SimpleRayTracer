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

#define COLLISION_DAMPENING float(0.9)
#define COLLISION_CONSTANT_C float(1)

class Geometric {
public:
    virtual glm::vec3 getNormal(glm::vec3 pos) = 0;
    virtual float getIntersect(Ray *ray) = 0;
    virtual float getDistance(glm::vec3 pos) = 0;
    virtual glm::vec3 getColor(glm::vec3 pos) = 0;
    
    virtual bool doesCollideWith(Geometric *obj) = 0;
    // TODO: This Function is very similar for all shapes and shouldn't be virtual
    virtual void mirrorCollisionHandling(Geometric *obj, float timeStep) = 0;
    void applyPhysics(float timeStep) {this->pos += this->velocity * timeStep;};
    bool movable = false;
    
    glm::vec3 pos;
    glm::vec3 velocity;
    glm::vec3 color;
    // lighting
    float diffCoeff;
    float specCoeff;
    float timeHit = -1.0;
    float refractIndex = 1.0;
    bool reflective = false;
    bool transmitive = false;

};

#endif /* Geometric_hpp */
