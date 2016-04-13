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
    
    // move, scale and rotate mesh
    void moveBy(glm::vec3 transVector);
    void moveTo(glm::vec3 newPos);
    void scale(float scaleFactor);
    void rotateX(float degrees);
    void rotateY(float degrees);
    void rotateZ(float degrees);
    
    glm::vec3 pos;
    Triangle *triangles;
    int numTriangles;
};

#endif /* Mesh_hpp */
