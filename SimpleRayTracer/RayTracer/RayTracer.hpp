//
//  RayTracer.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/11/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#ifndef RayTracer_hpp
#define RayTracer_hpp
#include <cstdio>
#include <OpenGL/gl.h>
#include "shaderSetup.hpp"
#include "Window.hpp"
#include "Scene.hpp"

/*
 This draws two triangles over the entire rendering scene then given a scene
 and a window size and width or the window itself. Will create a texture that
 will be draw on those two triangles
 */

class RayTracer {
public:
    RayTracer(Scene *scenePtr, Window *windowPtr);
    RayTracer(Scene *scenePtr);
    RayTracer();
    void raytraceScene(void);
    void changeScene(Scene *newScene);
    
private:
    void setColor(int x, int y); // <- that is run per pixel updating the 3D matrix
    void setupOpenGLCalls(void);
    void renderToWindow(void); // renders the current 3D matrix to the window
    void populateMatrix(void);
    void setupThreads(void);
    void shutdownThreads(void);
    void setVertexData(void);
    
    // render 2 triangles and texture
    GLuint shaderProgram;
    GLuint vBuffer, eBuffer, texBuffer;
    GLuint vertPos, uvPos, texPos;
    GLuint numVerts;
    GLfloat vertexData[20]; // set in construction
    GLushort elementData[6];
    // some 3D byte array for the texture data
    GLubyte pixelData[14];
    int height, width; // these will be covered by the window when that is fixed
    Scene *scene;
    Window *window;
};


#endif /* RayTracer_hpp */

/* TODO
 * Fresnel Equation for reflection
 
 * Set function to use per pixel including default for constructor which can be
    given one to start with
 * Needs to be given the window dimensions to be create the 3D array
 * Needs the scene to shoot rays into. Scene must have:
    object info
    camera info
 * Spawns threads to determine some particular pixel then collects and sets
    thread returned info to the 3D array
 * Setup the OpenGL code for:
    create 2 triangles that cover screne
    create a texture from 3D array
    pass texture to OpenGL
    compile and use the simple necessary shader code
        might want to create a shader object to use here
  * render function:
    The manager basically.. at this time in the scene what does it lool like and
    display that to the GLUT window. then in the future the GLFW window I am
    given.
 
  * Editing the code for the tracing function would be the marching and color
    determination part. This is what is CUDA cored or cpu pthreaded. First thing
    is get this with ray marching then to take small incrememted steps for black
    hole.
  * also pencile line drawing cause each pixel can be set
  * B-spline for camera movement
*/
