//
//  Camera.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/19/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#include "Camera.hpp"

Camera::Camera(glm::vec3 pos, glm::vec3 lookAtPoint, float roll) {
    this->pos = pos;
    this->lookAtPoint = lookAtPoint;
    this->roll = roll;
    this->focalLen = 1.0;
    createCamMatrix();
}


Camera::Camera(glm::vec3 pos, glm::vec3 lookAtPoint) {
    this->pos = pos;
    this->lookAtPoint = lookAtPoint;
    this->roll = 0.0;
    this->focalLen = 1.0;
    createCamMatrix();
}


glm::vec3 Camera::getRayDir(int row, int col) {
    //vec3 rayDir = camMat * normalize(vec3(p.xy, focalLen));
    return glm::vec3(0.0);
}



// Updates the camera matrix with the current position and lookAtPoint of the
// Camera
void Camera::createCamMatrix() {
    glm::vec3 cw, cp, cu, cv;
    cw = glm::normalize(this->lookAtPoint - this->pos);
    
    
    
    
}
