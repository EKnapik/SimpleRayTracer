//
//  Vec3.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 11/11/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#include "Vec3.hpp"

Vec3::Vec3(float x) {
    this->x = x;
    this->y = x;
    this->z = x;
}

Vec3::Vec3(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}