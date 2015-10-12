//
//  RayTracer.hpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/11/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#ifndef RayTracer_hpp
#define RayTracer_hpp
#include <OpenGL/gl.h>

/*
 This draws two triangles over the entire rendering scene then given a scene
 and a window size and width or the window itself. Will create a texture that
 will be draw on those two triangles
 */

class RayTacer {
public:
    void render(void);
private:
    const GLuint numVerts;
    const GLfloat screneData[30];
};


#endif /* RayTracer_hpp */

/* TODO
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
 
 
 */
