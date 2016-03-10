//
//  Mesh.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 3/9/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <stdio.h>
#include "Triangle.hpp"

class Mesh {
    Mesh();
    Mesh(Triangle *meshTriangles, int numTriangles);
    void addTriangle(Triangle *triangle);
    
    Triangle *triangles;
    int numTriangles;
};

#endif /* Mesh_hpp */
