//
//  PhysicsEngine.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 3/10/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//

#include "PhysicsEngine.hpp"

PhysicsEngine::PhysicsEngine() {
    this->scene = new Scene();
}


PhysicsEngine::PhysicsEngine(Scene *scene) {
    this->scene = scene;
    // TODO do stuff.....
}


void PhysicsEngine::applyTimeStep() {
    //rayTracer->scene->shapes[1]->velocity.y -= 9.8 * TIMESTEP;
    //rayTracer->scene->shapes[1]->pos += rayTracer->scene->shapes[1]->velocity * float(0.02);
    //rayTracer->scene->shapes[1]->mirrorCollisionHandling(rayTracer->scene->shapes[0]);
    applyTimeStep(DEFAULT_TIME_DELTA);
}


void PhysicsEngine::applyTimeStep(float timeDelta) {
    // update all objects
    for(int i = 0; i < this->scene->numObjects; i++) {
        // each object should choose if physics applies to it......
        // TODO: Ask each object to move
        if(this->scene->shapes[i]->movable) {
            
        }
    }
    // collision detect for all objects....
    for(int i = 0; i < this->scene->numObjects; i++) {
        for(int j = 0; j < this->scene->numObjects; j++) {
            if(i != j) {
                this->scene->shapes[i]->mirrorCollisionHandling(this->scene->shapes[j], DEFAULT_TIME_DELTA);
            }
        }
    }
}