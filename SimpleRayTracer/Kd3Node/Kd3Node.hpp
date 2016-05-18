//
//  Kd3Node.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 4/22/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//

#ifndef Kd3Node_hpp
#define Kd3Node_hpp

#include <stdio.h>
#include <iostream>
#include <stack>
#include "Ray.hpp"
#include "Geometric.hpp"
#include "Background.hpp"

#define MAX_NODE_DEPTH 3
#define TEST_STEP float(0.0005)


/*
 TODO: Check and change the triangle bounding interscection
 All the points could be outside the bounds but the triangle traverses within
 the bounds of the box.
 */


class Kd3Node {
public:
    Kd3Node(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax,
            int numObjects, Geometric** objects, int curDepth);
    ~Kd3Node();
    
    Geometric* traverse(Ray *ray,  glm::vec3 tmpPos);
    void PRINT();
    
    int numObjects = 0;
    Geometric** objects = NULL;

private:
    // solves for the best axis alligned partition using the surface
    // area heuristic
    glm::vec3 SAH(int numObjects, Geometric** objects);
    Geometric* intersectCast(Ray *ray);
    
    PlaneType pType;
    float xMin, xMax;
    float yMin, yMax;
    float zMin, zMax;
    glm::vec3 planePos;
    
    Kd3Node* smaller = NULL;
    Kd3Node* greater = NULL;
    int nodeDepth;
};

int countSmaller(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax, int numObjects, Geometric** objects);

int countGreater(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax, int numObjects, Geometric** objects);

Geometric** getSmaller(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax, int numObjects, Geometric** objects);

Geometric** getGreater(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax, int numObjects, Geometric** objects);


#endif /* Kd3Node_hpp */
