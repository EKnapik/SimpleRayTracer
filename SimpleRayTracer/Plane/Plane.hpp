//
//  Plane.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/19/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#ifndef Plane_hpp
#define Plane_hpp

#include <stdio.h>
#include <math.h>
#include "Geometric.hpp"
#include "glm/vec3.hpp"
#include "glm/glm.hpp"

class Plane: public Geometric {
public:
    Plane();
    Plane(glm::vec3 pos);
    Plane(glm::vec3 pos, glm::vec3 normal);
    Plane(glm::vec3 pos, glm::vec3 normal, glm::vec3 color);
    
    // Geometric
    glm::vec3 getNormal(glm::vec3 pos);
    float getIntersect(Ray *ray);
    float getDistance(glm::vec3 pos);
    glm::vec3 getColor(glm::vec3 pos);
    // Collision
    bool doesCollideWith(Geometric *obj);
    void mirrorCollisionHandling(Geometric *obj, float timeStep);
    
    void setNormal(glm::vec3 normal);
    void setPos(glm::vec3 pos);
    void setColor(glm::vec3 color);
    void setxLimit(glm::vec2 newxLim);
    void setyLimit(glm::vec2 newyLim);
    void setzLimit(glm::vec2 newzLim);
private:
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 xLimit = glm::vec2(-10000, 10000);
    glm::vec2 yLimit = glm::vec2(-10000, 10000);
    glm::vec2 zLimit = glm::vec2(-10000, 10000);
    // lamda material function
};


#endif /* Plane_hpp */
