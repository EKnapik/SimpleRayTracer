//
//  Ray.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/30/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#include "Ray.hpp"

Ray::Ray(glm::vec3 pos, glm::vec3 dir) {
    this->pos = pos;
    this->dir = dir;
}