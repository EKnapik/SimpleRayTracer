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
#include "Kd3Node.hpp"


#define MIN -5.0
#define MAX 10.0
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
    Geometric* kdTreeCast(Ray *ray);
    
    Camera *camera = NULL;
    Light *light = NULL;
    
    int numObjects = 0;
    Geometric **objects = NULL;
    
    int numParticles = 0;
    FluidParticle **particles = NULL;
    
    int numMeshes = 0;
    Mesh **meshes = NULL;
    
    // Spacial Data Structure:
    Kd3Node* kdTree = NULL;
    
    void updateDataStrucutre();
    void addMeshObj(Mesh *meshObj);
    void addGeometricObj(Geometric *geomObj);
    
private:
    Background *baseBackground;
};


// The scene from Turner Whitted's ray tracing paper
Scene* createTurnerWhitted();

Scene* createMeshTest();

// FLUID TEST
// Basic fluid simulation setup for use with physics engine and particle simulation
Scene* createFluidTest();


#endif /* Scene_hpp */
