//
//  Vec3.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 11/11/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#ifndef Vec3_hpp
#define Vec3_hpp

#include <stdio.h>
#include "VectorBase.hpp"

class Vec3: public VectorBase{
public:
    Vec3(double x);
    Vec3(double x, double y, double z);
    Vec3* normalize(Vec3 *vec);
    
    Vec3* operator+(const Vec3 *vector); // Add
    Vec3* operator-(const Vec3 *vector); // Subtract
    Vec3* operator*(const Vec3 *vector); // Cross
    Vec3* operator*(const double scale); // Scale
    Vec3* operator/(const double scale); // scale
};
#endif /* Vec3_hpp */
