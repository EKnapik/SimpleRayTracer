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


#define WINDOW_HEIGHT 512
#define WINDOW_WIDTH 512

void initOpenGL(void);
void display(void);

RayTracer *rayTracer;

int main(int argc, char * argv[]) {
    
    // Setup window statically because GLUT is poor at window and display render
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_HEIGHT, WINDOW_WIDTH);
    glutCreateWindow("Simple Ray Tracer");
    glutDisplayFunc(display);
    
    initOpenGL();
    
    rayTracer = new RayTracer();
    
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
    
    glutSwapBuffers();
    glutPostRedisplay();
}


/*
 Ideally there would be a main overal program that creates the scene of objects
 The rays would then shoot into the scene and color pixels
 
 Create window:
 Create scene
 Create rayTracer that takes a scene
    Create two triangles and draws to the screne
    Create a texture from raytraced pixels what to draw
        Simple Shader of setting texture CPU processing
    Create a specific shader for the scene all hard coded
 Create a Display function <- hard coded to the window connection
    Can Take a rayTracer to use.

*/