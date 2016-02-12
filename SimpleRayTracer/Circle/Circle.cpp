//
//  Circle.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/19/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#include "Circle.hpp"

Circle::Circle(glm::vec3 pos, float radius) {
    this->pos = pos;
    this->radius = radius;
    this->color = glm::vec3(0.0, 1.0, 1.0);
}

Circle::Circle(glm::vec3 pos, float radius, glm::vec3 color) {
    this->pos = pos;
    this->radius = radius;
    this->color = color;
}


glm::vec3 Circle::getNormal(glm::vec3 pos) {
    return glm::normalize((pos - this->pos) / this->radius);
};


float Circle::getIntersect(Ray *ray) {
    glm::vec3 oc = ray->pos - this->pos;
    float b = glm::dot(oc, ray->dir);
    float c = glm::dot(oc, oc) - (this->radius*this->radius);
    float h = b*b - c;
    if( h < 0.0 ) return -1.0;
    float t = -b - sqrt(h);
    return t;
}


float Circle::getDistance(glm::vec3 pos) {
    return (glm::length(pos - this->pos) - this->radius);
}


glm::vec3 Circle::getColor(glm::vec3 pos) {
    return this->color;
}


void Circle::setRadius(float radius) {
    this->radius = radius;
}


void Circle::setPos(glm::vec3 pos) {
    this->pos = pos;
}


void Circle::setColor(glm::vec3 color) {
    this->color = color;
}