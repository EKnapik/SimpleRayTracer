//
//  Scene.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/19/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#include "Scene.hpp"

Scene::Scene() {
    this->baseBackground = new Background();
}

Scene::Scene(Camera *camera) {
    this->baseBackground = new Background();
    this->camera = camera;
}

Geometric* Scene::intersectMarch(glm::vec3 rayOr, glm::vec3 rayDir) {
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
            distTemp = shapes[j]->getDistance(rayOr);
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
    
    if( t>tmax ) {
        return this->baseBackground;
    }
    returnShape->timeHit = t;
    return returnShape;
}


Geometric* Scene::intersectCast(glm::vec3 rayOr, glm::vec3 rayDir) {
    float resT = 10000.0; // infinity kinda
    float tempT;
    Geometric *returnShape = this->baseBackground;
    
    for(int i = 0; i < this->numObjects; i++) {
        tempT = shapes[i]->getIntersect(rayOr, rayDir);
        if(tempT < resT) {
            returnShape = shapes[i];
            returnShape->timeHit = tempT;
        }
    }
    
    return returnShape;
}