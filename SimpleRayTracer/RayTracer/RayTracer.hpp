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
#include "shaderSetup.h"

/*
 This draws two triangles over the entire rendering scene then given a scene
 and a window size and width or the window itself. Will create a texture that
 will be draw on those two triangles
 */

class RayTracer {
public:
    RayTracer(Scene *scenePtr, Window *windowPtr);
    void raytraceScene(void);
    void changeScene(Scene *newScene);
    
private:
    virtual void setColor(int x, int y); // <- that is run per pixel updating the 3D matrix
    void setupOpenGLCalls(void);
    void renderToWindow(void); // renders the current 3D matrix to the window
    void populateMatrix(void);
    void setupThreads(void);
    void shutdownThreads(void);
    
    // render 2 triangles and texture
    GLuint shaderProgram;
    GLuint vBuffer, eBuffer;
    GLuint vertPos, uvPos, texPos;
    const GLuint numVerts;
    const GLfloat vertexData[20]; // set in construction
    const GLushort elementData[6];
    // some 3D byte array for the texture data
    
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
    
    
RayTracer(Scene *scenePtr, Window *windowPtr) {
	* set values to this->scenePtr = scenePtr and window
	virtual void function that is run per pixel ... This is virtual so that another class could extend this and implement its own
		per pixel thing.
	dynamically create 3D array with window info

	Setup the OpenGL stuff
		compile and link simple shader
		vertex buffer
		texture buffer
}

public:
RayTracer(Scene *scenePtr, Window *windowPtr);
void raytraceScene()
void changeScene(Scene *newScene);

private:
GLuint shaderProgram

virtual void setColor(x, y); // <- that is run per pixel updating the 3D matrix
void setupOpenGLCalls()
void populateMatrix()
void renderToWindow()
void setupThreads()
void shutdownThreads()

setup texturegl() {
	GLuint texBufferID;

	char *image = //image data
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, texBufferID);
	glBindTexture(GL_TEXTURE_2D, texBufferID);
	glTexImage2D(GL_TEXTURE_2D .......) // passing the image data down

	// set texture parameters... To be honest none of these need to be set
	// because this is a ray tracer not a normal opengl texture and this will
	// always be the exact screne pixels
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//UV coordinate passing like in the vertex or normal passing

	// setup the texture variable uniform location to the texture loaded into GL_TEXTURE0
	// make sure glActiveTexture(GL_TEXTURE0)
	texID = glGetUniformLocation(program, "texture");
	glUniform1i(texID, 0); // the 0 refers to GL_TEXTURE0

}
    
*/
