//
//  Light.cpp
//  COMP371_Labs
//
//  Created by Gianluca Lepore on 2019-10-16.
//  Copyright © 2019 Concordia. All rights reserved.
//

#include "Light.hpp"

#include <iostream>
#include <cstdlib>
#include <string>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices

using namespace glm;

void Light::DrawLight(int lightshader){
    glm::mat4 scalingMatrix =  glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.5f));
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,30.0f,0.0f));
    glm::mat4 worldMatrix = translationMatrix * scalingMatrix;
    GLuint worldMatrixLocation = glGetUniformLocation(lightshader, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
};

void Light::SetObjectColor(int shader, vec3 objectColor){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "objectColor");
    glUniform3fv(vecLocation, 1, &objectColor[0]);
}

void Light::SetCameraPos(int shader, vec3 cameraPos){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "cameraPos");
    glUniform3fv(vecLocation, 1, &cameraPos[0]);
}

//MAKE SURE setting POS,anb for pointlight[i]

//Sets Light struct variables---------------------------------

//Lamps variables
void Light::SetLampAmbience(int shader, vec3 ambience){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[0].ambience");
    glUniform3fv(vecLocation, 1, &ambience[0]);
}

void Light::SetLampDiffuse(int shader, vec3 diffuse){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[0].diffuse");
    glUniform3fv(vecLocation, 1, &diffuse[0]);
}

void Light::SetLampSpecular(int shader, vec3 specular){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[0].specular");
    glUniform3fv(vecLocation, 1, &specular[0]);
}

void Light::SetLampColor(int shader, vec3 lightColor){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[0].lightColor");
    glUniform3fv(vecLocation, 1, &lightColor[0]);
}

void Light::SetLampPos(int shader, vec3 lightPos){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[0].lightPos");
    glUniform3fv(vecLocation, 1, &lightPos[0]);
}

//Left headlight--------
void Light::SetLeftAmbience(int shader, vec3 ambience){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[1].ambience");
    glUniform3fv(vecLocation, 1, &ambience[0]);
}

void Light::SetLeftDiffuse(int shader, vec3 diffuse){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[1].diffuse");
    glUniform3fv(vecLocation, 1, &diffuse[0]);
}

void Light::SetLeftSpecular(int shader, vec3 specular){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[1].specular");
    glUniform3fv(vecLocation, 1, &specular[0]);
}

void Light::SetLeftColor(int shader, vec3 lightColor){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[1].lightColor");
    glUniform3fv(vecLocation, 1, &lightColor[0]);
}

void Light::SetLeftPos(int shader, vec3 lightPos){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[1].lightPos");
    glUniform3fv(vecLocation, 1, &lightPos[0]);
}

//Right headlight------------
void Light::SetRightAmbience(int shader, vec3 ambience){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[2].ambience");
    glUniform3fv(vecLocation, 1, &ambience[0]);
}

void Light::SetRightDiffuse(int shader, vec3 diffuse){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[2].diffuse");
    glUniform3fv(vecLocation, 1, &diffuse[0]);
}

void Light::SetRightSpecular(int shader, vec3 specular){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[2].specular");
    glUniform3fv(vecLocation, 1, &specular[0]);
}

void Light::SetRightColor(int shader, vec3 lightColor){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[2].lightColor");
    glUniform3fv(vecLocation, 1, &lightColor[0]);
}

void Light::SetRightPos(int shader, vec3 lightPos){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[2].lightPos");
    glUniform3fv(vecLocation, 1, &lightPos[0]);
}

//Left Tailight
void Light::SetLeftTailAmbience(int shader, vec3 ambience){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[3].ambience");
    glUniform3fv(vecLocation, 1, &ambience[0]);
}

void Light::SetLeftTailDiffuse(int shader, vec3 diffuse){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[3].diffuse");
    glUniform3fv(vecLocation, 1, &diffuse[0]);
}

void Light::SetLeftTailSpecular(int shader, vec3 specular){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[3].specular");
    glUniform3fv(vecLocation, 1, &specular[0]);
}

void Light::SetLeftTailColor(int shader, vec3 lightColor){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[3].lightColor");
    glUniform3fv(vecLocation, 1, &lightColor[0]);
}

void Light::SetLeftTailPos(int shader, vec3 lightPos){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[3].lightPos");
    glUniform3fv(vecLocation, 1, &lightPos[0]);
}

//Right Tailight
void Light::SetRightTailAmbience(int shader, vec3 ambience){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[4].ambience");
    glUniform3fv(vecLocation, 1, &ambience[0]);
}

void Light::SetRightTailDiffuse(int shader, vec3 diffuse){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[4].diffuse");
    glUniform3fv(vecLocation, 1, &diffuse[0]);
}

void Light::SetRightTailSpecular(int shader, vec3 specular){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[4].specular");
    glUniform3fv(vecLocation, 1, &specular[0]);
}

void Light::SetRightTailColor(int shader, vec3 lightColor){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[4].lightColor");
    glUniform3fv(vecLocation, 1, &lightColor[0]);
}

void Light::SetRightTailPos(int shader, vec3 lightPos){
    //glUseProgram(shader);
    GLuint vecLocation = glGetUniformLocation(shader, "pointLights[4].lightPos");
    glUniform3fv(vecLocation, 1, &lightPos[0]);
}
