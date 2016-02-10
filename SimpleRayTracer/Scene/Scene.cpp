//
//  Scene.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/19/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#include "Scene.hpp"

Scene::Scene() {
    
}

Scene::Scene(Camera *camera) {
    this->camera = camera;
}