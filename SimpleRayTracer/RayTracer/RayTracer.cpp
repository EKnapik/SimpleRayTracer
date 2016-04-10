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
    // Set Defaults
    this->samplingLevel = 1;
    this->width = 800;
    this->height = 450;
	this->scene = scene;
	this->window = window;
    setVertexData();
    populateMatrix();
    setupOpenGLCalls();
}

RayTracer::RayTracer(Scene *scene) {
    // Set Defaults
    this->samplingLevel = 1;
    this->width = 800;
    this->height = 450;
    this->scene = scene;
    setVertexData();
    populateMatrix();
    setupOpenGLCalls();
}

// Default test scene
// Default ray tracer
RayTracer::RayTracer() {
    // Set Defaults
    this->samplingLevel = 1;
    this->width = 800;
    this->height = 450;
    this->scene = new Scene(); // loads the default scene
    setVertexData();
    populateMatrix();
	setupOpenGLCalls();
}

void RayTracer::raytraceScene(void) {
    populateMatrix();
    renderToWindow();
    sendTexture(); // Every time the scene changes the texture must be resent
}

void RayTracer::changeScene(Scene *newScene) {
    this->scene = newScene;
}


// Uses the height and width of the RayTracer to determine the pixelData offset
// to set the approprate values.
// Values are determined by using the current scene to create rays shooting them
// into the scene with a variable bounce depth
// Supersampling can be done here by treating the pixel grid as if it were
// doubled or tripled or more then there are 4, 9 or more rays shot per pixel
// this adds computation time but no greater memory impact
// SAMPLING:
//  Supersampling can be done as part of the ray tracer. THe defualt is sampling
//  level of 1 but that can be increased. The sampling is uniform super sampling
//  where the amount of rays per pixel is the sampling level squared.
void RayTracer::setColor(int row, int col) {
    int depth = 4; // THE DEPTH OF RAY RECURSION
    
    int dataOffset = (row * (4*this->width)) + (col * 4); // start of wher the color data should go
    int rowPrime, colPrime, rowEnd, colEnd, widthPrime, heightPrime;
    glm::vec4 totalColor = glm::vec4(0.0);
    glm::vec4 color = glm::vec4(0.0);
    Ray *ray = new Ray();
    widthPrime = this->width * this->samplingLevel;
    heightPrime = this->height * this->samplingLevel;
    ray->pos = scene->camera->getRayPos();
    
    rowPrime = row * this->samplingLevel;
    rowEnd = rowPrime + this->samplingLevel;
    for(;rowPrime < rowEnd; rowPrime++) {
        colPrime = col * this->samplingLevel;
        colEnd = colPrime + this->samplingLevel;
        for(;colPrime < colEnd; colPrime++) {
            ray->dir = scene->camera->getRayDir(rowPrime, colPrime, heightPrime, widthPrime);
            color = glm::vec4(illuminate(ray, depth), 1.0);
            // Color values may have been returned as greater than 1.0;
            if(color.x > 1.0) {
                color.x = 1.0;
            }
            if(color.y > 1.0) {
                color.y = 1.0;
            }
            if(color.z > 1.0) {
                color.z = 1.0;
            }
            totalColor += color;
        }
    }
    
    totalColor = totalColor / float(this->samplingLevel * this->samplingLevel);
    // color values are currently 0.0 -> 1.0 need to transform them
    // set the values
    pixelData[dataOffset] = (totalColor.x * 255);
    pixelData[dataOffset+1] = (totalColor.y * 255);
    pixelData[dataOffset+2] = (totalColor.z * 255);
    pixelData[dataOffset+3] = (totalColor.w * 255);
    
    delete ray;
}


float myClamp(float value, float min, float max) {
    if(value < min) {
        return min;
    }
    if(value > max) {
        return max;
    }
    return value;
}


/*
    Return Color is:
    the local illumination * Shadow test
    if(reflect):
        returnColor += illuminate(reflectRay) * kR
    if(trans):
        returnColor += illuminate(transRay) * kT
 */
glm::vec3 RayTracer::illuminate(Ray *ray, int depth) {
    if (depth <= 0) {
        return glm::vec3(-1.0);
    }
    Geometric *objHit = scene->intersectCast(ray);
    glm::vec3 posHit = ray->pos + (objHit->timeHit*ray->dir);
    if(objHit->timeHit < 0) {
        return objHit->getColor(posHit);
    }
    glm::vec3 nor = objHit->getNormal(posHit);
    glm::vec3 posShadow = ray->pos + (float(objHit->timeHit-0.001)*ray->dir);
    glm::vec3 reflectEye = glm::reflect(glm::normalize(ray->dir), nor); // rayDir is the eye to position
    glm::vec3 lightDir = normalize(scene->light->pos - posHit);
    glm::vec3 material = objHit->getColor(posHit);
    
    Ray *shadowRay = new Ray(posShadow, lightDir);
    Geometric *shadowObj = scene->intersectCast(shadowRay);
    float specCoeff, diffCoeff, ambCoeff;
    float spec, diff, shadow;
    glm::vec3 amb;
    glm::vec3 returnColor;
    float hitRef = objHit->refractIndex;
    if(ray->inside) {
        hitRef = 1.0;
        nor = -nor;
    }
    
    ambCoeff = scene->ambientCoeff; // scene property
    // These should be assigned per object
    diffCoeff = objHit->diffCoeff;
    specCoeff = objHit->specCoeff;
    shadow = shadowObj->timeHit;
    delete shadowRay;
    if(shadowObj->transmitive) {
        shadow = 0.75;
    } else if(shadow > 0.0) {
        shadow = 0.1;
    } else {
        shadow = 1.0;
    }
    amb = ambCoeff*glm::vec3(1.0, 1.0, 1.0);
    diff = shadow*diffCoeff*myClamp(glm::dot(nor,lightDir), 0.0, 1.0);
    spec = shadow*specCoeff*glm::pow(myClamp(glm::dot(reflectEye,lightDir), 0.0, 1.0), 20.0);
    returnColor = scene->light->color*material*(diff+spec);
    returnColor += amb;
    
    
    if(objHit->reflective) {
        Ray *reflectiveRay = new Ray(posShadow, reflectEye);
        glm::vec3 reflectColor = illuminate(reflectiveRay, depth-1);
        delete reflectiveRay;
        if(reflectColor.x >= 0) {
            returnColor += float(0.5)*reflectColor; // some do this instead of mix
        }
    }
    if(objHit->transmitive) {
        glm::vec3 posIn = ray->pos + (float(objHit->timeHit+0.001)*ray->dir);
        
        
        // total internal refraction check
        if(pow(glm::dot(nor, ray->dir),2) > 1-(pow(hitRef/ray->curRefIndex, 2))) {
            glm::vec3 transDir;
            float nRatio = ray->curRefIndex / hitRef;
            float sqrtVal = sqrt(1-(nRatio*nRatio*(1-pow(glm::dot(nor, ray->dir),2))));
            float scale = (nRatio*glm::dot(nor, ray->dir)) - sqrtVal;
            transDir = scale*nor - nRatio*ray->dir;
            transDir = glm::normalize(transDir);
            
            transDir = glm::refract(ray->dir, nor, ray->curRefIndex/hitRef);
            
            Ray *transRay = new Ray(posIn, transDir);
            //****************************************
             if(!ray->inside) {
             transRay->inside = true;
             }
            //************************************/
            transRay->curRefIndex = hitRef;
            glm::vec3 transColor = illuminate(transRay, depth-1);
            delete transRay;
            if(transColor.x >= 0) {
                // returnColor += float(0.8)*transColor;
                returnColor = mix(returnColor, transColor, 0.95);
            }
        }
    }
    

    return returnColor;
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
		exit(1);
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






