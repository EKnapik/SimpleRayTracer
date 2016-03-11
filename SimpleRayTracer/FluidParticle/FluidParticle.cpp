//
//  FluidParticle.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 3/10/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//

#include "FluidParticle.hpp"

FluidParticle::FluidParticle(glm::vec3 pos) {
    static int _id = 0;
    this->id = _id++;
    this->pos = pos;
    this->radius = FLUID_RADIUS;
    this->color = glm::vec3(0.3, 0.82, 1.0); // aqua blue
    
    this->density = 0.0;
    this->pressure = 0.0;
    this->mass = FLUID_PARTICLE_MASS;
    this->restDensity = FLUID_REST_DENSITY;
}

FluidParticle::FluidParticle(glm::vec3 pos, float radius) {
    this->pos = pos;
    this->radius = radius;
    this->color = glm::vec3(0.3, 0.82, 1.0); // aqua blue
    
    this->density = 0.0;
    this->pressure = 0.0;
    this->mass = 1.0;
}

/*
 * This is applying the physics for this particle for this time delta
 * the current particle should not be in the array of particles given
 */
void FluidParticle::updateParticle(float timeStep, FluidParticle **fluidParticles, int numParticles) {
    glm::vec3 gravity = glm::vec3(0.0, -9.8, 0.0);
    glm::vec3 pressureTerm = gradPressureOverDensity(fluidParticles, numParticles);
    glm::vec3 velocityTerm = viscosityGradSquaredVelocity(fluidParticles, numParticles);
    
    // solve for the change in velocity at this time according to Navier-Stokes
    glm::vec3 dvdt = gravity - pressureTerm + velocityTerm;
    // Update with Semi-implicit Euler integration
    this->velocity += dvdt * timeStep;
    this->pos += this->velocity * timeStep;
    // doing fluid fluid collision detection here
    collisionDetection(fluidParticles, numParticles, timeStep);
}

/*
 * the current particle should not be in the array of particles given
 */
void FluidParticle::updateDensity(FluidParticle **fluidParticles, int numParticles) {
    float density = 0.0;
    for(int i = 0; i < numParticles; i++) {
        if(this->id != fluidParticles[i]->id) {
            density += fluidParticles[i]->mass * W(this->pos, fluidParticles[i]->pos, FLUID_H_VALUE);
        }
    }
    
    printf("Density: %.2f  ID: %d  RestD: %.2f\n", density, this->id, this->restDensity);
    this->density = density;
}


void FluidParticle::updatePressure() {
    this->pressure = FLUID_CONSTANT_K * (this->density - this->restDensity);
}

/* The gradient pressure of a particle divided by the density of that particle
 * This is an estimation method using smoothing kernals
 * the current particle should not be in the array of particles given
 */
glm::vec3 FluidParticle::gradPressureOverDensity(FluidParticle **fluidParticles, int numParticles) {
    glm::vec3 result = glm::vec3(0.0);
    float pressureScale;
    for(int i = 0; i < numParticles; i++) {
        pressureScale = (this->pressure / (this->density * this->density));
        pressureScale += (fluidParticles[i]->pressure / (fluidParticles[i]->density * fluidParticles[i]->density));
        result += fluidParticles[i]->mass * pressureScale * gradientW(this->pos, fluidParticles[i]->pos, FLUID_H_VALUE);
    }
    printf("Grad pressure over Density: (%.2f, %.2f, %.2f)\n", result.x, result.y, result.z);
    return result;
}

/* The viscosity times the squared gradient of velocity for a particle
 * This is an estimation method using smoothing kernals
 * the current particle should not be in the array of particles given
 */
glm::vec3 FluidParticle::viscosityGradSquaredVelocity(FluidParticle **fluidParticles, int numParticles) {
    glm::vec3 result = glm::vec3(0.0);
    float viscosityScale = FLUID_FRICTION_MU / this->density;
    glm::vec3 velocityVar;
    for(int i = 0; i < numParticles; i++) {
        velocityVar = (fluidParticles[i]->velocity - this->velocity) / fluidParticles[i]->density;
        result += fluidParticles[i]->mass * velocityVar * gradientSquaredW(this->pos, fluidParticles[i]->pos, FLUID_H_VALUE);
    }
    result *= viscosityScale;
    
    return result;
}


/* Since we know the particles that are close to this one we can do collision
 * collision detection with the nearby particles
 * The collision 'mirror' reflection was taken from a paper that uses the twice
 * the checking distance for collision, the distance to the collision, the normal
 * it is similar to a perfect mirror reflection but through testing looks better
 * for water
 */
void FluidParticle::collisionDetection(FluidParticle **fluidParticles, int numParticles, float timeStep) {
    for(int i = 0; i < numParticles; i++) {
        mirrorCollisionHandling(fluidParticles[i], timeStep);
    }
}



