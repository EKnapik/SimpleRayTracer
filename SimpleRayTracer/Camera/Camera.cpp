//
//  Camera.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/19/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#include "Camera.hpp"
#include <iostream>

Camera::Camera(glm::vec3 pos, glm::vec3 lookAtPoint, float roll) {
    this->pos = pos;
    this->lookAtPoint = lookAtPoint;
    this->rollAngle = roll;
    this->focalLen = 1.5;
    createCamMatrix();
}


Camera::Camera(glm::vec3 pos, glm::vec3 lookAtPoint) {
    this->pos = pos;
    this->lookAtPoint = lookAtPoint;
    this->rollAngle = 0.0;
    this->focalLen = 1.5;
    createCamMatrix();
}


glm::vec3 Camera::getRayPos() {
    return this->pos;
}


// pos(0,0) translates to uv(0,0)
// pos(N,M) translates to uv(1,1)
// Returns a normalized ray direction value
glm::vec3 Camera::getRayDir(int row, int col, int height, int width) {
    // convert to 0 -> 1.0 (normalize)
    float fmtRow = float(row) / float(height);
    float fmtCol = float(col) / float(width);
    // convert to -1.0 -> 1.0 screen space
    fmtRow = (2*fmtRow) - 1.0;
    fmtCol = (2*fmtCol) - 1.0;
    // multiply by the aspect ratio
    fmtCol *= float(width) / float(height);
    
    return glm::normalize(camMatrix * glm::normalize(glm::vec3(fmtCol, fmtRow, focalLen)));
}


void Camera::setPos(glm::vec3 newPos) {
    this->pos = newPos;
}


void Camera::setLookAt(glm::vec3 newLookAt) {
    this->lookAtPoint = newLookAt;
}


void Camera::setRoll(float newRollAngle) {
    this->rollAngle = newRollAngle;
}


void Camera::setFocalLen(float newFocalLen) {
    this->focalLen = newFocalLen;
}


// Updates the camera matrix with the current position and lookAtPoint of the
// Camera
void Camera::createCamMatrix() {
    glm::vec3 cw, cp, cu, cv;
    double radianAngle = rollAngle / 180 * 3.14159;
    cw = glm::normalize(this->lookAtPoint - this->pos); // forward vector
    cp = glm::vec3(sin(radianAngle), cos(radianAngle), 0.0); // temp up
    cu = glm::normalize(glm::cross(cw, cp)); // right vector
    cv = glm::normalize(glm::cross(cu, cw)); // up vector
    this->camMatrix = glm::mat3(cu, cv, cw);
}
