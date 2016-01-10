//
//  VectorBase.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 12/12/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#include "VectorBase.hpp"
double VectorBase::X() {
    return data[0];
}

double VectorBase::Y() {
    return data[1];
}


double VectorBase::Z() {
    if(size > 2) {
        return data[2];
    }
    return 0;
}


double VectorBase::W() {
    if(size > 3) {
        return data[3];
    }
    return 0;
}

void VectorBase::setX(double x) {
    this->data[0] = x;
}

void VectorBase::setY(double y) {
    this->data[1] = y;
}

void VectorBase::setZ(double z) {
    if(size > 2) {
        this->data[2] = z;
    }
}

void VectorBase::setW(double w) {
    if(size > 3) {
        this->data[3] = w;
    }
}

// prints to standard out
void VectorBase::print() {
    for(int i = 0; i < size; i++) {
        printf("%.2g ", data[i]);
    }
    printf("\n");
}
