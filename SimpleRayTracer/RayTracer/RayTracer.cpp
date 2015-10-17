//
//  RayTracer.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/11/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#include "RayTracer.hpp"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

//RayTracer::RayTracer(Scene *scene, Window *window) {
//	this.scene = scene;
//	this.window = window;
RayTracer::RayTracer() {
    setVertexData();
	setupOpenGLCalls();
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
    
    // Set the texture data should be a 4 squares
    pixelData[0] = 255;
    pixelData[1] = 0;
    pixelData[2] = 0;
    
    pixelData[3] = 0;
    pixelData[4] = 255;
    pixelData[5] = 0;
    
    pixelData[6] = 0;
    pixelData[7] = 0;
    pixelData[8] = 255;
    
    pixelData[9] = 255;
    pixelData[10] = 255;
    pixelData[11] = 255;
}

void RayTracer::setupOpenGLCalls(void) {
	int dataSize = numVerts * 4 * sizeof(GLfloat);

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData);
	// glTexImage(GL_TEXTURE_2D, ......) // pass the 3D texture data to gpu

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Create shader program and find variable locations in shader program
	shaderProgram = shaderSetup("RayTracer.vert", "RayTracer.frag");
	if( shaderProgram ) {
		perror("ERROR SETTING UP SHADERS!!!!\n");
		_exit(1);
	}

	vertPos = glGetAttribLocation(shaderProgram, "currVert");
	uvPos = glGetAttribLocation(shaderProgram, "uvVert");
	texPos = glGetUniformLocation(shaderProgram, "texture");
}

void RayTracer::renderToWindow(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eBuffer);
	glBindTexture(GL_TEXTURE_2D, texBuffer);
	glUseProgram(shaderProgram);

	// pass the vertex data info
	int step = 5*sizeof(GLfloat);
    
	glEnableVertexAttribArray(vertPos);
	glVertexAttribPointer(vertPos, 3, GL_FLOAT, GL_FALSE, step, 0);

	glEnableVertexAttribArray(uvPos);
	glVertexAttribPointer(uvPos, 2, GL_FLOAT, GL_FALSE, step, BUFFER_OFFSET(3*sizeof(GLfloat)));

	glUniform1i(texPos, 0); // GL_TEXTURE0

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, elementData);
}


void RayTracer::raytraceScene(void) {
	renderToWindow();
}


/*
void RayTracer::changeScene(Scene *newScene) {
	this.scene = newScene;
}

void RayTracer::sendTexture(void) {
	// glTexImage(GL_TEXTURE_2D, ...)
}
*/










