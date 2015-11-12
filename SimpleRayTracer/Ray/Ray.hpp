//
//  Ray.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/30/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#ifndef Ray_hpp
#define Ray_hpp

#include <stdio.h>
#include "Vec4.hpp"

class Ray {
public:
    Ray(Vec4 *pos, Vec4 *dir);
    Vec4 *pos;
    Vec4 *dir;
private:
    
    // needs vector of initial position
    // vector of direction
    // color
};

#endif /* Ray_hpp */
