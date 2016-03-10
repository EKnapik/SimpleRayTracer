//
//  PhysicsEngine.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 3/10/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//

#ifndef PhysicsEngine_hpp
#define PhysicsEngine_hpp

#include <stdio.h>
#include "Scene.hpp"

#define DEFAULT_TIME_DELTA float(0.001)

class PhysicsEngine {
public:
    PhysicsEngine();
    PhysicsEngine(Scene *scene);
    void applyTimeStep();
    void applyTimeStep(float timeDelta);
    
    Scene *scene;
};

#endif /* PhysicsEngine_hpp */
