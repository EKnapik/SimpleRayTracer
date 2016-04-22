//
//  Background.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 2/10/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//

#include "Background.hpp"
Background::Background() {
    // black
    // this->color = glm::vec3(0.0);
    
    // cornflour blue
    this->color = glm::vec3(0.39, 0.61, 0.94);
}

glm::vec3 Background::getNormal(glm::vec3 pos) {
    return glm::vec3(0.0);
}

float Background::getIntersect(Ray *ray) {
    return -1.0;
}

float Background::getDistance(glm::vec3 pos) {
    return 1000000;
}

glm::vec3 Background::getColor(glm::vec3 pos) {
    return this->color;
}

bool Background::isLess(glm::vec3 pos, PlaneType pType) {
    return false;
}

bool Background::isGreater(glm::vec3 pos, PlaneType pType) {
    return true;
}

void Background::setColor(glm::vec3 color) {
    this->color = color;
}

bool Background::doesCollideWith(Geometric *obj) {
    return false;
}

void Background::mirrorCollisionHandling(Geometric *obj, float timeStep) {
    // NO-OP
}