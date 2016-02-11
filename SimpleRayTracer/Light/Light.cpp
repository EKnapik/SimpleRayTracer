//
//  Light.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 2/10/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//

#include "Light.hpp"

Light::Light(glm::vec3 pos, glm::vec3 color) {
    this->pos = pos;
    this->color = color;
}