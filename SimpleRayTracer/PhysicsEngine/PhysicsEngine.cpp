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
        if(this->scene->objects[i]->movable) {
            this->scene->objects[i]->velocity += 9.8 * timeDelta;
            this->scene->objects[i]->applyPhysics(timeDelta);
        }
    }
    
    // -----------FLUID STUFF-------
    // TODO  THERE SHOULD BE A PARTICLE ENGINEE....
    // Update the density and pressure of each particle
    for(int i = 0; i < this->scene->numParticles; i++) {
        this->scene->particles[i]->updateDensity(this->scene->particles, this->scene->numParticles);
        // printf("ID: %d, Density: %.2f\n", this->scene->particles[i]->id, this->scene->particles[i]->density);
    }
    for(int i = 0; i < this->scene->numParticles; i++) {
        this->scene->particles[i]->updatePressure();
        // printf("ID: %d, Pressure: %.2f\n", this->scene->particles[i]->id, this->scene->particles[i]->pressure);
    }
    // update the pressure over density term for each particle
    for(int i = 0; i < this->scene->numParticles; i++) {
        this->scene->particles[i]->updateGradPressureOverDensity(this->scene->particles, this->scene->numParticles);
    }
    // update the viscosity term for each particle
    for(int i = 0; i < this->scene->numParticles; i++) {
        this->scene->particles[i]->updateViscosityGradSquaredVelocity(this->scene->particles, this->scene->numParticles);
    }
    // Now move the particle
    for(int i = 0; i < this->scene->numParticles; i++) {
        this->scene->particles[i]->updateParticle(timeDelta, this->scene->particles, this->scene->numParticles);
    }
    // -----------FLUID STUFF  END-------
    
    
    // collision detect for all objects....
    for(int i = 0; i < this->scene->numObjects; i++) {
        for(int j = 0; j < this->scene->numObjects; j++) {
            if(i != j) {
                this->scene->objects[i]->mirrorCollisionHandling(this->scene->objects[j], DEFAULT_TIME_DELTA);
            }
        }
    }
    // collide particles with the scene objects
    for(int i = 0; i < this->scene->numParticles; i++) {
        for(int j = 0; j < this->scene->numObjects; j++) {
            if(i != j) {
                this->scene->particles[i]->mirrorCollisionHandling(this->scene->objects[j], DEFAULT_TIME_DELTA);
            }
        }
    }
}