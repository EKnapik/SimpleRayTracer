//
//  PhysicsEngine.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 3/10/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//

#include "PhysicsEngine.hpp"


PhysicsEngine::PhysicsEngine(Scene *scene) {
    this->scene = scene;
    // TODO do stuff.....
}


void PhysicsEngine::applyTimeStep() {
    applyTimeStep(DEFAULT_TIME_DELTA);
}


void PhysicsEngine::applyTimeStep(float timeDelta) {
    // update all objects
    for(int i = 0; i < this->scene->numObjects; i++) {
        // each object should choose if physics applies to it......
        // TODO: Ask each object to move
        if(this->scene->shapes[i]->movable) {
            this->scene->shapes[i]->velocity += 9.8 * timeDelta;
            this->scene->shapes[i]->applyPhysics(timeDelta);
        }
    }
    for(int i = 0; i < this->scene->numParticles; i++) {
        this->scene->particles[i]->updateParticle(timeDelta, this->scene->particles, this->scene->numParticles);
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