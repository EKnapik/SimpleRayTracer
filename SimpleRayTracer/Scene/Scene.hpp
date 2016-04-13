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
#include "Mesh.hpp"
#include "Background.hpp"
#include "glm/vec3.hpp"
#include "Ray.hpp"
#include "Plane.hpp"
#include "Light.hpp"
#include "Sphere.hpp"
#include "FluidParticle.hpp"


/* This should be given a file of some sort then will make the array of objects
 * in the scene and do stuff. This is merely the current state of the world
 * the Physics Enginee may adjust the positions the file is merely a starting
 * point.
 */
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
    
    int numParticles;
    FluidParticle **particles;
    
    int numMeshes;
    Mesh **meshes;
    
    // Spacial Data Structure:
    void updateDataStrucutre();
    void addMeshObj(Mesh *meshObj);
    void addGeometricObj(Geometric *geomObj);
    
    float ambientCoeff;
private:
    Background *baseBackground;
};

#endif /* Scene_hpp */
