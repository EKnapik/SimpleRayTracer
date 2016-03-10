//
//  Sphere.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/19/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#ifndef Sphere_hpp
#define Sphere_hpp

#include <stdio.h>
#include <math.h>
#include "Geometric.hpp"
#include "glm/vec3.hpp"
#include "glm/glm.hpp"



class Sphere: public Geometric {
public:
    Sphere();
    Sphere(glm::vec3 pos, float radius);
    Sphere(glm::vec3 pos, float radius, glm::vec3 color);
    
    // Geometric
    glm::vec3 getNormal(glm::vec3 pos);
    float getIntersect(Ray *ray);
    float getDistance(glm::vec3 pos);
    glm::vec3 getColor(glm::vec3 pos);
    // Collision
    bool doesCollideWith(Geometric *obj);
    void mirrorCollisionHandling(Geometric *obj);
    
    void setRadius(float radius);
    void setPos(glm::vec3 pos);
    void setColor(glm::vec3 color);
    
protected:
    float radius;
    // lamda material function
};
#endif /* Sphere_hpp */
