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


// 16 x 9
#define WINDOW_HEIGHT 450
#define WINDOW_WIDTH 800

void initOpenGL(void);
void display(void);

RayTracer *rayTracer;

int main(int argc, char * argv[]) {
    
    // Setup window statically because GLUT is poor at window and display render
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Simple Ray Tracer");
    glutDisplayFunc(display);
    initOpenGL();
    
    // Setup the ray tracer
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
    //rayTracer->scene->shapes[1]->pos.y += 0.01;
    
    glutSwapBuffers();
    glutPostRedisplay();
}


