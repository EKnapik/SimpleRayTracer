//
//  Sphere.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/19/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#include "Sphere.hpp"
#include <iostream>

Sphere::Sphere(glm::vec3 pos, float radius) {
    this->pos = pos;
    this->radius = radius;
    this->color = glm::vec3(0.0, 1.0, 1.0);
    this->diffCoeff = .9454545;
    this->specCoeff = .545454;
}

Sphere::Sphere(glm::vec3 pos, float radius, glm::vec3 color) {
    this->pos = pos;
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


void Sphere::setRadius(float radius) {
    this->radius = radius;
}


void Sphere::setPos(glm::vec3 pos) {
    this->pos = pos;
}


void Sphere::setColor(glm::vec3 color) {
    this->color = color;
}