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
    virtual double X() = 0;
    virtual double Y() = 0;
    virtual double Z() = 0;
    virtual double W() = 0;
    // operators
protected:
    int size;
    double *data;
};

#endif /* VectorBase_hpp */
