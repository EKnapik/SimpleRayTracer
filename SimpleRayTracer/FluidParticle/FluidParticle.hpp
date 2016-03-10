//
//  FluidParticle.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 3/10/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//

#ifndef FluidParticle_hpp
#define FluidParticle_hpp

#include <stdio.h>
#include "Sphere.hpp"
#include "FluidMath.hpp"

#define FLUID_PARTICLE_MASS 1.0
#define FLUID_CONSTANT_K 9999999
#define FLUID_RESTING_DENSITY 1.0
#define FLUID_FRICTION_MU 1
#define FLUID_H_VALUE 2

class FluidParticle: protected Sphere {
public:
    FluidParticle(glm::vec3 pos, float radius);
    
    void updateParticle(float timeDelta, FluidParticle **fluidParticles, int numParticles);
    void updateDensity(FluidParticle **fluidParticles, int numParticles);
    void updatePressure();
    glm::vec3 gradPressureOverDensity(FluidParticle **fluidParticles, int numParticles);
    glm::vec3 viscosityGradSquaredVelocity(FluidParticle **fluidParticles, int numParticles);
    
    // position covered by inheritance
    glm::vec3 velocity;
    float density;
    float pressure;
    float mass;
};

#endif /* FluidParticle_hpp */
