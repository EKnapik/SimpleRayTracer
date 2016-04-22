//
//  Sphere.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/19/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#include "Sphere.hpp"
#include <iostream>

Sphere::Sphere() {
    this->pos = glm::vec3(0.0);
    this->velocity = glm::vec3(0.0);
    this->radius = 0.0;
    this->color = glm::vec3(0.0);
    this->diffCoeff = 0.8;
    this->specCoeff = 0.3;
    this->collisionRadius = 0.0;
    this->type = Sphere_type;
}

Sphere::Sphere(glm::vec3 pos, float radius) {
    this->pos = pos;
    this->velocity = glm::vec3(0.0);
    this->radius = radius;
    this->color = glm::vec3(0.0, 1.0, 1.0);
    this->diffCoeff = .9454545;
    this->specCoeff = .545454;
    this->collisionRadius = radius;
    this->type = Sphere_type;
}

Sphere::Sphere(glm::vec3 pos, float radius, glm::vec3 color) {
    this->pos = pos;
    this->velocity = glm::vec3(0.0);
    this->radius = radius;
    this->color = color;
    this->diffCoeff = .9454545;
    this->specCoeff = .545454;
    this->collisionRadius = radius;
    this->type = Sphere_type;
}


glm::vec3 Sphere::getNormal(glm::vec3 pos) {
    return (pos - this->pos) / this->radius;
};


float Sphere::getIntersect(Ray *ray) {
    glm::vec3 oc = ray->pos - this->pos;
    float b = glm::dot(oc, ray->dir);
    float c = glm::dot(oc, oc) - (this->radius*this->radius);
    float h = b*b - c;
    if( h < 0.0 ) return -1.0;
    float t = -b - sqrt(h);
    if(ray->inside) {
        return -b + sqrt(h);
    }
    return t;
}


float Sphere::getDistance(glm::vec3 pos) {
    return (glm::length(pos - this->pos) - this->radius);
}


glm::vec3 Sphere::getColor(glm::vec3 pos) {
    return this->color;
}

bool Sphere::isLess(glm::vec3 pos, PlaneType pType) {
    switch (pType) {
        case XY:
            // planeNorm = glm::vec3(0.0, 0.0, 1.0);
            if((this->pos.z - radius) >= pos.z) {
                return false;
            }
            break;
        case YZ:
            // planeNorm = glm::vec3(1.0, 0.0, 0.0);
            if((this->pos.x - radius) >= pos.x) {
                return false;
            }
            break;
        case XZ:
            // planeNorm = glm::vec3(0.0, 1.0, 0.0);
            if((this->pos.y - radius) >= pos.y) {
                return false;
            }
            break;
        default:
            std::cerr << "Error Ray Traversing the Kd3Node" << std::endl;
            exit(1);
            break;
    }
    
    return true;
}


bool Sphere::doesCollideWith(Geometric *obj) {
    float dist = obj->getDistance(this->pos) - this->radius;
    if(dist < 0.001) {
        return true;
    }
    return false;
}


// TODDO:
// MOVE THE OBJECT OUTSIDE OF THE OBJECT THEN ADJUST VELOCITY
// DON'T STEP BACK A TIMESTEP
void Sphere::mirrorCollisionHandling(Geometric *obj, float timeStep) {
    float dist = obj->getDistance(this->pos) - this->radius;
    glm::vec3 nor = obj->getNormal(this->pos);
    //float normalCheck = glm::dot(this->velocity, nor);
    if(dist < 0.00001) {
        
        // move outside of the object and reverse velocity
        // TODO MOVE THE OBJECT TO THE EDGE. PLACE IT THERE DON'T ADD TO IT
        switch(obj->type) {
            // Sphere - Plane collision
            case Plane_type: this->pos.y = obj->pos.y + this->radius;
                break;
            // Sphere - Sphere collision
            case Sphere_type: this->pos += glm::normalize(this->velocity) * dist;
                break;
            // Sphere - Triangle collision
            case Triangle_type:
                break;
        }
        this->velocity = COLLISION_DAMPENING * glm::reflect(this->velocity, nor);
    }
}


void Sphere::setRadius(float radius) {
    this->radius = radius;
}


void Sphere::setPos(glm::vec3 pos) {
    this->pos = pos;
}


void Sphere::setColor(glm::vec3 color) {
    this->color = color;
}
