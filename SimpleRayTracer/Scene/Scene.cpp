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
    this->numObjects = 3;
    this->shapes = new Geometric *[numObjects];
    this->shapes[0] = new Plane();
    this->shapes[1] = new Circle(glm::vec3(0.1, 1.0, -1.0), 0.65, glm::vec3(0.9));
    this->shapes[2] = new Circle(glm::vec3(1.2, 1.4, 0.2), 0.7, glm::vec3(0.2, 0.1, 0.8));
}

Scene::Scene(Camera *camera) {
    this->baseBackground = new Background();
    this->numObjects = 0;
    this->camera = camera;
}

Scene::~Scene() {
    for(int i = 0; i < numObjects; i++) {
        delete shapes[i];
    }
    delete[] shapes;
}

Geometric* Scene::intersectMarch(Ray *ray) {
    float tmin = 0.0;
    float tmax = 60.0;
    float t = tmin;
    
    float const precis = 0.00002;
    int const steps = 100;
    Geometric *returnShape = this->baseBackground;
    
    
    for(int i = 0; i < steps; i++) {
        float dist = tmax;
        float distTemp;
        for(int j = 0; j < numObjects; j++) {
            distTemp = shapes[j]->getDistance(ray->pos);
            if(distTemp < dist) {
                dist = distTemp;
                returnShape = shapes[j];
            }
        }
        
        if(dist < precis || t > tmax) {
            break;
        }
        t += dist;
    }
    
    if( t>tmax || numObjects == 0) {
        return this->baseBackground;
    }
    returnShape->timeHit = t;
    return returnShape;
}


Geometric* Scene::intersectCast(Ray *ray) {
    float resT = 10000.0; // infinity kinda
    float tempT;
    Geometric *returnShape = this->baseBackground;
    
    for(int i = 0; i < this->numObjects; i++) {
        tempT = shapes[i]->getIntersect(ray);
        if(tempT < resT && tempT > 0) {
            resT = tempT;
            returnShape = shapes[i];
            returnShape->timeHit = resT;
        }
    }
    
    return returnShape;
}