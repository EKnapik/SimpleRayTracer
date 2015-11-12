//
//  Mat3.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 11/11/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#ifndef Mat3_hpp
#define Mat3_hpp

#include <stdio.h>
#include "Vec3.hpp"

class Mat3 {
    Mat3(void);
    Mat3(Vec3 *col1, Vec3 *col2, Vec3 *col3);
    
    // probably not the best underlying structure
    Vec3 *col1, *col2, *col3;
};

#endif /* Mat3_hpp */
