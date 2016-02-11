//
//  Light.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 2/10/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//

#include "Light.hpp"

Light::Light() {
    this->pos = glm::vec3(1.5, 5.0, 6.0);
    this->color = glm::vec3(1.0, 0.9, 0.7);
}

Light::Light(glm::vec3 pos, glm::vec3 color) {
    this->pos = pos;
    this->color = color;
}