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
#include <thread>
#include <mutex>
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
    ~RayTracer();
    void raytraceScene(void);
    void changeScene(Scene *newScene);
    Scene *scene;
    Window *window;
    
    // Rendering Flags
    int samplingLevel = 1;
    int rayDepthLevel = 8; // THE DEPTH OF RAY RECURSION
    
     // width and height will be covered by the window when that is fixed
    int height = 450;
    int width = 800;
    
    glm::vec3 illuminate(Ray *ray, int depth);
    glm::vec3 phongShading(Ray *inRay, Geometric *objHit);
    void setupOpenGLCalls(void);
    void renderToWindow(void); // renders the current 3D matrix to the window
    void populateMatrix(void);
    void parallelPopulateMatrix(void);
    void setVertexData(void);
    void sendTexture(void);
    // some 3D byte array for the texture data
    GLubyte *pixelData;
    
    private:
    
    // render 2 triangles and texture
    GLuint shaderProgram;
    GLuint vBuffer, eBuffer, texBuffer;
    GLuint vertPos, uvPos, texPos;
    GLuint numVerts;
    GLfloat vertexData[20]; // set in construction
    GLushort elementData[6];
};
#endif /* RayTracer_hpp */

void setColor(RayTracer* rayTracer, int row, int col); // <- that is run per pixel updating the 3D matrix








