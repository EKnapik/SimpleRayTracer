//
//  Scene.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/19/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <stdio.h>
#include "Camera.hpp"

class Scene {
public:
    Scene();
    Scene(Camera *camera);
    Camera *camera;
    // array of objects
    
};

#endif /* Scene_hpp */
