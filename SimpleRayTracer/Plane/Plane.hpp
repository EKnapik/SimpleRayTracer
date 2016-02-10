//
//  Plane.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/19/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#ifndef Plane_hpp
#define Plane_hpp

#include <stdio.h>
#include <math.h>
#include "Geometric.hpp"
#include "glm/vec3.hpp"
#include "glm/glm.hpp"

class Plane: public Geometric {
public:
    Plane();
    Plane(glm::vec3 pos);
    Plane(glm::vec3 pos, glm::vec3 normal);
    Plane(glm::vec3 pos, glm::vec3 normal, glm::vec3 color);
    
    glm::vec3 getNormal();
    float getIntersect(glm::vec3 rayOr, glm::vec3 rayDir);
    float getDistance(glm::vec3 pos);
    glm::vec3 getColor(glm::vec3 pos);
    void setNormal(glm::vec3 normal);
    void setPos(glm::vec3 pos);
    void setColor(glm::vec3 color);
private:
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec3 color;
    // lamda material function
};


#endif /* Plane_hpp */
