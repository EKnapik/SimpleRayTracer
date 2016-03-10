//
//  FluidMath.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 3/10/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//

#include "FluidMath.hpp"


// W smoothing kernal quite arbitrary
float W(glm::vec3 r, glm::vec3 rb, float h) {
    float scale = 315 / (64 * pi * pow(h,9));
    float inner = (h*h) - glm::dot(r-rb, r-rb);
    return scale * pow(inner, 3);
}

// The gradient of W smoothing kernal
glm::vec3 gradientW(glm::vec3 r, glm::vec3 rb, float h) {
    float scale = -45 / (pi * pow(h, 6));
    glm::vec3 vector = glm::normalize(r-rb);
    return scale * vector * float(pow(h-glm::length(r-rb),2));
}

// the graident squared of W smoothing kernal
float gradientSquaredW(glm::vec3 r, glm::vec3 rb, float h) {
    float scale = 45 / (pi * pow(h, 2));
    return scale * (h-glm::length(r-rb));
}