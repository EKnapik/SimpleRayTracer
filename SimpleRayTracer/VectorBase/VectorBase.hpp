//
//  VectorBase.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 12/12/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#ifndef VectorBase_hpp
#define VectorBase_hpp

#include <stdio.h>


class VectorBase {
public:
    double X();
    double Y();
    double Z();
    double W();
    // operators
private:
    int size;
    double *data;    
};

#endif /* VectorBase_hpp */
