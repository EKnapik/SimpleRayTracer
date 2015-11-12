//
//  Camera.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/19/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#include "Camera.hpp"


Camera::Camera(Vec3 *pos, Vec3 *lookAtPoint, float roll) {
    this->pos = pos;
    this->lookAtPoint = lookAtPoint;
    this->roll = roll;
    this->focalLen = 1.0;
    createCamMatrix();
}


Camera::Camera(Vec3 *pos, Vec3 *lookAtPoint) {
    this->pos = pos;
    this->lookAtPoint = lookAtPoint;
    this->roll = 0.0;
    this->focalLen = 1.0;
    createCamMatrix();
}


Vec3* Camera::getRayDir(int row, int col) {
    //vec3 rayDir = camMat * normalize(vec3(p.xy, focalLen));
    return new Vec3(0.0);
}



// Updates the camera matrix with the current position and lookAtPoint of the
// Camera
void Camera::createCamMatrix() {
    Vec3 *cw, *cp, *cu, *cv;
    
    
}