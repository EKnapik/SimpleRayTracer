//
//  Ray.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/30/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#ifndef Ray_hpp
#define Ray_hpp

#include <stdio.h>
#include "glm/vec3.hpp"


class Ray {
public:
    // create function
    Ray(glm::vec3 pos, glm::vec3 dir);
    glm::vec3 pos;
    glm::vec3 dir;
private:
    
};

#endif /* Ray_hpp */
