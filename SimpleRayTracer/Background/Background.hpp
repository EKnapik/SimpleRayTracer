//
//  Background.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 2/10/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//

#ifndef Background_hpp
#define Background_hpp

#include <stdio.h>
#include "glm/vec3.hpp"
#include "Geometric.hpp"

class Background: public Geometric {
public:
    Background();
    
    glm::vec3 getNormal(glm::vec3 pos);
    float getIntersect(glm::vec3 rayOr, glm::vec3 rayDir);
    float getDistance(glm::vec3 pos);
    glm::vec3 getColor(glm::vec3 pos);
    void setColor(glm::vec3 color);
    float timeHit = -1.0;
private:
    glm::vec3 color;
};
#endif /* Background_hpp */
