//
//  Vec4.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 11/1/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#include "Vec4.hpp"

/* !!!!!! PUBLIC FUNCTIONS !!!!!! */

Vec4::Vec4(double x) {
    size = 4;
    data = new double[4];
    data[0] = x;
    data[1] = x;
    data[2] = x;
    data[3] = x;
}

Vec4::Vec4(double x, double y, double z, double w) {
    size = 4;
    data = new double[4];
    data[0] = x;
    data[1] = y;
    data[2] = z;
    data[3] = w;
}


Vec4* Vec4::normalize(Vec4 *vec) {
    double length = len(vec);
    return *vec / length;
}

// Add
Vec4* Vec4::operator+(const Vec4 *vector) {
    Vec4 *res = new Vec4(0.0);
    for(int i = 0; i < size; i++) {
        res->data[i] = this->data[i] + vector->data[i];
    }
    return res;
}

// Subtract
Vec4* Vec4::operator-(const Vec4 *vector) {
    Vec4 *res = new Vec4(0.0);
    for(int i = 0; i < size; i++) {
        res->data[i] = this->data[i] - vector->data[i];
    }
    return res;
}


// Scale
Vec4* Vec4::operator*(const double scale) {
    Vec4 *res = new Vec4(0.0);
    for(int i = 0; i < size; i++) {
        res->data[i] = this->data[i] * scale;
    }
    return res;
}

// vector / scale
Vec4* Vec4::operator/(const double scale) {
    Vec4 *res = new Vec4(0.0);
    for(int i = 0; i < size; i++) {
        res->data[i] = this->data[i] / scale;
    }
    return res;
}