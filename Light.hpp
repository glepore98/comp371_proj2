//
//  Light.hpp
//  COMP371_Labs
//
//  Created by Gianluca Lepore on 2019-10-16.
//  Copyright © 2019 Concordia. All rights reserved.
//

#ifndef Light_hpp
#define Light_hpp
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices

#include <FreeImageIO.h>
#include <stdio.h>


#endif /* Light_hpp */

using namespace glm;

class Light
{
public:
    void SetObjectColor(int,vec3);
    void SetLampColor(int, vec3);
    void SetLampPos(int,vec3);
    void SetCameraPos(int, vec3);
    void SetLampAmbience(int,vec3);
    void SetLampDiffuse(int,vec3);
    void SetLampSpecular(int,vec3);
    void DrawLight(int);
    
    void SetLeftColor(int, vec3);
    void SetLeftPos(int,vec3);
    void SetLeftAmbience(int,vec3);
    void SetLeftDiffuse(int,vec3);
    void SetLeftSpecular(int,vec3);
    
    void SetRightColor(int, vec3);
    void SetRightPos(int,vec3);
    void SetRightAmbience(int,vec3);
    void SetRightDiffuse(int,vec3);
    void SetRightSpecular(int,vec3);
    
    void SetLeftTailColor(int, vec3);
    void SetLeftTailPos(int,vec3);
    void SetLeftTailAmbience(int,vec3);
    void SetLeftTailDiffuse(int,vec3);
    void SetLeftTailSpecular(int,vec3);
    
    void SetRightTailColor(int, vec3);
    void SetRightTailPos(int,vec3);
    void SetRightTailAmbience(int,vec3);
    void SetRightTailDiffuse(int,vec3);
    void SetRightTailSpecular(int,vec3);
    
};
    
    
    
