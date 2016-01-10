//
//  Vec3.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 11/11/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#include "Vec3.hpp"

Vec3::Vec3(double x) {
    size = 3;
    data = new double[3];
    data[0] = x;
    data[1] = x;
    data[2] = x;
}

Vec3::Vec3(double x, double y, double z) {
    size = 3;
    data = new double[3];
    data[0] = x;
    data[1] = y;
    data[2] = z;
}

Vec3* Vec3::normalize(Vec3 *vec) {
    double length = len(vec);
    return *vec / length;
}

// Add
Vec3* Vec3::operator+(const Vec3 *vector) {
    Vec3 *res = new Vec3(0.0);
    for(int i = 0; i < size; i++) {
        res->data[i] = this->data[i] + vector->data[i];
    }
    return res;
}

// Subtract
Vec3* Vec3::operator-(const Vec3 *vector) {
    Vec3 *res = new Vec3(0.0);
    for(int i = 0; i < size; i++) {
        res->data[i] = this->data[i] - vector->data[i];
    }
    return res;
}

// Cross
Vec3* Vec3::operator*(const Vec3 *vector) {
    Vec3 *res = new Vec3(0.0);
    res->data[0] = (this->data[1] * vector->data[2]) - (this->data[2] * vector->data[1]);
    res->data[1] = -((this->data[0] * vector->data[2]) - (this->data[2] * vector->data[0]));
    res->data[2] = (this->data[0] * vector->data[1]) - (this->data[1] * vector->data[0]);
    return res;
}

// Scale
Vec3* Vec3::operator*(const double scale) {
    Vec3 *res = new Vec3(0.0);
    for(int i = 0; i < size; i++) {
        res->data[i] = this->data[i] * scale;
    }
    return res;
}

// vector / scale
Vec3* Vec3::operator/(const double scale) {
    Vec3 *res = new Vec3(0.0);
    for(int i = 0; i < size; i++) {
        res->data[i] = this->data[i] / scale;
    }
    return res;
}