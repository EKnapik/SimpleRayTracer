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
    if(this->nodeDepth >= MAX_NODE_DEPTH) {
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
    glm::vec3 minUpper;
    glm::vec3 maxLower;
    switch (this->pType) {
        case XY:
            minUpper = glm::vec3(xMax, yMax, this->planePos.z);
            maxLower = glm::vec3(xMin, yMin, this->planePos.z);
            break;
        case YZ:
            minUpper = glm::vec3(this->planePos.x, yMax, zMax);
            maxLower = glm::vec3(this->planePos.x, yMin, zMin);
            break;
        case XZ:
            minUpper = glm::vec3(xMax, this->planePos.y, zMax);
            maxLower = glm::vec3(xMin, this->planePos.y, zMin);
            break;
    }
    // Find Objects Smaller and Greater than split plane
    int numSmaller = countSmaller(xMin, minUpper.x, yMin, minUpper.y, zMin, minUpper.z, numObjects, objects);
    int numGreater = countGreater(maxLower.x, xMax, maxLower.y, yMax, maxLower.z, zMax, numObjects, objects);
    if(numSmaller > 0) {
        this->smaller = new Kd3Node(xMin, minUpper.x, yMin, minUpper.y, zMin, minUpper.z, numSmaller,
                                    getSmaller(xMin, minUpper.x, yMin, minUpper.y, zMin, minUpper.z, numObjects, objects),
                                    curDepth+1);
    }
    if(numGreater > 0) {
        this->greater = new Kd3Node(maxLower.x, xMax, maxLower.y, yMax, maxLower.z, zMax, numGreater,
                                    getGreater(maxLower.x, xMax, maxLower.y, yMax, maxLower.z, zMax, numObjects, objects),
                                    curDepth+1);
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

// THe temp ray moves to the intersection point
Geometric* Kd3Node::traverse(Ray *ray, glm::vec3 tmpPos) {
    if(this->smaller == NULL && this->greater == NULL) {
        return intersectCast(ray);
    }
    
    /*
    // FULL TREE TRAVERSAL
    Geometric *tmpObj1 = NULL, *tmpObj2 = NULL;
    if(this->smaller != NULL) {
        tmpObj1 = this->smaller->traverse(ray, tmpPos);
    }
    if(this->greater != NULL) {
        
        tmpObj2 = this->greater->traverse(ray, tmpPos);
    }
    
    if(tmpObj1 == NULL) {
        return tmpObj2;
    } else if(tmpObj2 == NULL) {
        return tmpObj1;
    } else if(tmpObj1->timeHit < tmpObj2->timeHit) {
        return tmpObj1;
    } else {
        return tmpObj2;
    }
    */
    
    
    // !!! SWITCHED TO NOT HAVE THE MOVED POSITION
    glm::vec3 planeNorm;
    bool inSmaller = false;
    bool inGreater = false;
    bool inBounds = false;
    switch (this->pType) {
        case XY:
            planeNorm = glm::vec3(0.0, 0.0, 1.0);
            if(ray->pos.z < this->planePos.z) {
                inSmaller = true;
            } else {
                inGreater = true;
            }
            break;
        case YZ:
            planeNorm = glm::vec3(1.0, 0.0, 0.0);
            if(ray->pos.x < this->planePos.x) {
                inSmaller = true;
            } else {
                inGreater = true;
            }
            break;
        case XZ:
            planeNorm = glm::vec3(0.0, 1.0, 0.0);
            if(ray->pos.y < this->planePos.y) {
                inSmaller = true;
            } else {
                inGreater = true;
            }
            break;
        default:
            std::cerr << "Error Ray Traversing the Kd3Node" << std::endl;
            exit(1);
            break;
    }
    
    Geometric* retObj = NULL;
    // check for ray and plane being parallel
    float denom = glm::dot(ray->dir, planeNorm);
    if(denom < 0.00001 && denom > -0.00001) {
        if(inSmaller) {
            if(this->smaller != NULL) {
                retObj = this->smaller->traverse(ray, tmpPos);
            }
        } else {
            if(this->greater != NULL) {
                retObj = this->greater->traverse(ray, tmpPos);
            }
        }
        return retObj;
    }
    
    // intersect the plane and traverse accordingly:
    float hitTime;
    hitTime = (glm::dot((this->planePos - ray->pos), planeNorm) / denom);
    glm::vec3 posHit;
    if(hitTime < 0) {
        // Pointing away from the plane
        if(inSmaller) {
            if(this->smaller != NULL) {
                retObj = this->smaller->traverse(ray, tmpPos);
            }
        } else {
            if(this->greater != NULL) {
                retObj = this->greater->traverse(ray, tmpPos);
            }
        }
    } else {
        posHit = tmpPos + ray->dir*hitTime;
        switch (this->pType) {
            case XY:
                if(posHit.x > this->xMax || posHit.y > this->yMax ||
                   posHit.x < this->xMin || posHit.y < this->yMin) {
                    inBounds = false;
                } else {
                    inBounds = true;
                }
                break;
            case YZ:
                if(posHit.y > this->yMax || posHit.z > this->zMax ||
                   posHit.y < this->yMin || posHit.z < this->zMin) {
                    inBounds = false;
                } else {
                    inBounds = true;
                }
                break;
            case XZ:
                if(posHit.x > this->xMax || posHit.z > this->zMax ||
                   posHit.x < this->xMin || posHit.z < this->zMin) {
                    inBounds = false;
                } else {
                    inBounds = true;
                }
                break;
            default:
                std::cerr << "Error Ray Traversing the Kd3Node" << std::endl;
                exit(1);
                break;
        }
        if(!inBounds) {
            if(inSmaller) {
                if(this->smaller != NULL) {
                    retObj = this->smaller->traverse(ray, tmpPos);
                }
            } else {
                if(this->greater != NULL) {
                    retObj = this->greater->traverse(ray, tmpPos);
                }
            }
        } else {
            if(inSmaller) {
                if(this->smaller != NULL) {
                    retObj = this->smaller->traverse(ray, tmpPos);
                }
                if(retObj == NULL && this->greater != NULL) {
                    retObj = this->greater->traverse(ray, posHit);
                }
            } else {
                if(this->greater != NULL) {
                    retObj = this->greater->traverse(ray, tmpPos);
                }
                if(retObj == NULL && this->smaller != NULL) {
                    retObj = this->smaller->traverse(ray, posHit);
                }
            }
        }
    }
    return retObj;


    /*
    if(hitTime < 0) {
        if(smaller) {
            if(this->smaller != NULL) {
                retObj = this->smaller->traverse(ray, tmpPos);
            }
        } else {
            if(this->greater != NULL) {
                retObj = this->greater->traverse(ray, tmpPos);
            }
        }
    }
    glm::vec3 posHit = tmpPos + ray->dir*hitTime;
    if(posHit.x < this->xMax && posHit.y < yMax && posHit.z < zMax) {
        if(this->smaller != NULL) {
            retObj = this->smaller->traverse(ray, tmpPos);
        }
        if(this->greater != NULL && retObj != NULL) {
            retObj = this->smaller->traverse(ray, posHit);
        }
    }
    return retObj;
    /*
    Geometric* retObj = NULL;
    bool smaller = false;
    bool greater = false;
    switch (this->pType) {
        case XY:
            if(tmpPos.z < this->planePos.z) { // within smaller
                smaller = true;
            } else if(tmpPos.z > this->planePos.z){ // within greater
                greater = true;
            }
            break;
        case YZ:
            if(tmpPos.x < this->planePos.x) { // within smaller
                smaller = true;
            } else if(tmpPos.x > this->planePos.x) {
                greater = true;
            }
            break;
        case XZ:
            if(tmpPos.y < this->planePos.y) { // within smaller
                smaller = true;
            } else if(tmpPos.y > this->planePos.y) {
                greater = true;
            }
            break;
    }
    
    // Move tmpRay to the position it hit on the intersecting plane
    // Turns out you can never intersect a plane if you are below it.....
    // Feels bad
    //
    float hitTime;
    if(smaller) { // within the smaller slice
        if(denom < 0) { // pointing away from the greater <- ->
            if(this->smaller != NULL) {
                retObj = this->smaller->traverse(ray, tmpPos);
            }
        } else { // pointing toward the greater -> ->
            if(this->smaller != NULL) {
                retObj = this->smaller->traverse(ray, tmpPos);
            }
            if(this->greater != NULL && retObj == NULL) {
                // denom = glm::dot(ray->dir, planeNorm);
                hitTime = (glm::dot((this->planePos - tmpPos), planeNorm) / denom);
                retObj = this->greater->traverse(ray, tmpPos + (hitTime*ray->dir));
            }
        }
    } else if(greater){ // within the greater slice
        if(denom < 0) { // pointing toward the smaller -> <-
            if(this->greater != NULL) {
                retObj = this->greater->traverse(ray, tmpPos);
            }
            if(this->smaller != NULL && retObj == NULL) {
                hitTime = (glm::dot((this->planePos - tmpPos), planeNorm) / denom);
                retObj = this->smaller->traverse(ray, tmpPos + (hitTime*ray->dir));
            }
        } else { // pointing away from the smaller -> ->
            if(this->greater != NULL) {
                retObj = this->greater->traverse(ray, tmpPos);
            }
        }
    } else { // on the splitting line
        hitTime = (glm::dot((this->planePos - tmpPos), planeNorm) / denom);
        if(denom < 0) {
            if(this->smaller != NULL) {
                retObj = this->smaller->traverse(ray, tmpPos + (hitTime*ray->dir));
            }
        } else {
            if(this->greater != NULL) {
                retObj = this->greater->traverse(ray, tmpPos + (hitTime*ray->dir));
            }
        }
    }
    return retObj;
    */
    
}


void Kd3Node::PRINT() {
    printf("Split Plane at Depth %d: (%.2f, %.2f, %.2f) with %d Objects\t", this->nodeDepth,
           this->planePos.x, this->planePos.y, this->planePos.z, this->numObjects);
    printf("X(%.2f, %.2f)  Y(%.2f, %.2f)  Z(%.2f, %.2f)\n",
           this->xMin, this->xMax, this->yMin, this->yMax, this->zMin, this->zMax);
    
    if(this->smaller == NULL) {
        printf("Smaller(%d): NULL\n", this->nodeDepth);
    } else {
        printf("Smaller(%d):\t", this->nodeDepth);
        this->smaller->PRINT();
    }
    if(this->greater == NULL) {
        printf("Greater(%d): NULL\n", this->nodeDepth);
    } else {
        printf("Greater(%d):\t", this->nodeDepth);
        this->greater->PRINT();
    }
    
}



glm::vec3 Kd3Node::SAH(int numObjects, Geometric** objects) {
    glm::vec3 planeNorm;
    glm::vec3 bestPos;
    float bestSAH = -1;
    glm::vec3 minUpper;
    glm::vec3 maxLower;
    switch (this->pType) {
        case XY:
            planeNorm = glm::vec3(0.0, 0.0, 1.0);
            bestPos = glm::vec3((xMin+xMax)/2.0, (yMin+yMax)/2.0, zMin);
            minUpper = glm::vec3(xMax, yMax, bestPos.z);
            maxLower = glm::vec3(xMin, yMin, bestPos.z);
            break;
        case YZ:
            planeNorm = glm::vec3(1.0, 0.0, 0.0);
            bestPos = glm::vec3(xMin, (yMin+yMax)/2.0, (zMin+zMax)/2.0);
            minUpper = glm::vec3(bestPos.x, yMax, zMax);
            maxLower = glm::vec3(bestPos.x, yMin, zMin);
            break;
        case XZ:
            planeNorm = glm::vec3(0.0, 1.0, 0.0);
            bestPos = glm::vec3((xMin+xMax)/2.0, yMin, (zMin+zMax)/2.0);
            minUpper = glm::vec3(xMax, bestPos.y, zMax);
            maxLower = glm::vec3(xMin, bestPos.y, zMin);
            break;
    }
    glm::vec3 currPos = bestPos;
    
    int numSmaller;
    int numGreater;
    float testSAH;
    while(currPos.x <= this->xMax && currPos.y <= this->yMax && currPos.z <= this->zMax) {
        numSmaller = countSmaller(xMin, minUpper.x, yMin, minUpper.y, zMin, minUpper.z, numObjects, objects);
        numGreater = countGreater(maxLower.x, xMax, maxLower.y, yMax, maxLower.z, zMax, numObjects, objects);
        
        testSAH = (numSmaller*(std::fabsf(currPos.x-this->xMin))*(std::fabsf(currPos.y-this->yMin))*(std::fabsf(currPos.z-this->zMin)))
                + (numGreater*(std::fabsf(this->xMax-currPos.x))*(std::fabsf(this->yMax-currPos.y))*(std::fabsf(this->zMax-currPos.z)));
        
        if(testSAH < bestSAH || bestSAH < 0) {
            bestSAH = testSAH;
            bestPos = currPos;
        }
        
        currPos += TEST_STEP * planeNorm;
        minUpper += TEST_STEP * planeNorm;
        maxLower += TEST_STEP * planeNorm;
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



int countSmaller(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax,
                 int numObjects, Geometric** objects) {
    int smaller = 0;
    for(int i = 0; i < numObjects; i++) {
        if(objects[i]->inBounds(xMin, xMax, yMin, yMax, zMin, zMax)) {
            smaller++;
        }
    }
    return smaller;
}


int countGreater(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax,
                 int numObjects, Geometric** objects) {
    int greater = 0;
    for(int i = 0; i < numObjects; i++) {
        if(objects[i]->inBounds(xMin, xMax, yMin, yMax, zMin, zMax)) {
            greater++;
        }
    }
    return greater;
}

Geometric** getSmaller(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax,
                       int numObjects, Geometric** objects) {
    int numSmaller = countSmaller(xMin, xMax, yMin, yMax, zMin, zMax, numObjects, objects);
    Geometric** smaller = new Geometric *[numSmaller];
    
    int count = 0;
    for(int i = 0; i < numObjects; i++) {
        if(objects[i]->inBounds(xMin, xMax, yMin, yMax, zMin, zMax)) {
            smaller[count] = objects[i];
            count++;
        }
    }
    
    return smaller;
}


Geometric** getGreater(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax,
                       int numObjects, Geometric** objects) {
    int numGreater = countGreater(xMin, xMax, yMin, yMax, zMin, zMax, numObjects, objects);
    Geometric** greater = new Geometric *[numGreater];
    
    int count = 0;
    for(int i = 0; i < numObjects; i++) {
        if(objects[i]->inBounds(xMin, xMax, yMin, yMax, zMin, zMax)) {
            greater[count] = objects[i];
            count++;
        }
    }
    
    return greater;
}

