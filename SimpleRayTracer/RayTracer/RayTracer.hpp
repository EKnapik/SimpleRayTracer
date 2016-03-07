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
#include <iostream>
#include <OpenGL/gl.h>
#include "shaderSetup.hpp"
#include "Window.hpp"
#include "Geometric.hpp"
#include "Scene.hpp"
#include "Ray.hpp"
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/glm.hpp"

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
    Scene *scene;
    Window *window;
    
    // Rendering Flags
    bool superSampling;
    
private:
    void setColor(int row, int col); // <- that is run per pixel updating the 3D matrix
    glm::vec3 shootRay(Ray *ray, int depth);
    void setupOpenGLCalls(void);
    void renderToWindow(void); // renders the current 3D matrix to the window
    void populateMatrix(void);
    void setupThreads(void);
    void shutdownThreads(void);
    void setVertexData(void);
    void sendTexture(void);
    
    // render 2 triangles and texture
    GLuint shaderProgram;
    GLuint vBuffer, eBuffer, texBuffer;
    GLuint vertPos, uvPos, texPos;
    GLuint numVerts;
    GLfloat vertexData[20]; // set in construction
    GLushort elementData[6];
    // some 3D byte array for the texture data
    GLubyte *pixelData;
    int height, width; // these will be covered by the window when that is fixed
    
    
};
#endif /* RayTracer_hpp */

/*
 setColor():<- public
    Given camera and cell position will shoot a ray into the scene to ask
    something from the scene. Things that can be asked from the scene:
        Nearest object and dist to that object
            I guess this would have gravity info if I follow that path
        Intersect
            returns what object is intersected and at what t t
 
 Vect4 Color shootRay(Ray ray, int depth):
    // polls scene for nearest object marching untill hit
    // @ hit possibly spawns more rays at a depth + 1
    // does stuff to color depending on ray color, object intsected and stuff
    // returns color
 
*/








