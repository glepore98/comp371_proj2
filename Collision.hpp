//
//  Collision.hpp
//  COMP371_Labs
//
//  Created by Gianluca Lepore on 2019-11-30.
//  Copyright © 2019 Concordia. All rights reserved.
//

#ifndef Collision_hpp
#define Collision_hpp

#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include "Car.hpp"
#include "Box.hpp"


#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices

using namespace glm;

class Collision{
    public:
        Collision();
        void CheckCollision(CarParts&, CarParts&);
        vec3 GetCollision(Box& , Box&);
private:
        vector<CarParts> x;
};



#endif /* Collision_hpp */
