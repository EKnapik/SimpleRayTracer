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
    populateMatrix();
    setupOpenGLCalls();
}

RayTracer::RayTracer(Scene *scene) {
    this->scene = scene;
    setVertexData();
    populateMatrix();
    setupOpenGLCalls();
}

RayTracer::~RayTracer() {
    delete this->pixelData;
}

void RayTracer::raytraceScene(void) {
    populateMatrix();
    
    // CAN DO TONE REPRODUCTION BECAUSE EVERYTHING IS SET
    switch (this->toneModel) {
        case WARD_TONE:
            wardToneModel();
            break;
        case REINHARD_TONE:
            reinhardToneModel();
            break;
        default:
            break;
    }
    
    sendTexture(); // Every time the scene changes the texture must be resent
    renderToWindow();
}

void RayTracer::changeScene(Scene *newScene) {
    this->scene = newScene;
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
    Geometric *objHit = scene->kdTreeCast(ray);
    glm::vec3 posHit = ray->pos + (objHit->timeHit*ray->dir);
    if(objHit->timeHit < 0) {
        return objHit->getColor(posHit);
    }
    glm::vec3 nor = objHit->getNormal(posHit);
    // Local Illumination
    glm::vec3 returnColor = phongShading(ray, objHit);
    // Global Illumination
    if(objHit->reflective) {
        glm::vec3 reflectEye = glm::reflect(ray->dir, nor); // rayDir is the eye to position
        glm::vec3 posShadow = ray->pos + (float(objHit->timeHit-0.001)*ray->dir);
        Ray *reflectiveRay = new Ray(posShadow, reflectEye);
        glm::vec3 reflectColor = illuminate(reflectiveRay, depth-1);
        delete reflectiveRay;
        if(reflectColor.x >= 0) {
            returnColor += objHit->kR*reflectColor; // some do this instead of mix
        }
    }
    if(objHit->transmitive) {
        glm::vec3 posIn = ray->pos + (float(objHit->timeHit+0.001)*ray->dir);
        float hitRef = objHit->refractIndex;
        if(ray->inside) {
            hitRef = 1.0;
            nor = -nor;
        }
        
        float n_it = ray->curRefIndex / hitRef;
        float radicand = 1 + (n_it*n_it*(pow(glm::dot(-ray->dir, nor), 2) - 1));
        // total internal reflection check
        if(radicand < 0) {
            // Total internal reflection occured
            glm::vec3 reflectEye = glm::reflect(glm::normalize(ray->dir), nor); // rayDir is the eye to position
            glm::vec3 posShadow = ray->pos + (float(objHit->timeHit-0.001)*ray->dir);
            Ray *reflectiveRay = new Ray(posShadow, reflectEye);
            glm::vec3 reflectColor = illuminate(reflectiveRay, depth-1);
            delete reflectiveRay;
            if(reflectColor.x >= 0) {
                returnColor += objHit->kT*reflectColor; // some do this instead of mix
            }
        } else {
            // Total internal relfection didn't happen. Transmit ray normaly
            glm::vec3 transDir = n_it*ray->dir + (((n_it*glm::dot(-ray->dir, nor)) - float(sqrt(radicand)))*nor);
            transDir = glm::normalize(transDir);
            Ray *transRay = new Ray(posIn, transDir);
            if(!ray->inside) {
                transRay->inside = true;
            }
            transRay->curRefIndex = hitRef;
            glm::vec3 transColor = illuminate(transRay, depth-1);
            delete transRay;
            if(transColor.x >= 0) {
                returnColor += objHit->kT*transColor;
            }
        }
    }
    

    return returnColor;
}



glm::vec3 RayTracer::phongShading(Ray *inRay, Geometric *objHit) {
    glm::vec3 posHit = inRay->pos + (objHit->timeHit*inRay->dir);
    if(objHit->timeHit < 0) {
        return objHit->getColor(posHit);
    }
    glm::vec3 nor = objHit->getNormal(posHit);
    glm::vec3 posShadow = inRay->pos + (float(objHit->timeHit-0.001)*inRay->dir);
    glm::vec3 reflectEye = glm::reflect(glm::normalize(inRay->dir), nor); // rayDir is the eye to position
    glm::vec3 lightDir = normalize(scene->light->pos - posHit);
    glm::vec3 material = objHit->getColor(posHit);
    
    Ray *shadowRay = new Ray(posShadow, lightDir);
    Geometric *shadowObj = scene->kdTreeCast(shadowRay);
    float spec, diff, shadow;
    glm::vec3 amb;
    glm::vec3 returnColor;
    float hitRef = objHit->refractIndex;
    if(inRay->inside) {
        hitRef = 1.0;
        nor = -nor;
    }
    
    // These should be assigned per object
    shadow = shadowObj->timeHit;
    delete shadowRay;
    if(shadowObj->transmitive) {
        shadow = 0.7;
    } else if(shadow > 0.0) {
        shadow = 0.1;
    } else {
        shadow = 1.0;
    }
    amb = objHit->ambCoeff*glm::vec3(1.0, 1.0, 1.0);
    diff = shadow*objHit->diffCoeff*myClamp(glm::dot(nor,lightDir), 0.0, 1.0);
    spec = shadow*objHit->specCoeff*glm::pow(myClamp(glm::dot(reflectEye,lightDir), 0.0, 1.0), objHit->specExp);
    returnColor = scene->light->color*material*(diff+spec);
    returnColor += amb;
    
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
            setColor(this, row, col);
        }
    }
}

// Using Threads
// simple semi paralleled design
// not optimal
void RayTracer::parallelPopulateMatrix(void) {
    int numThreads = 6;
    std::thread* threads = new std::thread[numThreads];
    
    
    int i = 0;
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            threads[i] = std::thread(setColor, this, row, col);
            i++;
            if(i >= numThreads) {
                for(i = 0; i < numThreads; i++) {
                    threads[i].join();
                }
                i = 0;
            }
        }
    }
    i--;
    for(; i > 0; i--) {
        threads[i].join();
    }
    
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
void setColor(RayTracer* rayTracer, int row, int col) {
    int dataOffset = (row * (4*rayTracer->width)) + (col * 4); // start of wher the color data should go
    int rowPrime, colPrime, rowEnd, colEnd, widthPrime, heightPrime;
    glm::vec4 totalColor = glm::vec4(0.0);
    glm::vec4 color = glm::vec4(0.0);
    Ray *ray = new Ray();
    widthPrime = rayTracer->width * rayTracer->samplingLevel;
    heightPrime = rayTracer->height * rayTracer->samplingLevel;
    ray->pos = rayTracer->scene->camera->getRayPos();
    
    rowPrime = row * rayTracer->samplingLevel;
    rowEnd = rowPrime + rayTracer->samplingLevel;
    for(;rowPrime < rowEnd; rowPrime++) {
        colPrime = col * rayTracer->samplingLevel;
        colEnd = colPrime + rayTracer->samplingLevel;
        for(;colPrime < colEnd; colPrime++) {
            // ray dir is normalized
            ray->dir = rayTracer->scene->camera->getRayDir(rowPrime, colPrime, heightPrime, widthPrime);
            color = glm::vec4(rayTracer->illuminate(ray, rayTracer->rayDepthLevel), 1.0);
            
            // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
    
    totalColor = totalColor / float(rayTracer->samplingLevel * rayTracer->samplingLevel);
    // color values are currently 0.0 -> 1.0 need to transform them
    // set the values
    rayTracer->pixelData[dataOffset] = (totalColor.x * 255);
    rayTracer->pixelData[dataOffset+1] = (totalColor.y * 255);
    rayTracer->pixelData[dataOffset+2] = (totalColor.z * 255);
    rayTracer->pixelData[dataOffset+3] = (totalColor.w * 255);
    
    delete ray;
}




// HEURISTIC calculation for the luminacne at a particular
// pixel given the sRGB values at that pixel
// the 'magic'  numbers associated with calculating the red, gree, and blue
// contribution are based on the human visual perception of how much we precieve
// color
float RayTracer::luminanceAt(int row, int col) {
    int dataOffset = (row * (4*this->width)) + (col * 4); // start of where the color is
    float red = float(this->pixelData[dataOffset]) / 255.0 * 0.27;
    float green = float(this->pixelData[dataOffset+1]) / 255.0 * 0.67;
    float blue = float(this->pixelData[dataOffset+2]) / 255.0 * 0.06;
    float luminance = red + green + blue;
    
    return luminance;
}


float RayTracer::logAvgLuminance() {
    float total = 0.0;
    const float epsilon = 0.0001; // prevents ln(0)
    // Sum up the ln(luminance) of each pixel
    for(int row = 0; row < this->height; row++) {
        for(int col = 0; col < this->width; col++) {
            total += ln(epsilon + luminanceAt(row, col));
        }
    }
    // divide by number of pixels
    total = total / float(this->width*this->height);
    // take the e^(summed average)
    total = powf(E_CONST, total);
    
    return total;
}

// Ward tone reproduction
// uses sf - the scale factor
void RayTracer::wardToneModel() {
    float logAvg = logAvgLuminance();
    float top = (1.219 + powf((this->L_dmax/2.0), 0.4));
    float bot = (1.219 + powf(logAvg, 0.4));
    float sf = powf((top/bot), 2.5);
    printf("sf: %.2f\n", sf);
    // update each value
    // colorNew = color * scaleFactor / L_dmax
    int dataOffset;
    float tmp;
    for(int row = 0; row < this->height; row++) {
        for(int col = 0; col < this->width; col++) {
            dataOffset = (row * (4*this->width)) + (col * 4);
            for(int i = 0; i < 3; i++) {
                tmp = float(this->pixelData[dataOffset+i]) * sf / L_dmax;
                if(tmp > 255) {
                    tmp = 255;
                }
                this->pixelData[dataOffset+i] = tmp;
            }
        }
    }
}


// Reinhard Tone reproduction
void RayTracer::reinhardToneModel() {
    const float a = 0.18;
    float logAvg = logAvgLuminance();
    int dataOffset;
    float rScaled;
    float gScaled;
    float bScaled;
    
    float rTarget;
    float gTarget;
    float bTarget;
    
    // I pull the rgb out of 0-255 and transform them to 0-1.0 then
    // put them back into 0-255 for the pixel data
    for(int row = 0; row < this->height; row++) {
        for(int col = 0; col < this->width; col++) {
            dataOffset = (row * (4*this->width)) + (col * 4);
            rScaled = float(this->pixelData[dataOffset]) / 255.0 * a / logAvg;
            gScaled = float(this->pixelData[dataOffset+1]) / 255.0 * a / logAvg;
            bScaled = float(this->pixelData[dataOffset+2]) / 255.0 * a / logAvg;

            rTarget = rScaled / (1+rScaled) * 255;
            gTarget = gScaled / (1+gScaled) * 255;
            bTarget = bScaled / (1+bScaled) * 255;
            
            if(rTarget > 255) {
                rTarget = 255;
            }
            if(gTarget > 255) {
                gTarget = 255;
            }
            if(bTarget > 255) {
                bTarget = 255;
            }
            
            this->pixelData[dataOffset] = rTarget;
            this->pixelData[dataOffset+1] = gTarget;
            this->pixelData[dataOffset+2] = bTarget;
        }
    }
}




