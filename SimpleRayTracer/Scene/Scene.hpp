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
#include "Geometric.hpp"
#include "Background.hpp"
#include "glm/vec3.hpp"



class Scene {
public:
    Scene();
    Scene(Camera *camera);
    
    Geometric* intersectMarch(glm::vec3 rayOr, glm::vec3 rayDir);
    Geometric* intersectCast(glm::vec3 rayOr, glm::vec3 rayDir);
    
    Camera *camera;
    int numObjects;
    Geometric **shapes;
private:
    Background *baseBackground;
};

#endif /* Scene_hpp */
