//
//  Circle.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/19/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#include "Circle.hpp"

Circle::Circle(glm::vec3 pos, float radius) {
    
}


glm::vec3 Circle::getNormal(glm::vec3 pos) {
    
    return glm::vec3(0.0);
};


glm::vec3 Circle::getIntersect(glm::vec3 rayOr, glm::vec3 rayDir) {
    
    return glm::vec3(0.0);
}


glm::vec3 Circle::getDistance(glm::vec3 pos) {
    
    return glm::vec3(0.0);
}


glm::vec3 Circle::getColor(glm::vec3 pos) {
    
    return glm::vec3(0.0);
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