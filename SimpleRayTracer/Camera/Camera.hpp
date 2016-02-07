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
#include "glm/vec3.hpp"
#include "glm/mat3x3.hpp"


class Camera {
public:
    Camera(glm::vec3 pos, glm::vec3 lookAtPoint, float roll);
    Camera(glm::vec3 pos, glm::vec3 lookAtPoint);
    glm::vec3 getRayDir(int row, int col);
    
    glm::mat3x3 camMatrix;
    glm::vec3 pos;
    glm::vec3 lookAtPoint;
    float roll, focalLen;
private:
    void createCamMatrix();
    
};

#endif /* Camera_hpp */
