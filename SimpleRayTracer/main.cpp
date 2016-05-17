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
#include <time.h>
#include "RayTracer.hpp"


// 16 x 9
#define WINDOW_HEIGHT 450
#define WINDOW_WIDTH 800

void initOpenGL(void);
void display(void);

RayTracer *rayTracer;
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
    Mesh *myMesh1 = new Mesh("cube.obj");
    myMesh1->scale(2.0);
    myMesh1->moveBy(glm::vec3(0.0, 1.5, 0.0));
    //Mesh *myMesh2 = new Mesh("cube.obj");
    //Mesh *myMesh3 = new Mesh("cube.obj");
    //myMesh2->moveBy(glm::vec3(1.5, 0.5, 0.0));
    //myMesh3->moveBy(glm::vec3(-1.0, 1.5, -1.0));
    scene = createTurnerWhitted();
    //scene->addMeshObj(myMesh1);
    //scene->addMeshObj(myMesh2);
    //scene->addMeshObj(myMesh3);
    rayTracer = new RayTracer(scene);
    
    glutMainLoop();
    
    delete rayTracer;
    delete scene;
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
    clock_t t;
    t = clock();
    float rayTime;
    rayTracer->raytraceScene();
    t = clock() - t;
    rayTime = t;
    
    printf("RayTracer took (%.4f seconds)\n",((float)rayTime)/CLOCKS_PER_SEC);
    
    glutSwapBuffers();
    
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!#!#!#!#!#!#!
    // Doing this for now because I only really care about 1 frame for most cases
    // glutPostRedisplay();
}


