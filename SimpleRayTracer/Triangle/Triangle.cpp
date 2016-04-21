//
//  Triangle.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 3/9/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//

#include "Triangle.hpp"


Triangle::Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
    this->v1 = v1;
    this->v2 = v2;
    this->v3 = v3;
    glm::vec3 surfaceNorm = -glm::normalize(glm::cross(v2-v1, v3-v1));
    this->vNorm1 = surfaceNorm;
    this->vNorm2 = surfaceNorm;
    this->vNorm3 = surfaceNorm;
    
    this->pos = (v1 + v2 + v3) / float(3.0);
    this->color = glm::vec3(4.9, 0.1, 0.1);
    this->diffCoeff = .9454545;
    this->specCoeff = .545454;
}

Triangle::Triangle(glm::vec3 v1, glm::vec3 vNorm1, glm::vec3 v2, glm::vec3 vNorm2, glm::vec3 v3, glm::vec3 vNorm3) {
    // positional vertex data setting
    this->v1 = v1;
    this->v2 = v2;
    this->v3 = v3;
    // normal vertex data setting
    this->vNorm1 = vNorm1;
    this->vNorm2 = vNorm2;
    this->vNorm3 = vNorm3;
    
    this->pos = (v1 + v2 + v3) / float(3.0);
    this->color = glm::vec3(1.0, 0.1, 0.1);
    this->diffCoeff = .9454545;
    this->specCoeff = .545454;
}

// Interpolates between the three vertex normals to get the normal at this
// position on the triangle face
// For flat surface all the triangle normals should be the surface normal
// Computes the triangle normal through barycentric coordinates
glm::vec3 Triangle::getNormal(glm::vec3 pos) {
    // Pos=Av1+Bv2+Cv3
    // Nor=AvNorm1+BvNorm2+CvNorm3
    // Where A, B, C are area scalars
    
    // using the parallelogram area instead of triangle area for optimization
    float parallelogramArea = glm::length(glm::cross(this->v2-this->v1, this->v3-this->v1));
    float A = glm::length(glm::cross(this->v3-this->v2, pos-this->v2)) / parallelogramArea;
    float B = glm::length(glm::cross(this->v1-this->v3, pos-this->v3)) / parallelogramArea;
    
    // C = 1 - A - B;
    return glm::normalize((A*this->vNorm1 + B*this->vNorm2 + (1-A-B)*this->vNorm3));
}


// THIS CAN BE OPTIMIZED:
// When calculating for triangle intersect the Barycentric coordinates can be
// calculated and saved for this triangle so they do not need to be calculated
// again when getting the normal.
// U and V can be used for Barycentric Coordinates
float Triangle::getIntersect(Ray *ray) {
    float kEpsilon = 0.0001; // Near Zero Value
    
    glm::vec3 edge1 = this->v2 - this->v1;
    glm::vec3 edge2 = this->v3 - this->v1;
    glm::vec3 tVec, pVec, qVec;
    float det, inv_det, U, V, T;
    
    pVec = glm::cross(ray->dir, edge2);
    det = glm::dot(edge1, pVec);
    
    // Cull Testing Method
    if(det > -kEpsilon && det < kEpsilon) {
        return -1.0;
    }
    inv_det = 1.0 / det;
    
    // calculate distance to ray origin
    tVec = ray->pos - this->v1;
    
    // calculate U and test bounds
    U = glm::dot(tVec, pVec) * inv_det;
    if(U < 0.0 || U > 1.0) {
        return -1.0;
    }
    
    // Test V parameter and Check Bounds
    qVec = glm::cross(tVec, edge1);
    V = glm::dot(ray->dir, qVec) * inv_det;
    if(V < 0.0 || U+V > 1.0) {
        return -1.0;
    }
    
    // Calculate T
    T = glm::dot(edge2, qVec) * inv_det;
    return T;
}


float Triangle::getDistance(glm::vec3 pos) {
    return -1.0;
}


// Returns a nice red color
glm::vec3 Triangle::getColor(glm::vec3 pos) {
    return this->color;
}


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