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
    
    glm::vec3 pos;
    glm::vec3 velocity;
    float density;
    float pressure;
    float mass;
};

#endif /* FluidParticle_hpp */
