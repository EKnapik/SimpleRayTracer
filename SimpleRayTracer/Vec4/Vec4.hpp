//
//  Vec4.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 11/1/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#ifndef Vec4_hpp
#define Vec4_hpp

#include <stdio.h>


// rgba stored respectively xyza
class Vec4 {
public:
    Vec4(float x);
    Vec4(float x, float y, float z, float w);
    int getR();
    int getG();
    int getB();
    int getA();
    float x, y, z, w;
private:
    int floatToInt(float fValue);
    
};

// Vector4 can also be used as a vector 2 and a vector 3
// Need to write the operation overloaders for this class

#endif /* Vec4_hpp */


// asdf