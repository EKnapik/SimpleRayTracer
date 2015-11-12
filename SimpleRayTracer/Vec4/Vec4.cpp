//
//  Vec4.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 11/1/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#include "Vec4.hpp"

/* !!!!!! PUBLIC FUNCTIONS !!!!!! */

Vec4::Vec4(float x) {
    this->x = x;
    this->y = x;
    this->z = x;
    this->w = x;
}

Vec4::Vec4(float x, float y, float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

// Uses X float value to return 0-255 integer for Red
int Vec4::getR() {
    return floatToInt(this->x);
}

// Uses Y float value to return 0-255 integer for Green
int Vec4::getG() {
    return floatToInt(this->y);
}

// Uses Z float value to return 0-255 integer for Blue
int Vec4::getB() {
    return floatToInt(this->z);
}

// Uses W float value to return 0-255 integer for Alpha
int Vec4::getA() {
    return floatToInt(this->w);
}



/* !!!!!! PRIVATE FUNCTIONS !!!!!! */

// converts float value 0.0-1.0 to 0-255 integer value
// fValue < 0.0 returns 0
// fValue > 1.0 is set to 255
int Vec4::floatToInt(float fValue) {
    if (fValue < 0.0) {
        return 0;
    } else if (fValue > 1.0) {
        return 255;
    }
    // use compiler conversion of float -> int
    // not the best should use floor
    return int(fValue * 255);
}




