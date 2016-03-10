//
//  Physical.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 3/10/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//



// WAIT FOR NOW ON THIS IMPLEMENTATION !!!!!!!!!!!!!!!!!!!!!!!!
#ifndef Physical_hpp
#define Physical_hpp

#include <stdio.h>
#include "glm/vec3.hpp"

class Physical {
public:
    
    glm::vec3 pos;
    glm::vec3 velocity;
    float density;
    float pressure;
    float mass;
};

#endif /* Physical_hpp */
