//
//  FluidParticle.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 3/10/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//

#ifndef FluidParticle_hpp
#define FluidParticle_hpp

#include "Sphere.hpp"
#include "FluidMath.hpp"

// SETUP FOR WATER
#define FLUID_NUM_PARTICLES 1
#define FLUID_PARTICLE_MASS 0.5
#define FLUID_CONSTANT_K 1.0
#define FLUID_FRICTION_MU 3.5
#define FLUID_H_VALUE 0.94
#define FLUID_RADIUS 0.0492653
#define FLUID_REST_DENSITY 998.29

class FluidParticle: public Sphere {
public:
    FluidParticle(glm::vec3 pos);
    FluidParticle(glm::vec3 pos, float radius);
    
    void updateParticle(float timeStep, FluidParticle **fluidParticles, int numParticles);
    void updateDensity(FluidParticle **fluidParticles, int numParticles);
    void updatePressure();
    glm::vec3 gradPressureOverDensity(FluidParticle **fluidParticles, int numParticles);
    glm::vec3 viscosityGradSquaredVelocity(FluidParticle **fluidParticles, int numParticles);
    void collisionDetection(FluidParticle **fluidParticles, int numParticles, float timeStep);
    
    // position covered by inheritance
    int id;
    float density;
    float pressure;
    float mass;
    float restDensity; // THIS IS BASED ON THE totalMass / totalVolume of fluid
};

#endif /* FluidParticle_hpp */
