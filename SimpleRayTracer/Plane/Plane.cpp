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
    this->diffCoeff = .9454545;
    this->specCoeff = .545454;
    this->type = Plane_type;
}

Plane::Plane(glm::vec3 pos) {
    this->pos = pos;
    this->normal = glm::vec3(0.0, 1.0, 0.0);
    this->color = glm::vec3(0.8);
    this->diffCoeff = .9454545;
    this->specCoeff = .545454;
    this->type = Plane_type;
}

Plane::Plane(glm::vec3 pos, glm::vec3 normal) {
    this->pos = pos;
    this->normal = glm::normalize(normal);
    this->color = glm::vec3(0.8);
    this->diffCoeff = .9454545;
    this->specCoeff = .545454;
    this->type = Plane_type;
}

Plane::Plane(glm::vec3 pos, glm::vec3 normal, glm::vec3 color) {
    this->pos = pos;
    this->normal = glm::normalize(normal);
    this->color = color;
    this->diffCoeff = .9454545;
    this->specCoeff = .545454;
    this->type = Plane_type;
}

glm::vec3 Plane::getNormal(glm::vec3 pos) {
    return this->normal;
}

// if the denom is zero then the ray and plane are parallel
float Plane::getIntersect(Ray *ray) {
    float denom = glm::dot(ray->dir, this->normal);
    if(-0.00001 < denom && denom < 0.00001) {
        return -1.0;
    }
    float retT = glm::dot((this->pos - ray->pos), this->normal) / denom;
    glm::vec3 hitPos = ray->pos + (retT*ray->dir);
    if(hitPos.x < xLimit.x || hitPos.x > xLimit.y) {
        return -1.0;
    }
    if(hitPos.y < yLimit.x || hitPos.y > yLimit.y) {
        return -1.0;
    }
    if(hitPos.z < zLimit.x || hitPos.z > zLimit.y) {
        return -1.0;
    }
    
    return retT;
}

float Plane::getDistance(glm::vec3 pos) {
    glm::vec3 pq = glm::vec3(pos - this->pos);
    float scale = glm::dot(pq, this->normal) / glm::dot(this->normal, this->normal);
    return scale;
}

// returns checkard color grid
glm::vec3 Plane::getColor(glm::vec3 pos) {
    // The Floor function isn't working with negative values
    // NO idea why this is happening
    float tileSize = 2.0;
    float tempx = tileSize * (pos.x + 1000);
    float tempz = tileSize * (pos.z + 1000);
    int tile = floor(tempx) + floor(tempz);
    tile = tile % 2;
    if(tile > 0.0) {
        return glm::vec3(0.9, 0.1, 0.1);
    }
    return glm::vec3(0.9, 0.9, 0.1);
}


bool Plane::inBounds(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax) {
    if(this->pos.y >= yMin && this->pos.y <= yMax) {
        return true;
    }
    return false;
}


bool Plane::doesCollideWith(Geometric *obj) {
    float dist = obj->getDistance(this->pos);
    if(dist < 0.001) {
        return true;
    }
    return false;
}

void Plane::mirrorCollisionHandling(Geometric *obj, float timeStep) {
    float dist = obj->getDistance(this->pos);
    glm::vec3 nor = obj->getNormal(this->pos);
    float normalCheck = glm::dot(this->velocity, nor);
    if(dist < 0.001 && normalCheck < 0) {
        // reverse velocity
        this->pos += dist*glm::normalize(this->velocity);
        this->velocity = COLLISION_DAMPENING * glm::reflect(this->velocity, nor);
    }
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

void Plane::setxLimit(glm::vec2 newxLim) {
    this->xLimit = newxLim;
}

void Plane::setyLimit(glm::vec2 newyLim) {
    this->yLimit = newyLim;
}

void Plane::setzLimit(glm::vec2 newzLim) {
    this->zLimit = newzLim;
}

