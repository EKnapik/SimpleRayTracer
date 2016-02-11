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
    Ray(glm::vec3 pos, glm::vec3 dir);
    ~Ray();
    glm::vec3 pos;
    glm::vec3 dir;
    float curRefIndex = 1.0;
    bool inside = false;
private:
};

#endif /* Ray_hpp */
