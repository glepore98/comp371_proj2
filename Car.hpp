//
//  Car.hpp
//  COMP371_Labs
//
//  Created by Gianluca Lepore on 2019-09-27.
//  Copyright © 2019 Concordia. All rights reserved.
//

//Class CarParts contains all draw functions for the car's parts
#ifndef Car_hpp
#define Car_hpp
#include <string>
#include <stdio.h>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices

#include <FreeImageIO.h>
#include <glm/gtx/string_cast.hpp>


class CarParts{
public:
    CarParts();
    void DrawBody(int);
    void DrawBonnet(int);
    void DrawWheels(int, float);
    void DrawRoof(int);
    void DrawTrunk(int);
    void DrawWheels2(int, float);
    void DrawGrid_x(int);
    void DrawGrid_z(int);
    void SetPositions(int, int, int);
    int get_xPos();
    int get_yPos();
    int get_zPos();
    void RenderMode();
    void SetScale(float);
    float get_Scale();
    void SetOrientationAngle(float);
    float get_Orientation();
    void DrawGrid(int);
    void DrawGrid2(int);
    void SetTurnAngle(float);
    float get_Turn();
    //void getDirection(glm::vec2);
    void DrawHeadLights(int);
    //------
    glm::vec3 GetRightHeadLight();
    glm::vec3 GetLeftHeadLight();
    glm::vec3 GetRightTailight();
    glm::vec3 GetLeftTailight();
    //--------
    glm::vec3 GetThirdPersonPosition();
    glm::vec3 GetFirstPersonPosition();
    

private:
    int xPos;
    int yPos;
    int zPos;
    float Scale;
    float orientationAngle;
    float turnAngle;
    glm::vec2 direction;
    glm::vec3 front;

};

#endif /* Car_hpp */
