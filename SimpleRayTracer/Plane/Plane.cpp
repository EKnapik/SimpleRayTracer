//
//  Plane.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/19/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#include "Plane.hpp"

Plane::Plane() {
    this->pos = glm::vec3(0.0);
    this->normal = glm::vec3(0.0, 1.0, 0.0);
    this->color = glm::vec3(0.8);
}

Plane::Plane(glm::vec3 pos) {
    this->pos = pos;
    this->normal = glm::vec3(0.0, 1.0, 0.0);
    this->color = glm::vec3(0.8);
}

Plane::Plane(glm::vec3 pos, glm::vec3 normal) {
    this->pos = pos;
    this->normal = glm::normalize(normal);
    this->color = glm::vec3(0.8);
}

Plane::Plane(glm::vec3 pos, glm::vec3 normal, glm::vec3 color) {
    this->pos = pos;
    this->normal = glm::normalize(normal);
    this->color = color;
}

glm::vec3 Plane::getNormal(glm::vec3 pos) {
    return this->normal;
}

// if the denom is zero then the ray and plane are parallel
float Plane::getIntersect(Ray *ray) {
    return -ray->pos.y/ray->dir.y;
    /*
    float denom = glm::dot(ray->dir, this->normal);
    if(denom == 0) {
        return -1.0;
    }
    return (glm::dot((this->pos - ray->pos), this->normal) / denom);
     */
}

float Plane::getDistance(glm::vec3 pos) {
    glm::vec3 pq = glm::vec3(pos - this->pos);
    float scale = glm::dot(pq, this->normal) / glm::dot(this->normal, this->normal);
    return scale;
}

// returns checkard color grid
glm::vec3 Plane::getColor(glm::vec3 pos) {
    float tileSize = 2.0;
    int tile = floor(tileSize*pos.x) + floor(tileSize*pos.z);
    tile = tile % 2;
    if(tile > 0.0) {
        return glm::vec3(0.9, 0.1, 0.1);
    }
    return glm::vec3(0.9, 0.9, 0.1);
}

void Plane::setNormal(glm::vec3 normal) {
    this->normal = glm::normalize(normal);
}

void Plane::setPos(glm::vec3 pos) {
    this->pos = pos;
}

void Plane::setColor(glm::vec3 color) {
    this->color = color;
}