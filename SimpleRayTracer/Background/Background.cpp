//
//  Background.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 2/10/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//

#include "Background.hpp"
Background::Background() {
    this->color = glm::vec3(0.0);
}

glm::vec3 Background::getNormal(glm::vec3 pos) {
    return glm::vec3(0.0);
}

float Background::getIntersect(glm::vec3 rayOr, glm::vec3 rayDir) {
    return -1.0;
}

float Background::getDistance(glm::vec3 pos) {
    return 1000000;
}

glm::vec3 Background::getColor(glm::vec3 pos) {
    return this->color;
}

void Background::setColor(glm::vec3 color) {
    this->color = color;
}