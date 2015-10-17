//
//  RayTracer.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 10/11/15.
//  Copyright © 2015 EKnapik. All rights reserved.
//

#include "RayTracer.hpp"

//RayTracer::RayTracer(Scene *scene, Window *window) {
//	this.scene = scene;
//	this.window = window;
RayTracer::RayTracer() {
	// define the 4 points of square in counter clockwise order
	//			     x,    y,   z,   u,   v
	vertexData = {-1.0,  1.0, 0.0, 0.0, 1.0,
	              -1.0, -1.0, 0.0, 0.0, 0.0,
	               1.0, -1.0, 0.0, 1.0, 0.0,
	               1.0,  1.0, 0.0, 1.0, 1.0};
	elementData = {0, 1, 2, 3, 0, 2};
	numVerts = 4;
	setupOpenGLCalls();
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
	glBindBuffer(GL_ELEEMENT_ARRAY_BUFFER, 0);

	// texture sending
	glGenTextures(1, texBuffer);
	glBindTexture(GL_TEXTURE_2D, texBuffer);
	glActiveTexture(GL_TEXTURE0);
	glTexImage(GL_TEXTURE_2D, ......) // pass the 3D texture data to gpu

	glTexParameteri(GL_TEXTURE2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Create shader program and find variable locations in shader program
	shaderProgram = shaderSetup("RayTracer.vert", "RayTracer.frag");
	if( shaderProgram ) {
		perror("ERROR SETTING UP SHADERS!!!!\n");
		exit(1);
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
	glVertexAttribPointer(vertPos, 3, GL_FLOAT, step, 0);

	glEnableVertexAttribArray(uvPos);
	glVertexAttribPointer(uvPos, 2, GL_FLOAT, step, 3*sizeof(GLfloat));

	glUniform1i(texPos, 0); // GL_TEXTURE0

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, elementData);
	glutSwapBuffers();
}


/*
void RayTracer::changeScene(Scene *newScene) {
	this.scene = newScene;
}

void RayTracer::sendTexture(void) {
	// glTexImage(GL_TEXTURE_2D, ...)
}
*/










