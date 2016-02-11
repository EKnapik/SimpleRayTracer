//
//  Camera.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/19/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include <math.h>
#include "glm/vec3.hpp"
#include "glm/mat3x3.hpp"
#include "glm/glm.hpp"



class Camera {
public:
    Camera(glm::vec3 pos, glm::vec3 lookAtPoint, float roll);
    Camera(glm::vec3 pos, glm::vec3 lookAtPoint);
    glm::vec3 getRayPos();
    glm::vec3 getRayDir(int row, int col, int height, int width);
    void setPos(glm::vec3 newPos);
    void setLookAt(glm::vec3 newLookAt);
    void setRoll(float newRollAngle);
    void setFocalLen(float newFocalLen);
    void createCamMatrix();
    
private:
    glm::mat3x3 camMatrix;
    glm::vec3 pos;
    glm::vec3 lookAtPoint;
    float rollAngle, focalLen;
    
};

#endif /* Camera_hpp */
