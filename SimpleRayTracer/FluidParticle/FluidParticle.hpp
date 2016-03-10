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

class FluidParticle: public Sphere {
public:
    FluidParticle(glm::vec3 pos);
    
    void updateParticle(float timeDelta, FluidParticle *fluidParticles, int numParticles);
    void updateDensity(FluidParticle *fluidParticles, int numParticles);
    void updatePressure();
    
    // position covered by inheritance
    glm::vec3 velocity;
    float density;
    float pressure;
    float mass;
};

#endif /* FluidParticle_hpp */
