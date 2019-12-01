//
//  Box.hpp
//  COMP371_Labs
//
//  Created by Gianluca Lepore on 2019-11-30.
//  Copyright © 2019 Concordia. All rights reserved.
//

#ifndef Box_hpp
#define Box_hpp

#include <stdio.h>
#include <iostream>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices

#include <FreeImageIO.h>
#include <glm/gtx/string_cast.hpp>




using namespace std;
using namespace glm;

class Box{
public:
        void UpdateBoxPoints();
    
        Box();
        ~Box();
        void setTransform(mat4);
        void Update(mat4);
        void UpdateCenter();
        vector<vec3> GetBoxPoints();
        mat4 getTransform();
        vec3 getCenter();
    
private:
        vector<vec3> boxPoints; //Cube Points transformed to world space (for one car part component)
        mat4 transform;
        vec3 center;
};



#endif /* Box_hpp */
