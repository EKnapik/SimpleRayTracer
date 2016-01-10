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
#include "VectorBase.hpp"


// rgba stored respectively xyza
class Vec4: public VectorBase {
public:
    Vec4(double x);
    Vec4(double x, double y, double z, double w);
    Vec4* normalize(Vec4 *vec);
    
    Vec4* operator+(const Vec4 *vector); // Add
    Vec4* operator-(const Vec4 *vector); // Subtract
    Vec4* operator*(const double scale); // Scale
    Vec4* operator/(const double scale); // scale

};

// Vector4 can also be used as a vector 2 and a vector 3
// Need to write the operation overloaders for this class

#endif /* Vec4_hpp */


// asdf