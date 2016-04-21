//
//  Mesh.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 3/9/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//

#include "Mesh.hpp"

Mesh::Mesh() {
    this->pos = glm::vec3(0.0, 0.0, 0.0);
    this->numTriangles = 0;
}


Mesh::Mesh(Triangle **meshTriangles, int numTriangles) {
    this->pos = glm::vec3(0.0, 0.0, 0.0);
    this->triangles = meshTriangles;
    this->numTriangles = numTriangles;
}


/*
 Used for the importing of wavefront .obj files
 */
Mesh::Mesh(std::string fileName) {
    std::ifstream meshFile(fileName);
    std::string line;
    std::string name;
    
    // verticies
    // numVerticies
    // normals
    
    int numVerticies = 0;
    while(std::getline(meshFile, line)){
        if(line == "" || line[0] == '#') // skip comments and blank lines
            continue;
        
        std::istringstream lineStream(line);
        lineStream >> name; // the type of line we are about to read
        
        if(name == "v"){    // Vertex
            numVerticies++;
            float *vertex = new float[3];
            sscanf(line.c_str(), "%*s %f %f %f", &vertex[0], &vertex[1], &vertex[2]);
        }
        
        if(name == "vn"){    // Vertex Normal
            float *vertNorm = new float[3];
            sscanf(line.c_str(), "%*s %f %f %f", &vertNorm[0], &vertNorm[1], &vertNorm[2]);
        }
    }
    
    
}



void Mesh::addTriangle(Triangle *triangle) {
    // expand the pointer
    // add to numTriangles
    Triangle **tmpTriangles = new Triangle *[numTriangles+1];
    for(int i = 0; i < numTriangles; i++) {
        tmpTriangles[i] = this->triangles[i];
    }
    delete [] this->triangles;
    this->triangles = tmpTriangles;
    this->triangles[numTriangles] = triangle;
    this->numTriangles++;
}

// Uses the transVector to create the translation
// matrix then multiplies all triangles by that matrix
void Mesh::moveBy(glm::vec3 transVector) {
    this->pos += transVector;
    glm::mat4 transMatrix = glm::translate(glm::mat4(1.0), transVector);
    glm::vec4 resultVector;
    for(int i = 0; i < this->numTriangles; i++) {
        // for each vector in the triangle
        resultVector = transMatrix * glm::vec4(this->triangles[i]->v1, 1.0);
        this->triangles[i]->v1 = glm::vec3(resultVector);
        
        resultVector = transMatrix * glm::vec4(this->triangles[i]->v2, 1.0);
        this->triangles[i]->v2 = glm::vec3(resultVector);
        
        resultVector = transMatrix * glm::vec4(this->triangles[i]->v3, 1.0);
        this->triangles[i]->v3 = glm::vec3(resultVector);
        
        this->triangles[i]->pos = (this->triangles[i]->v1 + this->triangles[i]->v2 + this->triangles[i]->v3) / float(3.0);
    }
}

// Translates the mesh to new position
// Uses translation matrix on all triangles
void Mesh::moveTo(glm::vec3 newPos) {
    moveBy(newPos-this->pos);
}

// Scales the mesh by scale factor
// Uses scale matrix on all triangles
void Mesh::scale(float scaleFactor) {
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), glm::vec3(scaleFactor));
    glm::vec4 resultVector;
    for(int i = 0; i < this->numTriangles; i++) {
        // for each vector in the triangle
        resultVector = scaleMatrix * glm::vec4(this->triangles[i]->v1, 1.0);
        this->triangles[i]->v1 = glm::vec3(resultVector);
        
        resultVector = scaleMatrix * glm::vec4(this->triangles[i]->v2, 1.0);
        this->triangles[i]->v2 = glm::vec3(resultVector);
        
        resultVector = scaleMatrix * glm::vec4(this->triangles[i]->v3, 1.0);
        this->triangles[i]->v3 = glm::vec3(resultVector);
        
        this->triangles[i]->pos = (this->triangles[i]->v1 + this->triangles[i]->v2 + this->triangles[i]->v3) / float(3.0);
    }
}

// Uses rotation matrix on all triangles
void Mesh::rotateX(float degrees) {
    glm::mat4 rotateXMatrix = glm::rotate(glm::mat4(1.0), degrees, glm::vec3(1.0, 0.0, 0.0));
    glm::vec4 resultVector;
    for(int i = 0; i < this->numTriangles; i++) {
        // for each vector in the triangle
        resultVector = rotateXMatrix * glm::vec4(this->triangles[i]->v1, 1.0);
        this->triangles[i]->v1 = glm::vec3(resultVector);
        
        resultVector = rotateXMatrix * glm::vec4(this->triangles[i]->v2, 1.0);
        this->triangles[i]->v2 = glm::vec3(resultVector);
        
        resultVector = rotateXMatrix * glm::vec4(this->triangles[i]->v3, 1.0);
        this->triangles[i]->v3 = glm::vec3(resultVector);
        
        this->triangles[i]->pos = (this->triangles[i]->v1 + this->triangles[i]->v2 + this->triangles[i]->v3) / float(3.0);
    }
}

// Uses rotation matrix on all triangles
void Mesh::rotateY(float degrees) {
    glm::mat4 rotateYMatrix = glm::rotate(glm::mat4(1.0), degrees, glm::vec3(0.0, 1.0, 0.0));
    glm::vec4 resultVector;
    for(int i = 0; i < this->numTriangles; i++) {
        // for each vector in the triangle
        resultVector = rotateYMatrix * glm::vec4(this->triangles[i]->v1, 1.0);
        this->triangles[i]->v1 = glm::vec3(resultVector);
        
        resultVector = rotateYMatrix * glm::vec4(this->triangles[i]->v2, 1.0);
        this->triangles[i]->v2 = glm::vec3(resultVector);
        
        resultVector = rotateYMatrix * glm::vec4(this->triangles[i]->v3, 1.0);
        this->triangles[i]->v3 = glm::vec3(resultVector);
        
        this->triangles[i]->pos = (this->triangles[i]->v1 + this->triangles[i]->v2 + this->triangles[i]->v3) / float(3.0);
    }
}

// Uses rotation matrix on all triangles
void Mesh::rotateZ(float degrees) {
    glm::mat4 rotateZMatrix = glm::rotate(glm::mat4(1.0), degrees, glm::vec3(1.0, 0.0, 1.0));
    glm::vec4 resultVector;
    for(int i = 0; i < this->numTriangles; i++) {
        // for each vector in the triangle
        resultVector = rotateZMatrix * glm::vec4(this->triangles[i]->v1, 1.0);
        this->triangles[i]->v1 = glm::vec3(resultVector);
        
        resultVector = rotateZMatrix * glm::vec4(this->triangles[i]->v2, 1.0);
        this->triangles[i]->v2 = glm::vec3(resultVector);
        
        resultVector = rotateZMatrix * glm::vec4(this->triangles[i]->v3, 1.0);
        this->triangles[i]->v3 = glm::vec3(resultVector);
        
        this->triangles[i]->pos = (this->triangles[i]->v1 + this->triangles[i]->v2 + this->triangles[i]->v3) / float(3.0);
    }
}

