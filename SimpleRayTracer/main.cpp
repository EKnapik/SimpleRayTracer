//
//  main.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/11/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#include <iostream>
#include "RayTracer.hpp"
#include "PhysicsEngine.hpp"


// 16 x 9
#define WINDOW_HEIGHT 450
#define WINDOW_WIDTH 800

#define TIMESTEP float(0.002)

void initOpenGL(void);
void display(void);

RayTracer *rayTracer;
PhysicsEngine *physEngine;
Scene *scene;

int main(int argc, char * argv[]) {
    
    // Setup window statically because GLUT is poor at window and display render
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Simple Ray Tracer");
    glutDisplayFunc(display);
    initOpenGL();
    
    // Setup the ray tracer
    scene = new Scene();
    rayTracer = new RayTracer(scene);
    physEngine = new PhysicsEngine(scene);
    
    glutMainLoop();
    return 0;
}


void initOpenGL(void) {
    glEnable(GL_DEPTH_TEST | GL_CULL_FACE | GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor( 0.0, 0.0, 0.0, 1.0 ); 
    glCullFace(GL_FRONT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    return;
}


void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // poll events
    // bind and draw
    rayTracer->raytraceScene();
    physEngine->applyTimeStep(); // DO THE PHYSICS
    //printf("X: %.2f, Y: %.2f, Z: %.2f\n",physEngine->scene->particles[0]->pos.x, physEngine->scene->particles[0]->pos.y, physEngine->scene->particles[0]->pos.z);
    
    //rayTracer->scene->shapes[1]->velocity.y -= 9.8 * TIMESTEP;
    //rayTracer->scene->shapes[1]->pos += rayTracer->scene->shapes[1]->velocity * float(0.02);
    //rayTracer->scene->shapes[1]->mirrorCollisionHandling(rayTracer->scene->shapes[0]);
    
    glutSwapBuffers();
    glutPostRedisplay();
}


