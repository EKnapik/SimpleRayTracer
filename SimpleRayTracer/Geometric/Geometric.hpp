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
#include <iostream>
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Ray.hpp"


#define COLLISION_DAMPENING float(0.9)
#define COLLISION_CONSTANT_C float(1)

enum PlaneType {XY, YZ, XZ};
enum ObjType {Triangle_type, Plane_type, Sphere_type};

class Geometric {
public:
    virtual glm::vec3 getNormal(glm::vec3 pos) = 0;
    virtual float getIntersect(Ray *ray) = 0;
    virtual float getDistance(glm::vec3 pos) = 0;
    virtual glm::vec3 getColor(glm::vec3 pos) = 0;
    
    // Is this object completely less than the given point ?
    virtual bool isLess(glm::vec3 pos, PlaneType pType) = 0;
    
    virtual bool doesCollideWith(Geometric *obj) = 0;
    // TODO: This Function is very similar for all shapes and shouldn't be virtual
    virtual void mirrorCollisionHandling(Geometric *obj, float timeStep) = 0;
    void applyPhysics(float timeStep) {this->pos += this->velocity * timeStep;};
    bool movable = false;
    
    glm::vec3 pos = glm::vec3(0.0);
    glm::vec3 velocity = glm::vec3(0.0);
    glm::vec3 color = glm::vec3(0.0);
    // Collision
    ObjType type;
    float collisionRadius;
    // lighting with basic values
    float diffCoeff = 0.7;
    float specCoeff = 0.3;
    float ambCoeff = 0.1;
    float specExp = 20;
    float kR = 0.75; // reflective coeff
    float kT = 0.8; // transmissive coeff
    float timeHit = -1.0;
    float refractIndex = 1.0;
    bool reflective = false;
    bool transmitive = false;

};

#endif /* Geometric_hpp */
