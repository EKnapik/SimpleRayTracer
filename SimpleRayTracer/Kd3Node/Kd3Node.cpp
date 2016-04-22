//
//  Kd3Node.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 4/22/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//


/*
 TODO:
 THIS CAN BE OPTIMIZED!!!! the determining of the SAH iterates through the
 list of shapes smaller and greater several times
 */
#include "Kd3Node.hpp"


Kd3Node::Kd3Node(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax,
                 int numObjects, Geometric** objects, int curDepth) {
    this->xMin = xMin;
    this->xMax = xMax;
    this->yMin = yMin;
    this->yMax = yMax;
    this->zMin = zMin;
    this->zMax = zMax;
    // Check for max tree depth
    this->nodeDepth = curDepth;
    if(this->nodeDepth > MAX_NODE_DEPTH) {
        this->numObjects = numObjects;
        this->objects = objects;
        return;
    }
    // Choose the splitting plane type based on the depth
    switch (this->nodeDepth % 3) {
        case 0:
            this->pType = XY;
            break;
        case 1:
            this->pType = YZ;
            break;
        case 2:
            this->pType = XZ;
            break;
        default:
            std::cerr << "Error Splitting the Kd3Node Plane" << std::endl;
            exit(1);
            break;
    }
    // Find the best splitting plane position
    this->planePos = this->SAH(numObjects, objects);
    // Find Objects Smaller and Greater than split plane
    int numSmaller = countSmaller(this->planePos, this->pType, numObjects, objects);
    this->smaller = new Kd3Node(xMin, xMax, yMin, yMax, zMin, zMax,
                                numSmaller, getSmaller(this->planePos, this->pType, numObjects, objects), curDepth+1);
    this->greater = new Kd3Node(xMin, xMax, yMin, yMax, zMin, zMax,
                                numObjects - numSmaller, getGreater(this->planePos, this->pType, numObjects, objects), curDepth+1);
}


Kd3Node::~Kd3Node() {
    delete[] this->objects;
    delete this->smaller;
    delete this->greater;
}


Geometric* Kd3Node::traverse(Ray *ray) {
    glm::vec3 planeNorm;
    switch (this->pType) {
        case XY:
            planeNorm = glm::vec3(0.0, 0.0, 1.0);
            break;
        case YZ:
            planeNorm = glm::vec3(1.0, 0.0, 0.0);
            break;
        case XZ:
            planeNorm = glm::vec3(0.0, 1.0, 0.0);
            break;
        default:
            std::cerr << "Error Ray Traversing the Kd3Node" << std::endl;
            exit(1);
            break;
    }
    
    return NULL;
}



glm::vec3 Kd3Node::SAH(int numObjects, Geometric** objects) {
    float TEST_STEP = 0.1;
    glm::vec3 bestPos = glm::vec3(this->xMin, this->yMin, this->zMin);
    float bestSAH = -1;
    glm::vec3 currPos = bestPos;
    glm::vec3 planeNorm;
    switch (this->pType) {
        case XY:
            planeNorm = glm::vec3(0.0, 0.0, 1.0);
            break;
        case YZ:
            planeNorm = glm::vec3(1.0, 0.0, 0.0);
            break;
        case XZ:
            planeNorm = glm::vec3(0.0, 1.0, 0.0);
            break;
        default:
            std::cerr << "Error Ray Traversing the Kd3Node" << std::endl;
            exit(1);
            break;
    }
    
    int numSmaller;
    int numGreater;
    float testSAH;
    while(currPos.x < this->xMax && currPos.y < this->yMax && currPos.z < this->zMax) {
        numSmaller = countSmaller(currPos, this->pType, numObjects, objects);
        numGreater = numObjects - numSmaller;
        testSAH = (numSmaller*(currPos.x-this->xMin)*(currPos.y-this->yMin)*(currPos.z-this->zMin))
                + (numGreater*(this->xMax-currPos.x)*(this->yMax-currPos.y)*(this->zMax-currPos.z));
        if(bestSAH < 0 || testSAH <= bestSAH) {
            bestSAH = testSAH;
            bestPos = currPos;
        }
        currPos += planeNorm*TEST_STEP;
    }
    
    return bestPos;
}



int countSmaller(glm::vec3 splitPos, PlaneType pType, int numObjects, Geometric** objects) {
    int smaller = 0;
    for(int i = 0; i < numObjects; i++) {
        if(objects[i]->isLess(splitPos, pType)) {
            smaller++;
        }
    }
    return smaller;
}


int countGreater(glm::vec3 splitPos, PlaneType pType, int numObjects, Geometric** objects) {
    return numObjects - countSmaller(splitPos, pType, numObjects, objects);
}

Geometric** getSmaller(glm::vec3 splitPos, PlaneType pType, int numObjects, Geometric** objects) {
    Geometric** smaller;
    
    return smaller;
}


Geometric** getGreater(glm::vec3 splitPos, PlaneType pType, int numObjects, Geometric** objects) {
    Geometric** greater;
    
    return greater;
}