//
//  Triangle.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 3/9/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//

#ifndef Triangle_hpp
#define Triangle_hpp

#include "Geometric.hpp"
#include "glm/vec3.hpp"
#include "glm/glm.hpp"

class Triangle: public Geometric{
public:
    Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
    Triangle(glm::vec3 v1, glm::vec3 vNorm1, glm::vec3 v2, glm::vec3 vNorm2, glm::vec3 v3, glm::vec3 vNorm3);
    
    // must be implemented because this extends Geometric
    // Geometric
    glm::vec3 getNormal(glm::vec3 pos); // based on how the triangle is given
    float getIntersect(Ray *ray);
    float getDistance(glm::vec3 pos);
    // color must be implemented but it makes sense that the mesh object
    // knows the color this would be a forward backward relation between
    // subclasses... decisions decisions
    glm::vec3 getColor(glm::vec3 pos);
    bool isLess(glm::vec3 pos, PlaneType pType);
    
    // Collision
    bool doesCollideWith(Geometric *obj);
    void mirrorCollisionHandling(Geometric *obj, float timeStep);
    
    glm::vec3 v1;
    glm::vec3 v2;
    glm::vec3 v3;
    glm::vec3 color;
private:
    glm::vec3 vNorm1;
    glm::vec3 vNorm2;
    glm::vec3 vNorm3;
};

#endif /* Triangle_hpp */


/*
 * My idea right now is to have basic geometric object be moveable that is
 * I can just adjust their position and everything will work. However this
 * does not work for triangles because they are in the scene description
 * but the physics engine should not be moving the triangle but the model
 * itself which in turn will move the triangles.
 */