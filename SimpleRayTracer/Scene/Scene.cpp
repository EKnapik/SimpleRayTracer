//
//  Scene.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/19/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#include "Scene.hpp"
#include <iostream>

Scene::Scene() {
    this->baseBackground = new Background();
    this->numObjects = 0;
    this->numMeshes = 0;
    this->numParticles = 0;
}

Scene::Scene(Camera *camera) {
    this->baseBackground = new Background();
    this->numObjects = 0;
    this->camera = camera;
    this->light = new Light();
    this->numParticles = 0;
}

Scene::~Scene() {
    delete this->camera;
    delete this->light;
    delete[] this->shapes;
    delete[] this->meshes;
    delete[] this->particles;
}

// MARCHES FOR GEOMETRICS AND PARTICLES
Geometric* Scene::intersectMarch(Ray *ray) {
    float tmin = 0.0;
    float tmax = 60.0;
    float t = tmin;
    
    float const precis = 0.0001;
    int const steps = 100;
    Geometric *returnShape = this->baseBackground;
    if(numObjects == 0) {
        return returnShape;
    }
    
    for(int i = 0; i < steps; i++) {
        float dist = tmax;
        float distTemp;
        for(int j = 0; j < numObjects; j++) {
            distTemp = shapes[j]->getDistance(ray->pos + t*ray->dir);
            if(distTemp < dist) {
                dist = distTemp;
                returnShape = shapes[j];
            }
        }
        for(int j = 0; j < numParticles; j++) {
            distTemp = particles[j]->getDistance(ray->pos + t*ray->dir);
            if(distTemp < dist) {
                dist = distTemp;
                returnShape = particles[j];
            }
        }
        
        if(dist < precis || t > tmax) {
            break;
        }
        t += dist;
    }
    
    if( t>tmax ) {
        return this->baseBackground;
    }
    returnShape->timeHit = t;
    return returnShape;
}

// INTERSECTS THE GEOMETRICS AND THE PARTICLES
Geometric* Scene::intersectCast(Ray *ray) {
    float resT = 10000.0; // infinity kinda
    float tempT;
    Geometric *returnShape = this->baseBackground;
    
    for(int i = 0; i < this->numObjects; i++) {
        tempT = shapes[i]->getIntersect(ray);
        if(tempT > 0 && tempT < resT) {
            resT = tempT;
            returnShape = shapes[i];
            returnShape->timeHit = resT;
        }
    }
    
    for(int i = 0; i < this->numParticles; i++) {
        tempT = particles[i]->getIntersect(ray);
        if(tempT > 0 && tempT < resT) {
            resT = tempT;
            returnShape = particles[i];
            returnShape->timeHit = resT;
        }
    }
    
    return returnShape;
}


void Scene::updateDataStrucutre() {
    
}


void Scene::addMeshObj(Mesh *meshObj) {
    this->numMeshes++;
    this->meshes = (Mesh**) realloc(this->meshes, this->numMeshes*sizeof(Mesh*));
    if(this->shapes == NULL) {
        delete[] this->meshes;
        std::cerr << "Error reallocating memory for adding Mesh obj" << std::endl;
        exit(1);
        
    }
    this->meshes[numMeshes-1] = meshObj;
    
    int start = this->numObjects;
    int i = 0;
    this->numObjects += meshObj->numTriangles;
    this->shapes = (Geometric**) realloc(this->shapes, this->numObjects*sizeof(Geometric*));
    if(this->shapes == NULL) {
        delete[] this->shapes;
        std::cerr << "Error reallocating memory for adding geometric obj from Mesh" << std::endl;
        exit(1);
        
    }
    for( ; start < this->numObjects; start++) {
        this->shapes[start] = meshObj->triangles[i];
        i++;
    }
    
    updateDataStrucutre();
}


void Scene::addGeometricObj(Geometric *geomObj) {
    this->numObjects++;
    this->shapes = (Geometric**) realloc(this->shapes, this->numObjects*sizeof(Geometric*));
    if(this->shapes == NULL) {
        delete[] this->shapes;
        std::cerr << "Error reallocating memory for adding geometric obj" << std::endl;
        exit(1);
        
    }
    this->shapes[this->numObjects-1] = geomObj;
    
    updateDataStrucutre();
}




// The scene from Turner Whitted's ray tracing paper
Scene* createTurnerWhitted() {
    Scene *scene = new Scene();
    scene->camera = new Camera(glm::vec3(1.0, 1.3, 2.2), glm::vec3(1.0, 1.1, -1.0));
    scene->light = new Light();
    scene->numObjects = 3;
    scene->shapes = new Geometric *[scene->numObjects];
    Plane *plane = new Plane();
    plane->setxLimit(glm::vec2(-5, 3));
    plane->setzLimit(glm::vec2(-10, 5));
    scene->shapes[0] = plane;
    scene->shapes[0]->ambCoeff = 0.15;
    scene->shapes[0]->diffCoeff = 0.95;
    scene->shapes[0]->specCoeff = 0.5;
    scene->shapes[0]->specExp = 20.0;
    
    scene->shapes[1] = new Sphere(glm::vec3(0.1, 1.0, -1.0), 0.65, glm::vec3(0.7));
    scene->shapes[1]->reflective = true;
    scene->shapes[1]->ambCoeff = 0.15;
    scene->shapes[1]->diffCoeff = 0.25;
    scene->shapes[1]->specCoeff = 1.0;
    scene->shapes[1]->specExp = 20.0;
    scene->shapes[1]->kT = 0;
    scene->shapes[1]->kR = 0.75;
    
    scene->shapes[2] = new Sphere(glm::vec3(1.2, 1.4, 0.2), 0.7, glm::vec3(1.0, 1.0, 1.0));
    scene->shapes[2]->transmitive = true;
    scene->shapes[2]->ambCoeff = 0.15;
    scene->shapes[2]->diffCoeff = 0.075;
    scene->shapes[2]->specCoeff = 0.2;
    scene->shapes[2]->specExp = 20.0;
    scene->shapes[2]->kT = 0.8;
    scene->shapes[2]->kR = 0.01;
    scene->shapes[2]->refractIndex = 0.95;
    
    scene->numParticles = 0;
    
    return scene;
}


// FLUID TEST
// Basic fluid simulation setup for use with physics engine and particle simulation
Scene* createFluidTest() {
    Scene *scene = new Scene();
    
    scene->camera = new Camera(glm::vec3(1.0, 1.1, 2.0), glm::vec3(1.0, 1.1, -1.0));
    scene->light = new Light();
    
    scene->numObjects = 1;
    scene->shapes = new Geometric *[scene->numObjects];
    Plane *plane = new Plane();
    plane->setxLimit(glm::vec2(-20, 20));
    plane->setzLimit(glm::vec2(-20, 20));
    scene->shapes[0] = plane;
    
    scene->numParticles = FLUID_NUM_PARTICLES;
    scene->particles = new FluidParticle *[scene->numParticles];
    
    float currX = 0.0;
    float currY = 0.4;
    float currZ = 0.0;
    
    int tmp = powf(scene->numParticles, float(1.0/3.0));
    int count = 0;
    for(int i = 0; i < tmp; i++) {
        currZ = 0.0;
        for(int j = 0; j < tmp; j++) {
            currX = 0.0;
            for(int w = 0; w < tmp; w++) {
                scene->particles[count] = new FluidParticle(glm::vec3(currX, currY, currZ));
                count++;
                currX += (2*FLUID_RADIUS);
            }
            currZ -= (2*FLUID_RADIUS);
        }
        currY += (2*FLUID_RADIUS);
    }
    
    
    //for(int i = 0; i < numParticles; i++) {
    //    scene->particles[i] = new FluidParticle(glm::vec3(currX, 1.0, 0.0));
    //    currX += (2*FLUID_RADIUS);
    //}
    
    return scene;
}
