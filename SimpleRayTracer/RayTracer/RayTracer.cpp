//
//  RayTracer.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/11/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#include "RayTracer.hpp"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

RayTracer::RayTracer(Scene *scene, Window *window) {
    this->width = 800;
    this->height = 450;
	this->scene = scene;
	this->window = window;
    setVertexData();
    populateMatrix();
    setupOpenGLCalls();
}

RayTracer::RayTracer(Scene *scene) {
    this->width = 800;
    this->height = 450;
    this->scene = scene;
    setVertexData();
    populateMatrix();
    setupOpenGLCalls();
}

// Default test scene
RayTracer::RayTracer() {
    this->width = 800;
    this->height = 450;
    Camera *camera = new Camera(glm::vec3(0.0, 1.0, 1.0), glm::vec3(0.0, 1.0, -1.0));
    this->scene = new Scene(camera);
    setVertexData();
    populateMatrix();
	setupOpenGLCalls();
}

void RayTracer::raytraceScene(void) {
    populateMatrix();
    renderToWindow();
    sendTexture();
}

void RayTracer::changeScene(Scene *newScene) {
    this->scene = newScene;
}


// Uses the height and width of the RayTracer to determine the pixelData offset
// to set the approprate values.
// Values are determined by using the current scene to create rays shooting them
// into the scene with a variable bounce depth
void RayTracer::setColor(int row, int col) {
    int dataOffset = (row * (4*this->width)) + (col * 4); // start of wher the color data should go
    Ray *ray = new Ray(scene->camera->getRayPos(), scene->camera->getRayDir(row, col, this->width, this->height));
    
    glm::vec4 color = shootRay(ray, 1.0);
    
    color = glm::vec4(0.0, 1.0, 1.0, 1.0);
    // color = glm::vec4(float(row/height), float(row/height), float(row/height), 1.0);
    // color values are currently 0.0 -> 1.0 need to transform them
    // set the values
    pixelData[dataOffset] = (color.x * 255);
    pixelData[dataOffset+1] = (color.y * 255);
    pixelData[dataOffset+2] = (color.z * 255);
    pixelData[dataOffset+3] = (color.w * 255);
    
    delete ray;
}


glm::vec4 RayTracer::shootRay(Ray *ray, int depth) {
    if (depth <= 0) {
        return glm::vec4(0.0);
    }
    Geometric *objHit;
    
    // do something with that object's color
    // spawn more rays and mix colors
    // return color
    
    return glm::vec4(0.0);
}


void RayTracer::setupOpenGLCalls(void) {
	int dataSize = numVerts * 5 * sizeof(GLfloat);

	// Initialize the vertex and element buffer for later drawing
	glGenBuffers(1, &vBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
	glBufferData(GL_ARRAY_BUFFER, dataSize, vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &eBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLushort), elementData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// texture sending
	glGenTextures(1, &texBuffer);
	glBindTexture(GL_TEXTURE_2D, texBuffer);
	glActiveTexture(GL_TEXTURE0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Create shader program and find variable locations in shader program
	shaderProgram = shaderSetup("RayTracer.vert", "RayTracer.frag");
	if( !shaderProgram ) {
		perror("ERROR SETTING UP SHADERS!!!!\n");
		_exit(1);
	}

	vertPos = glGetAttribLocation(shaderProgram, "currVert");
	uvPos = glGetAttribLocation(shaderProgram, "uvVert");
	texPos = glGetUniformLocation(shaderProgram, "texture");
}

void RayTracer::renderToWindow(void) {
    glUseProgram(shaderProgram);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eBuffer);
	glBindTexture(GL_TEXTURE_2D, texBuffer);

	// pass the vertex data info
	int step = 5*sizeof(GLfloat);
	glEnableVertexAttribArray(vertPos);
	glVertexAttribPointer(vertPos, 3, GL_FLOAT, GL_FALSE, step, 0);

	glEnableVertexAttribArray(uvPos);
	glVertexAttribPointer(uvPos, 2, GL_FLOAT, GL_FALSE, step, BUFFER_OFFSET(3*sizeof(GLfloat)));

	glUniform1i(texPos, 0); // GL_TEXTURE0

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}


// populates the matrix at the current time in the scene
// optionally could send the setColor function to N threads for the
// matrix population
void RayTracer::populateMatrix(void) {
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            setColor(row, col);
        }
    }
}

// Using Threads
void RayTracer::setupThreads(void) {
    return;
}

void RayTracer::shutdownThreads(void) {
    return;
}


void RayTracer::setVertexData(void) {
    // define the 4 points of square in counter clockwise order
    //			     x,    y,   z,   u,   v
    vertexData[0] = -1.0;
    vertexData[1] = 1.0;
    vertexData[2] = 0.0;
    vertexData[3] = 0.0;
    vertexData[4] = 1.0;
    
    vertexData[5] = -1.0;
    vertexData[6] = -1.0;
    vertexData[7] = 0.0;
    vertexData[8] = 0.0;
    vertexData[9] = 0.0;
    
    vertexData[10] = 1.0;
    vertexData[11] = -1.0;
    vertexData[12] = 0.0;
    vertexData[13] = 1.0;
    vertexData[14] = 0.0;
    
    vertexData[15] = 1.0;
    vertexData[16] = 1.0;
    vertexData[17] = 0.0;
    vertexData[18] = 1.0;
    vertexData[19] = 1.0;
    
    numVerts = 4;
    elementData[0] = 0;
    elementData[1] = 1;
    elementData[2] = 2;
    elementData[3] = 3;
    elementData[4] = 0;
    elementData[5] = 2;
    
    pixelData = new GLubyte[height * width * 4];
}

void RayTracer::sendTexture(void) {
	// texture sending
	glBindTexture(GL_TEXTURE_2D, texBuffer);
	glActiveTexture(GL_TEXTURE0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

/*
 pixelData[0] = 255; // bot left R
 pixelData[1] = 0; // bot left G
 pixelData[2] = 0; // bot left B
 pixelData[3] = 255; // Alpha
 
 pixelData[4] = 0; // bot right R
 pixelData[5] = 255; // bot right G
 pixelData[6] = 0; // bot right B
 pixelData[7] = 255; // Alpha
 
 pixelData[8] = 255; // top left R
 pixelData[9] = 0; // top left G
 pixelData[10] = 255; // top left B
 pixelData[11] = 255; // Alpha
 
 pixelData[12] = 0; // top right R
 pixelData[13] = 0; // top right G
 pixelData[14] = 255; // top right B
 pixelData[15] = 255; // Alpha
 */









