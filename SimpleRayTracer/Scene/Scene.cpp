//
//  Scene.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/19/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#include "Scene.hpp"
#include <iostream>

Scene::Scene() {
    this->baseBackground = new Background();
    this->camera = new Camera(glm::vec3(1.0, 1.1, 2.0), glm::vec3(1.0, 1.1, -1.0));
    this->light = new Light();
    
    this->numObjects = 1;
    this->shapes = new Geometric *[numObjects];
    Plane *plane = new Plane();
    plane->setxLimit(glm::vec2(-20, 20));
    plane->setzLimit(glm::vec2(-20, 20));
    this->shapes[0] = plane;
    
    this->numParticles = 1;
    this->particles = new FluidParticle *[numParticles];
    float currPos = 0.0;
    
    for(int i = 0; i < numParticles; i++) {
        this->particles[i] = new FluidParticle(glm::vec3(currPos, 1.0, 0.0));
        currPos += (2*FLUID_RADIUS) + 0.01;
    }
    
    
    this->ambientCoeff = 0.1;
}

Scene::Scene(Camera *camera) {
    this->baseBackground = new Background();
    this->numObjects = 0;
    this->camera = camera;
    this->light = new Light();
    this->ambientCoeff = 0.1;
}

Scene::~Scene() {
    for(int i = 0; i < numObjects; i++) {
        delete shapes[i];
    }
    delete[] shapes;
}

// MARCHES FOR GEOMETRICS AND PARTICLES
Geometric* Scene::intersectMarch(Ray *ray) {
    float tmin = 0.0;
    float tmax = 60.0;
    float t = tmin;
    
    float const precis = 0.0001;
    int const steps = 100;
    Geometric *returnShape = this->baseBackground;
    if(numObjects == 0) {
        return returnShape;
    }
    
    for(int i = 0; i < steps; i++) {
        float dist = tmax;
        float distTemp;
        for(int j = 0; j < numObjects; j++) {
            distTemp = shapes[j]->getDistance(ray->pos + t*ray->dir);
            if(distTemp < dist) {
                dist = distTemp;
                returnShape = shapes[j];
            }
        }
        for(int j = 0; j < numParticles; j++) {
            distTemp = particles[j]->getDistance(ray->pos + t*ray->dir);
            if(distTemp < dist) {
                dist = distTemp;
                returnShape = particles[j];
            }
        }
        
        if(dist < precis || t > tmax) {
            break;
        }
        t += dist;
    }
    
    if( t>tmax ) {
        return this->baseBackground;
    }
    returnShape->timeHit = t;
    return returnShape;
}

// INTERSECTS THE GEOMETRICS AND THE PARTICLES
Geometric* Scene::intersectCast(Ray *ray) {
    float resT = 10000.0; // infinity kinda
    float tempT;
    Geometric *returnShape = this->baseBackground;
    
    for(int i = 0; i < this->numObjects; i++) {
        tempT = shapes[i]->getIntersect(ray);
        if(tempT > 0 && tempT < resT) {
            resT = tempT;
            returnShape = shapes[i];
            returnShape->timeHit = resT;
        }
    }
    
    for(int i = 0; i < this->numParticles; i++) {
        tempT = particles[i]->getIntersect(ray);
        if(tempT > 0 && tempT < resT) {
            resT = tempT;
            returnShape = particles[i];
            returnShape->timeHit = resT;
        }
    }
    
    return returnShape;
}


/*
 this->baseBackground = new Background();
 this->camera = new Camera(glm::vec3(1.0, 1.1, 2.0), glm::vec3(1.0, 1.1, -1.0));
 this->light = new Light();
 this->numObjects = 3;
 this->shapes = new Geometric *[numObjects];
 Plane *plane = new Plane();
 plane->setxLimit(glm::vec2(-5, 3));
 plane->setzLimit(glm::vec2(-10, 5));
 this->shapes[0] = plane;
 this->shapes[1] = new Sphere(glm::vec3(0.1, 1.0, -1.0), 0.65, glm::vec3(0.3));
 this->shapes[1]->reflective = true;
 this->shapes[2] = new Sphere(glm::vec3(1.2, 1.4, 0.2), 0.7, glm::vec3(0.1, 0.1, 0.8));
 this->shapes[2]->transmitive = true;
 this->shapes[2]->refractIndex = 1.517;
 
 this->ambientCoeff = 0.1;
*/

