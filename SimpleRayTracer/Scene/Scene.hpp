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
#include "Ray.hpp"
#include "Plane.hpp"
#include "Circle.hpp"
#include "Light.hpp"



class Scene {
public:
    Scene();
    Scene(Camera *camera);
    ~Scene();
    
    Geometric* intersectMarch(Ray *ray);
    Geometric* intersectCast(Ray *ray);
    
    Camera *camera;
    Light *light;
    int numObjects;
    Geometric **shapes;
    
    float ambientCoeff;
private:
    Background *baseBackground;
};

#endif /* Scene_hpp */
