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
#include <math.h>


class VectorBase {
public:
    double X();
    double Y();
    double Z();
    double W();
    void setX(double x);
    void setY(double y);
    void setZ(double z);
    void setW(double w);
    void print();
    // operators
    double dot(VectorBase *vec1, VectorBase *vec2);
    double len(VectorBase *vec);
    double angle(VectorBase *vec1, VectorBase *vec2);
protected:
    int size;
    double *data;
};

#endif /* VectorBase_hpp */
