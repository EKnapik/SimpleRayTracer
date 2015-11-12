//
//  Ray.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/30/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#include "Ray.hpp"


Ray::Ray(Vec4 *pos, Vec4 *dir) {
    this->pos = pos;
    this->dir = dir;
}

