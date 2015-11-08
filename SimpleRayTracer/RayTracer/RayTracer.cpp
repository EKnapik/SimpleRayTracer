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
	this->scene = scene;
	this->window = window;
    setVertexData();
    setupOpenGLCalls();
}

RayTracer::RayTracer(Scene *scene) {
    this->scene = scene;
    setVertexData();
    setupOpenGLCalls();
}

RayTracer::RayTracer() {
    setVertexData();
	setupOpenGLCalls();
}

void RayTracer::raytraceScene(void) {
    renderToWindow();
    /*
     setup
     populate matrix
     shudown
     renderToWindow()
     */
}

void RayTracer::changeScene(Scene *newScene) {
    this->scene = newScene;
}

// relies on height and width
// could just write shaders for this function
// edits pixelData
void RayTracer::setColor(int row, int col) {
    // usig to camera and matrix index create a ray and get the color
    // then set the color values appropriately in the matrix
    // mkray
    // color = shoot ray
    int dataOffset = (row * (4*width)) + (col * 4); // start of wher the color data should go
    // do something to get the 4 color data values
    // set the values
    pixelData[dataOffset] = 0;
    pixelData[dataOffset+1] = 0;
    pixelData[dataOffset+2] = 0;
    pixelData[dataOffset+3] = 0;
    
    
}


Vec4 RayTracer::shootRay(Ray ray, int depth) {
    if (depth <= 0) {
        return Vec4();
    }
    
    
    return Vec4();
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
	glClear(GL_COLOR_BUFFER_BIT);
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
    vertexData[4] = 0.0;
    
    vertexData[5] = -1.0;
    vertexData[6] = -1.0;
    vertexData[7] = 0.0;
    vertexData[8] = 0.0;
    vertexData[9] = 1.0;
    
    vertexData[10] = 1.0;
    vertexData[11] = -1.0;
    vertexData[12] = 0.0;
    vertexData[13] = 1.0;
    vertexData[14] = 1.0;
    
    vertexData[15] = 1.0;
    vertexData[16] = 1.0;
    vertexData[17] = 0.0;
    vertexData[18] = 1.0;
    vertexData[19] = 0.0;
    
    numVerts = 4;
    elementData[0] = 0;
    elementData[1] = 1;
    elementData[2] = 2;
    elementData[3] = 3;
    elementData[4] = 0;
    elementData[5] = 2;
    
    // Set the texture data should be a 4 squares
    width = 2;
    height = 2;
    pixelData = new GLubyte[16];
    pixelData[0] = 255; // top left R
    pixelData[1] = 0; // top left G
    pixelData[2] = 0; // top left B
    pixelData[3] = 255; // Alpha
    
    pixelData[4] = 0; // top right R
    pixelData[5] = 255; // top right G
    pixelData[6] = 0; // top right B
    pixelData[7] = 255; // Alpha
    
    pixelData[8] = 255; // bot left R
    pixelData[9] = 0; // bot left G
    pixelData[10] = 255; // bot left B
    pixelData[11] = 255; // Alpha
    
    pixelData[12] = 0; // bot right R
    pixelData[13] = 0; // bot right G
    pixelData[14] = 255; // bot right B
    pixelData[15] = 255; // Alpha
}

/*
void RayTracer::sendTexture(void) {
	// glTexImage(GL_TEXTURE_2D, ...)
}
*/










