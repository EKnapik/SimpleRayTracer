//
//  Circle.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/19/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#ifndef Circle_hpp
#define Circle_hpp

#include <stdio.h>
#include <math.h>
#include "Geometric.hpp"
#include "glm/vec3.hpp"
#include "glm/glm.hpp"

class Circle: public Geometric {
public:
    Circle(glm::vec3 pos, float radius);
    Circle(glm::vec3 pos, float radius, glm::vec3 color);
    
    glm::vec3 getNormal(glm::vec3 pos);
    float getIntersect(Ray *ray);
    float getDistance(glm::vec3 pos);
    glm::vec3 getColor(glm::vec3 pos);
    void setRadius(float radius);
    void setPos(glm::vec3 pos);
    void setColor(glm::vec3 color);
private:
    float radius;
    glm::vec3 color;
    // lamda material function
};
#endif /* Circle_hpp */
