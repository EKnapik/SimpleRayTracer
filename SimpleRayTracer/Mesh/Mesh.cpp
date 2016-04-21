//
//  Mesh.cpp
//  SimpleRayTracer
//
//  Created by Eric Knapik on 3/9/16.
//  Copyright © 2016 EKnapik. All rights reserved.
//

#include "Mesh.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.hpp"

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
    this->pos = glm::vec3(0.0, 0.0, 0.0);
    this->numTriangles = 0;
    this->triangles = (Triangle**) malloc(this->numTriangles*sizeof(Triangle*));
    
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    
    std::string err;
    bool ret = tinyobj::LoadObj(shapes, materials, err, fileName.c_str());
    
    if (!err.empty()) { // `err` may contain warning message.
        std::cerr << err << std::endl;
    }
    
    if (!ret) {
        exit(1);
    }
    
    int index;
    for (size_t i = 0; i < shapes.size(); i++) {
        index = 0;
        assert((shapes[i].mesh.indices.size() % 3) == 0);
        assert((shapes[i].mesh.positions.size() % 3) == 0);
        while(index < shapes[i].mesh.indices.size()) {
            int v1_index, v2_index, v3_index;
            v1_index = shapes[i].mesh.indices[index];
            index++;
            v2_index = shapes[i].mesh.indices[index];
            index++;
            v3_index = shapes[i].mesh.indices[index];
            index++;
            
            /*
            printf("Triangle: (%.2f, %.2f, %.2f), (%.2f, %.2f, %.2f), (%.2f, %.2f, %.2f)\n",
                   shapes[i].mesh.positions[3*v1_index+0],
                   shapes[i].mesh.positions[3*v1_index+1],
                   shapes[i].mesh.positions[3*v1_index+2],
                   
                   shapes[i].mesh.positions[3*v2_index+0],
                   shapes[i].mesh.positions[3*v2_index+1],
                   shapes[i].mesh.positions[3*v2_index+2],
                   
                   shapes[i].mesh.positions[3*v3_index+0],
                   shapes[i].mesh.positions[3*v3_index+1],
                   shapes[i].mesh.positions[3*v3_index+2]);
            */
            this->numTriangles++;
            // reallocing every time and not checking the pointer good stuff.....
            this->triangles = (Triangle**) realloc(this->triangles, this->numTriangles*sizeof(Triangle*));
            this->triangles[numTriangles-1] = new Triangle(
                glm::vec3(shapes[i].mesh.positions[3*v1_index+0], shapes[i].mesh.positions[3*v1_index+1],
                          shapes[i].mesh.positions[3*v1_index+2]),
                                                           
                glm::vec3(shapes[i].mesh.positions[3*v2_index+0], shapes[i].mesh.positions[3*v2_index+1],
                          shapes[i].mesh.positions[3*v2_index+2]),
                                                           
                glm::vec3(shapes[i].mesh.positions[3*v3_index+0], shapes[i].mesh.positions[3*v3_index+1],
                          shapes[i].mesh.positions[3*v3_index+2]));
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

