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
    // fix the plane position because the SAH does not consider the 'infinate' edge
    glm::vec3 minBounds;
    glm::vec3 maxBounds;
    switch (this->pType) {
        case XY:
            minBounds = glm::vec3(xMin, yMin, this->planePos.z);
            maxBounds = glm::vec3(xMax, yMax, this->planePos.z);
            break;
        case YZ:
            minBounds = glm::vec3(this->planePos.x, yMin, zMin);
            maxBounds = glm::vec3(this->planePos.x, yMax, zMax);
            break;
        case XZ:
            minBounds = glm::vec3(xMin, this->planePos.y, zMin);
            maxBounds = glm::vec3(xMax, this->planePos.y, zMax);
            break;
    }
    // Find Objects Smaller and Greater than split plane
    int numSmaller = countSmaller(this->planePos, this->pType, numObjects, objects);
    int numGreater = countGreater(this->planePos, this->pType, numObjects, objects);
    if(numSmaller > 0) {
        this->smaller = new Kd3Node(xMin, maxBounds.x, yMin, maxBounds.y, zMin, maxBounds.z,
                                numSmaller, getSmaller(this->planePos, this->pType, numObjects, objects), curDepth+1);
    }
    if(numGreater > 0) {
        this->greater = new Kd3Node(minBounds.x, xMax, minBounds.y, yMax, minBounds.z, zMax,
                                numGreater, getGreater(this->planePos, this->pType, numObjects, objects), curDepth+1);
    }
}


Kd3Node::~Kd3Node() {
    if(this->objects != NULL) {
        delete this->objects;
    }
    if(this->smaller != NULL) {
        delete this->smaller;
    }
    if(this->greater != NULL) {
        delete this->greater;
    }
}


Geometric* Kd3Node::traverse(Ray *ray) {
    if(this->smaller == NULL && this->greater == NULL) {
        return intersectCast(ray);
    }
    
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
    
    float denom = glm::dot(ray->dir, planeNorm);
    if(-0.00001 < denom && denom < 0.00001) {
        // is the ray greater or less than the plane
        switch (this->pType) {
            case XY:
                if(ray->pos.z < this->planePos.z) { // ray is within smaller
                    if(this->smaller != NULL) {
                        return this->smaller->traverse(ray);
                    }
                } else {
                    if(this->greater != NULL) {
                        return this->greater->traverse(ray);
                    }
                }
                return NULL;
                break;
            case YZ:
                if(ray->pos.x < this->planePos.x) { // ray is within smaller
                    if(this->smaller == NULL) {
                        return NULL;
                    }
                    return this->smaller->traverse(ray);
                }  else {
                    if(this->greater == NULL) {
                        return NULL;
                    }
                    return this->greater->traverse(ray);
                }
                break;
            case XZ:
                if(ray->pos.y < this->planePos.y) { // ray is within smaller
                    if(this->smaller == NULL) {
                        return NULL;
                    }
                    return this->smaller->traverse(ray);
                }  else {
                    if(this->greater == NULL) {
                        return NULL;
                    }
                    return this->greater->traverse(ray);
                }
                break;
        }
    }
    
    Geometric* retObj = NULL;
    float retT = glm::dot((this->planePos - ray->pos), planeNorm) / denom;
    if(retT < 0) {
        if(this->smaller != NULL) {
            retObj = this->smaller->traverse(ray);
        }
        if(retObj == NULL && this->greater != NULL) {
            return this->greater->traverse(ray);
        }
    } else {
        if(this->greater != NULL) {
            retObj = this->greater->traverse(ray);
        }
        if(retObj == NULL && this->smaller != NULL) {
            return this->smaller->traverse(ray);
        }
    }
    
    return retObj;
}


void Kd3Node::PRINT() {
    printf("Split Plane at Depth %d: (%.2f, %.2f, %.2f) with %d Objects\n", this->nodeDepth,
           this->planePos.x, this->planePos.y, this->planePos.z, this->numObjects);
    
    if(this->smaller == NULL) {
        printf("Smaller(%d): NULL\n", this->nodeDepth);
    } else {
        printf("   ");
        this->smaller->PRINT();
    }
    if(this->greater == NULL) {
        printf("Greater(%d): NULL\n", this->nodeDepth);
    } else {
        printf("   ");
        this->greater->PRINT();
    }
    
}



glm::vec3 Kd3Node::SAH(int numObjects, Geometric** objects) {
    float TEST_STEP = 0.1;
    glm::vec3 bestPos = glm::vec3(xMin, yMin, zMin);
    float bestSAH = -1;
    glm::vec3 currPos = bestPos;
    
    int numSmaller;
    int numGreater;
    float testSAH;
    while(currPos.x < this->xMax && currPos.y < this->yMax && currPos.z < this->zMax) {
        numSmaller = countSmaller(currPos, this->pType, numObjects, objects);
        numGreater = countGreater(currPos, this->pType, numObjects, objects);
        
        testSAH = (numSmaller*(std::abs(currPos.x-this->xMin))*(std::abs(currPos.y-this->yMin))*(std::abs(currPos.z-this->zMin)))
                + (numGreater*(std::abs(this->xMax-currPos.x))*(std::abs(this->yMax-currPos.y))*(std::abs(this->zMax-currPos.z)));
        
        if(testSAH < bestSAH || bestSAH < 0) {
            bestSAH = testSAH;
            bestPos = currPos;
        }
        
        currPos.x += TEST_STEP;
        currPos.y += TEST_STEP;
        currPos.z += TEST_STEP;
    }
    
    return bestPos;
}



Geometric* Kd3Node::intersectCast(Ray *ray) {
    float resT = 10000.0; // infinity kinda
    float tempT;
    Geometric* returnShape = NULL;
    
    for(int i = 0; i < this->numObjects; i++) {
        tempT = this->objects[i]->getIntersect(ray);
        if(tempT > 0 && tempT < resT) {
            resT = tempT;
            returnShape = this->objects[i];
            returnShape->timeHit = resT;
        }
    }
    
    return returnShape;
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
    int greater = 0;
    for(int i = 0; i < numObjects; i++) {
        if(objects[i]->isGreater(splitPos, pType)) {
            greater++;
        }
    }
    return greater;
}

Geometric** getSmaller(glm::vec3 splitPos, PlaneType pType, int numObjects, Geometric** objects) {
    int numSmaller = countSmaller(splitPos, pType, numObjects, objects);
    Geometric** smaller = new Geometric *[numSmaller];
    
    int count = 0;
    for(int i = 0; i < numObjects; i++) {
        if(objects[i]->isLess(splitPos, pType)) {
            smaller[count] = objects[i];
            count++;
        }
    }
    
    return smaller;
}


Geometric** getGreater(glm::vec3 splitPos, PlaneType pType, int numObjects, Geometric** objects) {
    int numGreater = countGreater(splitPos, pType, numObjects, objects);
    Geometric** greater = new Geometric *[numGreater];
    
    int count = 0;
    for(int i = 0; i < numObjects; i++) {
        if(objects[i]->isGreater(splitPos, pType)) {
            greater[count] = objects[i];
            count++;
        }
    }
    
    return greater;
}

