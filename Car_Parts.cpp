//
//  Car_Parts.cpp
//  COMP371_Labs
//
//  Created by Gianluca Lepore on 2019-09-27.
//  Copyright © 2019 Concordia. All rights reserved.
//

#include <stdio.h>

#include "Car.hpp"

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

CarParts::CarParts(){
    xPos = 0;
    yPos = 0;
    zPos = 0;
}

//Draws Car's Main Body
void CarParts::DrawBody(int shaderProgram){

    mat4 scalingMatrix =  scale(glm::mat4(1.0f), glm::vec3(2.25f,1.2f,1.0f));
    mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,1.0f,0.0f));
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(orientationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 scalingMatrix2 = glm::scale(glm::mat4(1.0f), glm::vec3(Scale+1,Scale+1,Scale+1));
    glm::mat4 translationMatrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(xPos,yPos,zPos));
    glm::mat4 worldMatrix = translationMatrix2 * scalingMatrix2 * rotationMatrix * translationMatrix *  scalingMatrix;
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    };

//Bonnet
void CarParts::DrawBonnet(int shaderProgram){
    glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 0.7f, 0.9f));
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.53f, 0.75f,0.0f));
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(orientationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 scalingMatrix2 = glm::scale(glm::mat4(1.0f), glm::vec3(Scale+1,Scale+1,Scale+1));
    glm::mat4 translationMatrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(xPos,yPos,zPos));
    glm::mat4 worldMatrix = translationMatrix2 * scalingMatrix2 * rotationMatrix * translationMatrix *  scalingMatrix;
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    //worldMatrixLocation = glGetUniformLocation(text, "worldMatrix");
    //glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
};

//Roof
void CarParts::DrawRoof(int shaderProgram){
    glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.25f, 0.5f, 0.8f));
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,1.82f,0.0f));
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(orientationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 scalingMatrix2 = glm::scale(glm::mat4(1.0f), glm::vec3(Scale+1,Scale+1,Scale+1));
    glm::mat4 translationMatrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(xPos,yPos,zPos));
    glm::mat4 worldMatrix = translationMatrix2 * scalingMatrix2 * rotationMatrix * translationMatrix *  scalingMatrix;
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    //worldMatrixLocation = glGetUniformLocation(text, "worldMatrix");
    //glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
};

//DrawTrunk
void CarParts::DrawTrunk(int shaderProgram){
    glm::mat4 scalingMatrix =  glm::scale(glm::mat4(1.0f), glm::vec3(0.6f, 0.5f, 1.0f));
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-1.42f,0.75f,0.0f));
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(orientationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 scalingMatrix2 = glm::scale(glm::mat4(1.0f), glm::vec3(Scale+1,Scale+1,Scale+1));
    glm::mat4 translationMatrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(xPos,yPos,zPos));
    glm::mat4 worldMatrix = translationMatrix2 * scalingMatrix2 * rotationMatrix * translationMatrix *  scalingMatrix;
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    //worldMatrixLocation = glGetUniformLocation(text, "worldMatrix");
    //glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
};

void CarParts::DrawGrid(int shaderProgram){
    
    glm::mat4 translationMatrix;
    glm::mat4 worldMatrix = translationMatrix;
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    //worldMatrixLocation = glGetUniformLocation(text, "worldMatrix");
    //glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    
    for (int i=0;i<=100;i++){
        for (int j=0;j<=100;j++){
            translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(i, 0.0f, j));
            worldMatrix = translationMatrix;
            worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
            //worldMatrixLocation = glGetUniformLocation(text, "worldMatrix");
            //glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 6); //NEED TRIANGLES!!
        }
    }
};


//Draws Grid
void CarParts::DrawGrid_x(int shaderProgram){
    
    glm::mat4 translationMatrix;
    glm::mat4 worldMatrix = translationMatrix;
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    //worldMatrixLocation = glGetUniformLocation(text, "worldMatrix");
    //glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    
    for (int i=0;i<=100;i++){
        for (int j=0;j<=100;j++){
            translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(i, 0.0f, j));
            worldMatrix = translationMatrix;
            worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
            //worldMatrixLocation = glGetUniformLocation(text, "worldMatrix");
            //glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
            glDrawArrays(GL_LINES, 0, 2);
        }
    }
};

void CarParts::DrawGrid_z(int shaderProgram){
    
    glm::mat4 translationMatrix;
    glm::mat4 worldMatrix = translationMatrix;
    //GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    //glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    //worldMatrixLocation = glGetUniformLocation(text, "worldMatrix");
    //glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    
    for (int i=0;i<=100;i++){
        for (int j=0;j<=100;j++){
            translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(j, 0.0f, i));
            worldMatrix = translationMatrix;
            GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
            //worldMatrixLocation = glGetUniformLocation(text, "worldMatrix");
            //glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
            glDrawArrays(GL_LINES, 0, 2);
        }
    }
};


//Wheels
void CarParts::DrawWheels(int shaderProgram, float angle){
    
    //FrontRight
    glm::mat4 scalingMatrix =  glm::scale(glm::mat4(1.0f), glm::vec3(0.55f, 0.55f, 0.3f));
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.1f,0.4f,0.6f));
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(orientationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 wheelrotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 scalingMatrix2 = glm::scale(glm::mat4(1.0f), glm::vec3(Scale+1,Scale+1,Scale+1));
    glm::mat4 translationMatrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(xPos,yPos,zPos));
    float turn = std::max(-45.0f, std::min(45.0f, turnAngle));
    mat4 turnMatrix = rotate(mat4(1.0f), radians(turn), vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 worldMatrix = translationMatrix2 * scalingMatrix2 * rotationMatrix * translationMatrix * turnMatrix * wheelrotationMatrix * scalingMatrix;
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    //worldMatrixLocation = glGetUniformLocation(text, "worldMatrix");
    //glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    //FrontLeft
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.1f,0.4f,-0.6f));
    //wheelrotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    translationMatrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(xPos,yPos,zPos));
    worldMatrix = translationMatrix2 * scalingMatrix2 * rotationMatrix * translationMatrix * turnMatrix * wheelrotationMatrix * scalingMatrix;
    worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    //worldMatrixLocation = glGetUniformLocation(text, "worldMatrix");
    //glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    //BackRight
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-1.1f, 0.4f,0.6f));
    //wheelrotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    translationMatrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(xPos,yPos,zPos));
    worldMatrix = translationMatrix2 * scalingMatrix2 * rotationMatrix * translationMatrix * wheelrotationMatrix * scalingMatrix;
    worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    //worldMatrixLocation = glGetUniformLocation(text, "worldMatrix");
    //glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //BackLeft
    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-1.1f,0.4f,-0.6f));
    //wheelrotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    //translationMatrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(xPos,yPos,zPos));
    worldMatrix = translationMatrix2 * scalingMatrix2 * rotationMatrix * translationMatrix *  wheelrotationMatrix * scalingMatrix;
    worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    //worldMatrixLocation = glGetUniformLocation(text, "worldMatrix");
    //glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void CarParts::DrawWheels2(int shaderProgram, float angle){
    //Front Right
    mat4 scalingMatrix =  scale(mat4(1.0f), vec3(0.55f, 0.55f, 0.55f));
    mat4 translationMatrix = translate(mat4(1.0f), vec3(1.1f,0.55f,0.6f));
    mat4 rotationMatrix = rotate(glm::mat4(1.0f), radians(orientationAngle), vec3(0.0f, 1.0f, 0.0f));
    mat4 wheelrotationMatrix = rotate(mat4(1.0f), radians(angle), vec3(0.0f, 0.0f, 1.0f));
    mat4 scalingMatrix2 = scale(mat4(1.0f), vec3(Scale+1,Scale+1,Scale+1));
    mat4 translationMatrix2 = translate(mat4(1.0f), vec3(xPos,yPos,zPos));
    float turn = std::max(-45.0f, std::min(45.0f, turnAngle));
    mat4 turnMatrix = rotate(mat4(1.0f), radians(turn), vec3(0.0f, 1.0f, 0.0f));
    mat4 worldMatrix = translationMatrix2 * scalingMatrix2 * rotationMatrix * translationMatrix * turnMatrix * wheelrotationMatrix * scalingMatrix;
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    //worldMatrixLocation = glGetUniformLocation(text, "worldMatrix");
    //glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 144);
    
    //Front Left
    translationMatrix = translate(mat4(1.0f), vec3(1.1f,0.55f,-0.6f));
    worldMatrix = translationMatrix2 * scalingMatrix2 * rotationMatrix * translationMatrix * turnMatrix * wheelrotationMatrix * scalingMatrix;
    worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    //worldMatrixLocation = glGetUniformLocation(text, "worldMatrix");
    //glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 144);
    
    //BackRight
    translationMatrix = translate(mat4(1.0f), vec3(-1.1f,0.55f,0.6f));
    worldMatrix = translationMatrix2 * scalingMatrix2 * rotationMatrix * translationMatrix * wheelrotationMatrix * scalingMatrix;
    worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    //worldMatrixLocation = glGetUniformLocation(text, "worldMatrix");
    //glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 144);
    
    //BackLeft
    translationMatrix = translate(mat4(1.0f), vec3(-1.1f,0.55f,-0.6f));
    worldMatrix = translationMatrix2 * scalingMatrix2 * rotationMatrix * translationMatrix * wheelrotationMatrix * scalingMatrix;
    worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    //worldMatrixLocation = glGetUniformLocation(text, "worldMatrix");
    //glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 144);
}

void CarParts::DrawHeadLights(int shader){
    //Left HeadLight
    mat4 scalingMatrix =  scale(glm::mat4(1.0f), vec3(0.2f, 0.2f, 0.25f));
    mat4 translationMatrix = translate(mat4(1.0f), vec3(1.9f,0.85f,-0.25f));
    mat4 rotationMatrix = rotate(glm::mat4(1.0f), radians(orientationAngle), vec3(0.0f, 1.0f, 0.0f));
    mat4 translationMatrix2 = translate(mat4(1.0f), vec3(xPos,yPos,zPos));
    mat4 scalingMatrix2 = scale(mat4(1.0f), vec3(Scale+1,Scale+1,Scale+1));
    mat4 worldMatrix = translationMatrix2 * scalingMatrix2 * rotationMatrix * translationMatrix * scalingMatrix;
    GLuint worldMatrixLocation = glGetUniformLocation(shader, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    //Right Headlight
    translationMatrix = translate(mat4(1.0f), vec3(1.9f,0.85f,0.25f));
    worldMatrix = translationMatrix2 * scalingMatrix2 * rotationMatrix * translationMatrix * scalingMatrix;
    worldMatrixLocation = glGetUniformLocation(shader, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    //Left TailLight
    translationMatrix = translate(mat4(1.0f), vec3(-1.65f,0.85f,0.25f));
    worldMatrix = translationMatrix2 * scalingMatrix2 * rotationMatrix * translationMatrix * scalingMatrix;
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    //Right Tailight
    translationMatrix = translate(mat4(1.0f), vec3(-1.65f,0.85f,-0.25f));
    worldMatrix = translationMatrix2 * scalingMatrix2 * rotationMatrix * translationMatrix * scalingMatrix;
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
};

vec3 CarParts::GetRightHeadLight(){
    //mat4 scalingMatrix =  scale(glm::mat4(1.0f), vec3(0.2f, 0.2f, 0.25f));
    //mat4 translationMatrix = translate(mat4(1.0f), vec3(1.9f,0.85f,-0.25f));
     mat4 rotationMatrix = rotate(glm::mat4(1.0f), radians(orientationAngle), vec3(0.0f, 1.0f, 0.0f));
     mat4 translationMatrix2 = translate(mat4(1.0f), vec3(xPos,yPos,zPos));
    mat4 scalingMatrix2 = scale(mat4(1.0f), vec3(Scale+1,Scale+1,Scale+1));
    vec3 worldPosition = translationMatrix2 * scalingMatrix2 * rotationMatrix *  vec4(1.9f,0.85f,0.25f,1.0f);
    return worldPosition;
}

vec3 CarParts::GetLeftHeadLight(){
    //mat4 scalingMatrix =  scale(glm::mat4(1.0f), vec3(0.2f, 0.2f, 0.25f));
    //mat4 translationMatrix = translate(mat4(1.0f), vec3(1.9f,0.85f,-0.25f));
    mat4 rotationMatrix = rotate(glm::mat4(1.0f), radians(orientationAngle), vec3(0.0f, 1.0f, 0.0f));
    mat4 translationMatrix2 = translate(mat4(1.0f), vec3(xPos,yPos,zPos));
    mat4 scalingMatrix2 = scale(mat4(1.0f), vec3(Scale+1,Scale+1,Scale+1));
    vec3 worldPosition = translationMatrix2 * scalingMatrix2 * rotationMatrix *  vec4(1.9f,0.85f,-0.25f,1.0f);
    return worldPosition;
}

vec3 CarParts::GetRightTailight(){
    //mat4 scalingMatrix =  scale(glm::mat4(1.0f), vec3(0.2f, 0.2f, 0.25f));
    //mat4 translationMatrix = translate(mat4(1.0f), vec3(1.9f,0.85f,-0.25f));
    mat4 rotationMatrix = rotate(glm::mat4(1.0f), radians(orientationAngle), vec3(0.0f, 1.0f, 0.0f));
    mat4 translationMatrix2 = translate(mat4(1.0f), vec3(xPos,yPos,zPos));
    mat4 scalingMatrix2 = scale(mat4(1.0f), vec3(Scale+1,Scale+1,Scale+1));
    vec3 worldPosition = translationMatrix2 * scalingMatrix2 * rotationMatrix *  vec4(-1.65f,0.85f,0.25f,1.0f);
    return worldPosition;
}

vec3 CarParts::GetLeftTailight(){
    //mat4 scalingMatrix =  scale(glm::mat4(1.0f), vec3(0.2f, 0.2f, 0.25f));
    //mat4 translationMatrix = translate(mat4(1.0f), vec3(1.9f,0.85f,-0.25f));
    mat4 rotationMatrix = rotate(glm::mat4(1.0f), radians(orientationAngle), vec3(0.0f, 1.0f, 0.0f));
    mat4 translationMatrix2 = translate(mat4(1.0f), vec3(xPos,yPos,zPos));
    mat4 scalingMatrix2 = scale(mat4(1.0f), vec3(Scale+1,Scale+1,Scale+1));
    vec3 worldPosition = translationMatrix2 * scalingMatrix2 * rotationMatrix *  vec4(-1.65f,0.85f,-0.25f,1.0f);
    return worldPosition;
}

vec3 CarParts::GetThirdPersonPosition(){
    
    mat4 translationMatrix2 = translate(mat4(1.0f), vec3(xPos,yPos,zPos));
    mat4 translationMatrix = translate(mat4(1.0f), vec3(-5.25f, 3.3f, 0.0f));
    mat4 rotationMatrix = rotate(glm::mat4(1.0f), radians(orientationAngle), vec3(0.0f, 1.0f, 0.0f));
    
    mat4 scalingMatrix = scale(mat4(1.0f), vec3(Scale+1,Scale+1,Scale+1));
    vec3 worldPosition = translationMatrix2 * scalingMatrix * rotationMatrix * vec4(-5.25f, 3.3f, 0.0f,1.0f);
    return worldPosition;
}

vec3 CarParts::GetFirstPersonPosition(){
    //vec3 FirstPersonCamPos = vec3(c.get_xPos()+1.25f, 1.3f, c.get_zPos());
    mat4 translationMatrix2 = translate(mat4(1.0f), vec3(xPos,yPos,zPos));
    mat4 translationMatrix = translate(mat4(1.0f), vec3(-5.25f, 3.3f, 0.0f));
    mat4 rotationMatrix = rotate(glm::mat4(1.0f), radians(orientationAngle), vec3(0.0f, 1.0f, 0.0f));
    
    mat4 scalingMatrix = scale(mat4(1.0f), vec3(Scale+1,Scale+1,Scale+1));
    vec3 worldPosition = translationMatrix2 * scalingMatrix * rotationMatrix * vec4(1.25f, 1.5f, 0.0f, 1.0f);
    return worldPosition;
}

//Sets new position for car on grid
void CarParts::SetPositions(int x, int y, int z){
    xPos=x;
    yPos=y;
    zPos=z;
}

int CarParts::get_xPos(){
    return xPos;
}

int CarParts::get_yPos(){
    return yPos;
}

int CarParts::get_zPos(){
    return zPos;
}

void CarParts::SetScale(float x){
    Scale=x;
}

float CarParts::get_Scale(){
    return Scale;

}

float CarParts::get_Orientation(){
    return orientationAngle;
}

void CarParts::SetOrientationAngle(float x){
    orientationAngle=x;
}

float CarParts::get_Turn(){
    return turnAngle;
}

void CarParts::SetTurnAngle(float angle){
    turnAngle = angle;
}


