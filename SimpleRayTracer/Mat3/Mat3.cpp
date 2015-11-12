//
//  Mat3.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 11/11/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#include "Mat3.hpp"

Mat3::Mat3() {
    this->col1 = new Vec3(1.0, 0.0, 0.0);
    this->col2 = new Vec3(0.0, 1.0, 0.0);
    this->col3 = new Vec3(0.0, 0.0, 1.0);
}

Mat3::Mat3(Vec3 *col1, Vec3 *col2, Vec3 *col3) {
    this->col1 = col1;
    this->col2 = col2;
    this->col3 = col3;
}