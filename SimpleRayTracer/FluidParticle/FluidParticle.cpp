//
//  FluidParticle.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 3/10/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//

#include "FluidParticle.hpp"

FluidParticle::FluidParticle(glm::vec3 pos, float radius) {
    this->pos = pos;
    this->radius = radius;
    this->color = glm::vec3(0.38, 0.82, 0.96); // aqua blue
}

/*
 * This is applying the physics for this particle for this time delta
 * the current particle should not be in the array of particles given
 */
void FluidParticle::updateParticle(float timeDelta, FluidParticle **fluidParticles, int numParticles) {
    updateDensity(fluidParticles, numParticles);
    updatePressure();
    glm::vec3 gravity = glm::vec3(0.0, -9.8, 0.0);
    glm::vec3 pressureTerm = gradPressureOverDensity(fluidParticles, numParticles);
    glm::vec3 velocityTerm = viscosityGradSquaredVelocity(fluidParticles, numParticles);
    
    // solve for the change in velocity at this time according to Navier-Stokes
    glm::vec3 dvdt = gravity - pressureTerm + velocityTerm;
    // Update with Semi-implicit Euler integration
    this->velocity = dvdt * timeDelta;
    this->pos = this->velocity * timeDelta;
    // doing fluid fluid collision detection here
    collisionDetection(fluidParticles, numParticles);
}

/*
 * the current particle should not be in the array of particles given
 */
void FluidParticle::updateDensity(FluidParticle **fluidParticles, int numParticles) {
    float density = 0.0;
    for(int i = 0; i < numParticles; i++) {
        density += fluidParticles[i]->mass * W(this->pos, fluidParticles[i]->pos, FLUID_H_VALUE);
    }
    
    this->density = density;
}


void FluidParticle::updatePressure() {
    this->pressure = FLUID_CONSTANT_K * (this->density - FLUID_RESTING_DENSITY);
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
void FluidParticle::collisionDetection(FluidParticle **fluidParticles, int numParticles) {
    float distToParticle;
    float checkValue;
    for(int i = 0; i < numParticles; i++) {
        distToParticle = fluidParticles[i]->getDistance(this->pos) - this->radius;
        if(distToParticle <= 0.01) { // did we collide with the particle?
            checkValue = glm::dot(this->velocity,fluidParticles[i]->getNormal(this->pos));
            if(checkValue < 0) { // collision will happen
                // fix velocity by reversing it by 'mirror' reflection
                this->velocity += COLLISION_DAMPENING * float(0.02-distToParticle) * fluidParticles[i]->getNormal(this->pos);
            }
        }
    }
}



