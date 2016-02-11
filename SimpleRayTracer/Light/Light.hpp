//
//  Light.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 2/10/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//

#ifndef Light_hpp
#define Light_hpp

#include <stdio.h>
#include "glm/vec3.hpp"

class Light {
public:
    Light(glm::vec3 pos, glm::vec3 color);
    
    glm::vec3 pos;
    glm::vec3 color;
};

#endif /* Light_hpp */
