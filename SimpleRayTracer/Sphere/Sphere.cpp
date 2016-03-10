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
    this->diffCoeff = 0.5;
    this->specCoeff = 0.5;
}

Sphere::Sphere(glm::vec3 pos, float radius) {
    this->pos = pos;
    this->velocity = glm::vec3(0.0);
    this->radius = radius;
    this->color = glm::vec3(0.0, 1.0, 1.0);
    this->diffCoeff = .9454545;
    this->specCoeff = .545454;
}

Sphere::Sphere(glm::vec3 pos, float radius, glm::vec3 color) {
    this->pos = pos;
    this->velocity = glm::vec3(0.0);
    this->radius = radius;
    this->color = color;
    this->diffCoeff = .9454545;
    this->specCoeff = .545454;
}


glm::vec3 Sphere::getNormal(glm::vec3 pos) {
    return glm::normalize((pos - this->pos) / this->radius);
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


bool Sphere::doesCollideWith(Geometric *obj) {
    float dist = obj->getDistance(this->pos) - this->radius;
    if(dist < 0.0001) {
        return true;
    }
    return false;
}

void Sphere::mirrorCollisionHandling(Geometric *obj) {
    float dist = obj->getDistance(this->pos) - this->radius;
    glm::vec3 nor = obj->getNormal(this->pos);
    float normalCheck = glm::dot(this->velocity, nor);
    if(dist < 0.001 && normalCheck < 0) {
        // reverse velocity
        this->pos -= this->velocity * float(0.002);
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
