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
}

Scene::Scene(Camera *camera) {
    this->baseBackground = new Background();
    this->numObjects = 0;
    this->camera = camera;
    this->light = new Light();
    this->numObjects = 0;
    this->numMeshes = 0;
}

Scene::~Scene() {
    delete this->camera;
    delete this->light;
    delete[] this->objects;
    delete[] this->meshes;
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
            distTemp = objects[j]->getDistance(ray->pos + t*ray->dir);
            if(distTemp < dist) {
                dist = distTemp;
                returnShape = objects[j];
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
        tempT = objects[i]->getIntersect(ray);
        if(tempT > 0 && tempT < resT) {
            resT = tempT;
            returnShape = objects[i];
            returnShape->timeHit = resT;
        }
    }
    
    return returnShape;
}



Geometric* Scene::kdTreeCast(Ray *ray) {
    if(this->kdTree == NULL) {
        return intersectCast(ray);
    }
    // this will be modified within the traversal
    Geometric* retObj = this->kdTree->traverse(ray, ray->pos);
    if(retObj == NULL) {
        retObj = this->baseBackground;
    }
    
    return retObj;
}


// The naieve approach just recreate it.
void Scene::updateDataStrucutre() {
    delete this->kdTree;
    this->kdTree = new Kd3Node(MIN, MAX, MIN, MAX, MIN, MAX,
                               this->numObjects, this->objects, 0);
}


void Scene::addMeshObj(Mesh *meshObj) {
    this->numMeshes++;
    if(this->numMeshes <= 1) {
        this->meshes = (Mesh**) malloc(sizeof(Mesh*));
    } else {
        this->meshes = (Mesh**) realloc(this->meshes, this->numMeshes*sizeof(Mesh*));
    }
    if(this->objects == NULL) {
        delete[] this->meshes;
        std::cerr << "Error (re)allocating memory for adding Mesh obj" << std::endl;
        exit(1);
        
    }
    this->meshes[numMeshes-1] = meshObj;
    
    int start = this->numObjects;
    int i = 0;
    this->numObjects += meshObj->numTriangles;
    if(start < 1) {
        this->objects = (Geometric**) malloc(sizeof(Geometric*));
    } else {
        this->objects = (Geometric**) realloc(this->objects, this->numObjects*sizeof(Geometric*));
    }
    if(this->objects == NULL) {
        delete[] this->objects;
        std::cerr << "Error reallocating memory for adding geometric obj from Mesh" << std::endl;
        exit(1);
        
    }
    for( ; start < this->numObjects; start++) {
        this->objects[start] = meshObj->triangles[i];
        i++;
    }
    
    updateDataStrucutre();
}


void Scene::addGeometricObj(Geometric *geomObj) {
    this->numObjects++;
    if(this->numObjects <= 1) {
        this->objects = (Geometric**) malloc(sizeof(Geometric*));
    } else {
        this->objects = (Geometric**) realloc(this->objects, this->numObjects*sizeof(Geometric*));
    }
    if(this->objects == NULL) {
        delete[] this->objects;
        std::cerr << "Error reallocating memory for adding geometric obj" << std::endl;
        exit(1);
        
    }
    this->objects[this->numObjects-1] = geomObj;
    
    updateDataStrucutre();
}




// The scene from Turner Whitted's ray tracing paper
Scene* createTurnerWhitted() {
    Scene *scene = new Scene();
    scene->camera = new Camera(glm::vec3(1.0, 1.3, 2.2), glm::vec3(1.0, 1.1, -1.0));
    scene->light = new Light();
    scene->numObjects = 3;
    scene->objects = new Geometric *[scene->numObjects];
    Plane *plane = new Plane();
    plane->setxLimit(glm::vec2(-5, 3));
    plane->setzLimit(glm::vec2(-10, 5));
    scene->objects[0] = plane;
    scene->objects[0]->ambCoeff = 0.15;
    scene->objects[0]->diffCoeff = 0.95;
    scene->objects[0]->specCoeff = 0.5;
    scene->objects[0]->specExp = 20.0;
    
    scene->objects[1] = new Sphere(glm::vec3(0.1, 1.0, -1.0), 0.65, glm::vec3(0.7));
    scene->objects[1]->reflective = true;
    scene->objects[1]->ambCoeff = 0.15;
    scene->objects[1]->diffCoeff = 0.25;
    scene->objects[1]->specCoeff = 1.0;
    scene->objects[1]->specExp = 20.0;
    scene->objects[1]->kT = 0;
    scene->objects[1]->kR = 0.75;
    
    scene->objects[2] = new Sphere(glm::vec3(1.2, 1.4, 0.2), 0.7, glm::vec3(1.0, 1.0, 1.0));
    scene->objects[2]->transmitive = true;
    scene->objects[2]->ambCoeff = 0.15;
    scene->objects[2]->diffCoeff = 0.075;
    scene->objects[2]->specCoeff = 0.2;
    scene->objects[2]->specExp = 20.0;
    scene->objects[2]->kT = 0.8;
    scene->objects[2]->kR = 0.01;
    scene->objects[2]->refractIndex = 0.95;
    
    scene->kdTree = new Kd3Node(MIN, MAX, MIN, MAX, MIN, MAX,
                               scene->numObjects, scene->objects, 0);
    
    return scene;
}

Scene* createMeshTest() {
    Scene *scene = new Scene();
    scene->camera = new Camera(glm::vec3(0.0, 2.0, 5.0), glm::vec3(0.0, 1.0, -1.0));
    scene->light = new Light();
    scene->numObjects = 1;
    scene->objects = new Geometric *[scene->numObjects];
    Plane *plane = new Plane();
    scene->objects[0] = plane;
    scene->objects[0]->ambCoeff = 0.15;
    scene->objects[0]->diffCoeff = 0.95;
    scene->objects[0]->specCoeff = 0.5;
    scene->objects[0]->specExp = 20.0;
    
    return scene;
}

