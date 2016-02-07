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


class Camera {
public:
    /*
    Camera(Vec3 *pos, Vec3 *lookAtPoint, float roll);
    Camera(Vec3 *pos, Vec3 *lookAtPoint);
    Vec3* getRayDir(int row, int col);
    
    Mat3 *camMatrix;
    Vec3 *pos;
    Vec3 *lookAtPoint;
    float roll, focalLen;
    */
private:
    void createCamMatrix();
    
};

#endif /* Camera_hpp */
