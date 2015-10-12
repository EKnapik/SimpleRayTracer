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

/*
 This draws two triangles over the entire rendering scene then given a scene
 and a window size and width or the window itself. Will create a texture that
 will be draw on those two triangles
 */

class RayTacer {
public:
    void render(void);
private:
    const GLuint numVerts;
    const GLfloat screneData[30];
};


#endif /* RayTracer_hpp */
