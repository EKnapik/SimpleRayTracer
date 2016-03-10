//
//  Triangle.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 3/9/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//

#include "Triangle.hpp"





bool Triangle::doesCollideWith(Geometric *obj) {
    float dist = obj->getDistance(this->pos);
    if(dist < 0.001) {
        return true;
    }
    return false;
}

void Triangle::mirrorCollisionHandling(Geometric *obj, float timeStep) {
    float dist = obj->getDistance(this->pos);
    glm::vec3 nor = obj->getNormal(this->pos);
    float normalCheck = glm::dot(this->velocity, nor);
    if(dist < 0.001 && normalCheck < 0) {
        // reverse velocity
        this->pos -= this->velocity * timeStep;
        this->velocity = COLLISION_DAMPENING * glm::reflect(this->velocity, nor);
    }
}