//
//  Box.cpp
//  COMP371_Labs
//
//  Created by Gianluca Lepore on 2019-11-30.
//  Copyright © 2019 Concordia. All rights reserved.
//

#include "Box.hpp"
#include "Car.hpp"
#include <iostream>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <fstream>
#include <iomanip>

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


    Box::~Box(){
    
    }

    Box::Box(){
        for (int i=0; i<8;i++){
            boxPoints.push_back(vec3(0.0f,0.0f,0.0f));
        }
    }

    void Box::UpdateBoxPoints(){
        boxPoints[0] = vec3(transform * vec4(-0.5f, -0.5f, -0.5f, 1.0f));
        boxPoints[1] = vec3(transform * vec4(0.5f, 0.5f, 0.5f, 1.0f));
        boxPoints[2] = vec3(transform * vec4(0.5f, 0.5f, -0.5f, 1.0f));
        boxPoints[3] = vec3(transform * vec4(-0.5f, -0.5f, 0.5f, 1.0f));
        boxPoints[4] = vec3(transform * vec4(0.5f, -0.5f, 0.5f, 1.0f));
        boxPoints[5] = vec3(transform * vec4(-0.5f, 0.5f, -0.5f, 1.0f));
        boxPoints[6] = vec3(transform * vec4(0.5f, -0.5f, -0.5f, 1.0f));
        boxPoints[7] = vec3(transform * vec4(-0.5f, 0.5f, 0.5f, 1.0f));
        
    }

    void Box::setTransform(mat4 trans){
        transform = trans;
    }

    void Box::UpdateCenter(){
        center = vec3(transform * vec4(0.0f, 0.0f, 0.0f, 1.0f));
    }

    vec3 Box::getCenter(){
        return center;
    }

    void Box::Update(mat4 UpdatedTransform){
        transform = UpdatedTransform;
        UpdateBoxPoints();
        UpdateCenter();
    }

    
    vector<vec3> Box::GetBoxPoints(){
        return boxPoints;
    }

    mat4 Box::getTransform(){
        return transform;
    }
