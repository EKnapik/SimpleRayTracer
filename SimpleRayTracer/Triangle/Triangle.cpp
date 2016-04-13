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
    glm::vec3 surfaceNorm = glm::normalize(glm::cross(v2-v1, v3-v1));
    this->vNorm1 = surfaceNorm;
    this->vNorm2 = surfaceNorm;
    this->vNorm3 = surfaceNorm;
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
    return (A*this->vNorm1 + B*this->vNorm2 + (1-A-B)*this->vNorm3);
}


// THIS CAN BE OPTIMIZED:
// When calculating for triangle intersect the Barycentric coordinates can be
// calculated and saved for this triangle so they do not need to be calculated
// again when getting the normal.
float Triangle::getIntersect(Ray *ray) {
    float kEpsilon = 0.01; // Near Zero Value
    
    // compute surface normal
    glm::vec3 surfaceNorm = glm::normalize(glm::cross(this->v2-this->v1, this->v3-this->v1));
    
    // check for ray parallel with triangle
    float nDotRayDir = glm::dot(ray->dir, surfaceNorm);
    if(nDotRayDir < kEpsilon) {
        return -1.0;
    }
    
    float d = glm::dot(surfaceNorm, this->v1);
    // compute t for when the ray hits the triangle plane
    float t = (glm::dot(surfaceNorm, ray->pos) + d) / nDotRayDir;
    // check if the triangle is in behind the ray
    if (t < 0) {
        return -1.0; // the triangle is behind the ray
    }
    
    // Compute Intersection Point
    glm::vec3 intersectP = ray->pos + (t*ray->dir);
    // Now check if P is within the triangle BARYCENTRIC OPTIMIZATION CAN GO HERE
    glm::vec3 C; // Vector perpendicular to triangle's plane
    
    // edge 1
    C = glm::cross((v2-v1), (intersectP-v1));
    if(glm::dot(surfaceNorm, C) < 0) {
        return -1.0; // P is on the right side
    };
    
    // edge 2
    C = glm::cross((v3-v2), (intersectP-v2));
    if(glm::dot(surfaceNorm, C) < 0) {
        return -1.0; // P is on the right side
    };
    
    // edge 3
    C = glm::cross((v1-v3), (intersectP-v3));
    if(glm::dot(surfaceNorm, C) < 0) {
        return -1.0; // P is on the right side
    };
    
    // P is within triangle
    return t;
}


float Triangle::getDistance(glm::vec3 pos) {
    return -1.0;
}


// Returns a nice red color
glm::vec3 Triangle::getColor(glm::vec3 pos) {
    return glm::vec3(0.9, 0.1, 0.1);
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