//
//  Triangle.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 3/9/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//

#ifndef Triangle_hpp
#define Triangle_hpp

#include <stdio.h>
#include "Geometric.hpp"
#include "glm/vec3.hpp"
#include "glm/glm.hpp"

class Triangle: public Geometric{
public:
    Triangle(glm::vec3 x1, glm::vec3 x2, glm::vec3 x3);
    
    // must be implemented because this extends Geometric
    // Geometric
    glm::vec3 getNormal(glm::vec3 pos);
    float getIntersect(Ray *ray);
    float getDistance(glm::vec3 pos);
    // color must be implemented but it makes sense that the mesh object
    // knows the color this would be a forward backward relation between
    // subclasses... decisions decisions
    glm::vec3 getColor(glm::vec3 pos);
    
    // Collision
    bool doesCollideWith(Geometric *obj);
    void mirrorCollisionHandling(Geometric *obj);
    
    glm::vec3 v1;
    glm::vec3 v2;
    glm::vec3 v3;
    
    glm::vec3 vn1;
    glm::vec3 vn2;
    glm::vec3 vn3;
};

#endif /* Triangle_hpp */
