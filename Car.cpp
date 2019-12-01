//
//  Car.cpp
//  COMP371_Labs
//
//  Created by Gianluca Lepore on 2019-09-27.
//  Copyright © 2019 Concordia. All rights reserved.
//

#include "Car.hpp"
#include "Light.hpp"
#include "Quad.hpp"
#include "Collision.hpp"

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

const char* getCarVertexShaderSource()
{
    
    return
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;"
    "layout (location = 1) in vec3 aColor;"
    ""
    "uniform mat4 worldMatrix;"
    "uniform mat4 viewMatrix = mat4(1.0f);"
    "uniform mat4 projectionMatrix = mat4(1.0f);"
    "uniform mat4 rotationMatrix_x = mat4(1.0f);"
    "uniform mat4 rotationMatrix_y = mat4(1.0f);"
    ""
    "out vec3 vertexColor;"
    ""
    "void main()"
    "{"
    "   vertexColor = aColor;"
    "   gl_Position = projectionMatrix * viewMatrix * rotationMatrix_x * rotationMatrix_y * worldMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
    "}";
}

const char* getCarFragmentShaderSource()
{
    return
    "#version 330 core\n"
    "in vec3 vertexColor;"
    ""
    "out vec4 FragColor;"
    "void main()"
    "{"
    "   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
    "}";
}

const char* getCarTexturedVertexShaderSource()
{
    
    return
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;"
    "layout (location = 1) in vec3 aColor;"
    "layout (location = 2) in vec2 aText;"
    "layout (location = 3) in vec3 aNormal;"
    ""
    "uniform mat4 worldMatrix;"
    "uniform mat4 viewMatrix = mat4(1.0f);"
    "uniform mat4 projectionMatrix = mat4(1.0f);"
    "uniform mat4 rotationMatrix_x = mat4(1.0f);"
    "uniform mat4 rotationMatrix_y = mat4(1.0f);"
    ""
    "out vec3 vertexColor;"
    "out vec3 normal;"
    "out vec3 fragPos;"
    "out vec2 textUV;"
    ""
    "void main()"
    "{"
    "   vertexColor = aColor;"
    "   gl_Position = projectionMatrix * viewMatrix * rotationMatrix_x * rotationMatrix_y * worldMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
    "   fragPos = vec3(worldMatrix * vec4(aPos, 1.0));"
    "   normal = mat3(transpose(inverse(worldMatrix))) * aNormal;"
    "   textUV = aText;"
    "}";
}

const char* getCarTexturedFragmentShaderSource()
{
    return
    "#version 330 core\n"
    ""
    "struct PointLight{"
    "vec3 lightPos;"
    "vec3 ambience;"
    "vec3 diffuse;"
    "vec3 specular;"
    "vec3 lightColor;"
    "};"
    //"#define PointLight_num 3"
    "in vec3 vertexColor;"
    "in vec3 normal;"
    "in vec3 fragPos;"
    "in vec2 textUV;"
    ""
    "uniform sampler2D textureSampler;"
    ""
    //"uniform vec3 lightPos;"
    "uniform vec3 cameraPos;"
    "uniform PointLight pointLights[1];"
    ""
    "vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);"
    ""
    "out vec4 FragColor;"
    "void main()"
    "{" //properties
    "   vec3 norm = normalize(normal);"
    "   vec3 viewDir = normalize(cameraPos - fragPos);"
    "   vec3 result = CalcPointLight(pointLights[0], norm, fragPos, viewDir);"
    "   for (int i=1; i<3; i++){"
    "       result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);"
    "   }"
    "   vec4 textureColor = texture(textureSampler,textUV);"
    "   FragColor = textureColor * vec4(result, 1.0f) * vec4(vertexColor, 1.0f);"
    "}"
    "   vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){"
    "       vec3 lightDir = normalize(light.lightPos - fragPos);"
    "       float diff = max(dot(normal, lightDir), 0.0);"
    "       vec3 reflectDir = reflect(-lightDir, normal);"
    "       float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);"
    "       vec3 ambientLight = light.ambience * light.lightColor;"
    "       vec3 diffuseLight = light.diffuse * diff * light.lightColor;"
    "       vec3 specularLight = light.specular * spec * light.lightColor;"
    "       return (ambientLight + diffuseLight + specularLight);"
    "}";
}

const char* getLampVertexShaderSource()
{
    return
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;"
    ""
    "uniform mat4 worldMatrix;"
    "uniform mat4 viewMatrix = mat4(1.0f);"
    "uniform mat4 projectionMatrix = mat4(1.0f);"
    "uniform mat4 rotationMatrix_x = mat4(1.0f);"
    "uniform mat4 rotationMatrix_y = mat4(1.0f);"
    ""
    "void main()"
    "{"
    "   gl_Position = projectionMatrix * viewMatrix * rotationMatrix_x * rotationMatrix_y * worldMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
    "}";
}


const char* getLampFragmentShaderSource()
{
    return
    "#version 330 core\n"
    "out vec4 FragColor;"
    ""
    "void main()"
    "{"
    "   FragColor = vec4(1.0);"
    "}";
}

const char* getGridVertexShaderSource()
{
    
    return
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;"
    "layout (location = 1) in vec3 aColor;"
    ""
    "uniform mat4 worldMatrix;"
    "uniform mat4 viewMatrix = mat4(1.0f);"
    "uniform mat4 projectionMatrix = mat4(1.0f);"
    "uniform mat4 rotationMatrix_x = mat4(1.0f);"
    "uniform mat4 rotationMatrix_y = mat4(1.0f);"
    ""
    "out vec3 vertexColor;"
    ""
    "void main()"
    "{"
    "   vertexColor = aColor;"
    "   gl_Position = projectionMatrix * viewMatrix * rotationMatrix_x * rotationMatrix_y * worldMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
    "}";
}

const char* getGridFragmentShaderSource()
{
    return
    "#version 330 core\n"
    "in vec3 vertexColor;"
    ""
    "out vec4 FragColor;"
    "void main()"
    "{"
    "   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
    "}";
}

const char* getGridTexturedVertexShaderSource()
{
    
    return
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;"
    "layout (location = 1) in vec3 aColor;"
    "layout (location = 2) in vec2 aText;"
    "layout (location = 3) in vec3 aNormal;"
    ""
    "uniform mat4 worldMatrix;"
    "uniform mat4 viewMatrix = mat4(1.0f);"
    "uniform mat4 projectionMatrix = mat4(1.0f);"
    "uniform mat4 rotationMatrix_x = mat4(1.0f);"
    "uniform mat4 rotationMatrix_y = mat4(1.0f);"
    "uniform mat4 lightSpaceMatrix;"
    ""
    "out vec3 vertexColor;"
    "out vec3 normal;"
    "out vec3 fragPos;"
    "out vec2 textUV;"
    "out vec4 fragPosLightSpace;"
    ""
    "void main()"
    "{"
    "   vertexColor = aColor;"
    "   gl_Position = projectionMatrix * viewMatrix * rotationMatrix_x * rotationMatrix_y * worldMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
    //"   gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
    "   fragPos = vec3(worldMatrix * vec4(aPos, 1.0));"
    "   normal = mat3(transpose(inverse(worldMatrix))) * aNormal;"
    "   textUV = aText;"
    "   fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);"
    "}";
}
/*
const char* getGridTexturedVertexShaderSource()
{
    
    return
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;"
    "layout (location = 1) in vec3 aColor;"
    "layout (location = 2) in vec2 aText;"
    "layout (location = 3) in vec3 aNormal;"
    ""
    "uniform mat4 worldMatrix;"
    "uniform mat4 viewMatrix = mat4(1.0f);"
    "uniform mat4 projectionMatrix = mat4(1.0f);"
    "uniform mat4 rotationMatrix_x = mat4(1.0f);"
    "uniform mat4 rotationMatrix_y = mat4(1.0f);"
    ""
    "out vec3 vertexColor;"
    "out vec3 normal;"
    "out vec3 fragPos;"
    "out vec2 textUV;"
    ""
    "void main()"
    "{"
    "   vertexColor = aColor;"
    "   gl_Position = projectionMatrix * viewMatrix * rotationMatrix_x * rotationMatrix_y * worldMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
    "   fragPos = vec3(worldMatrix * vec4(aPos, 1.0));"
    "   normal = mat3(transpose(inverse(worldMatrix))) * aNormal;"
    "   textUV = aText;"
    "}";
}
 */

const char* getGridTexturedFragmentShaderSource()
{
    return
    "#version 330 core\n"
    ""
    "struct PointLight{"
    "vec3 lightPos;"
    "vec3 ambience;"
    "vec3 diffuse;"
    "vec3 specular;"
    "vec3 lightColor;"
    "};"
    "in vec3 vertexColor;"
    "in vec3 normal;"
    "in vec3 fragPos;"
    "in vec2 textUV;"
    "in vec4 fragPosLightSpace;"
    ""
    "uniform sampler2D textureSampler;"
    "uniform sampler2D diffuseTexture;"
    "uniform sampler2D shadowMap;"
    ""
    "uniform vec3 cameraPos;"
    "uniform PointLight pointLights[5];"
    ""
    "vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);"
    ""
    "out vec4 FragColor;"
    ""
    "float ShadowCalculation(vec4 fragPosLightSpace,PointLight light,vec3 fragPos){"
    "   vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;"
    "   projCoords = projCoords * 0.5 + 0.5;"
    "   float closestDepth = texture(shadowMap, projCoords.xy).r;"
    "   float currentDepth = projCoords.z;"
    "   vec3 lightDir = normalize(light.lightPos - fragPos);"
    "   float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);"
    //"   float bias = 0.01;"
    "   float shadow = ((currentDepth - bias) > closestDepth) ? 1.0 : 0.0;"
    "   return shadow;"
    "}"
    "void main()"
    "{" //properties
    "   vec3 norm = normalize(normal);"
    "   vec3 viewDir = normalize(cameraPos - fragPos);"
    "   vec3 result = CalcPointLight(pointLights[0], norm, fragPos, viewDir);"
    "   for (int i=1; i<5; i++){"
    "       result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);"
    "   }"
    "   vec4 textureColor = texture(textureSampler,textUV);"
    "   FragColor = textureColor * vec4(result, 1.0f);"
    "}"
    "   vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){"
    "       vec3 lightDir = normalize(light.lightPos - fragPos);"
    "       float diff = max(dot(normal, lightDir), 0.0);"
    "       vec3 reflectDir = reflect(-lightDir, normal);"
    "       float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);"
    "       vec3 ambientLight = light.ambience * light.lightColor;"
    "       vec3 diffuseLight = light.diffuse * diff * light.lightColor;"
    "       vec3 specularLight = light.specular * spec * light.lightColor;"
    "       return (ambientLight + ((1.0f - ShadowCalculation(fragPosLightSpace, pointLights[0], fragPos)) * (diffuseLight + specularLight)));"
    //"       return (ambientLight + diffuseLight + specularLight);"
    "}";
}
/*
const char* getGridTexturedFragmentShaderSource()
{
    return
    "#version 330 core\n"
    ""
    "struct PointLight{"
    "vec3 lightPos;"
    "vec3 ambience;"
    "vec3 diffuse;"
    "vec3 specular;"
    "vec3 lightColor;"
    "};"
    //"#define PointLight_num 3"
    "in vec3 vertexColor;"
    "in vec3 normal;"
    "in vec3 fragPos;"
    "in vec2 textUV;"
    ""
    "uniform sampler2D textureSampler;"
    ""
    //"uniform vec3 lightPos;"
    "uniform vec3 cameraPos;"
    "uniform PointLight pointLights[3];"
    ""
    "vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);"
    ""
    "out vec4 FragColor;"
    "void main()"
    "{" //properties
    "   vec3 norm = normalize(normal);"
    "   vec3 viewDir = normalize(cameraPos - fragPos);"
    //"   float ambience = 0.1;"
    //"   vec3 ambientLight = ambience * lightColor;"
    //""
    "   vec3 result = CalcPointLight(pointLights[0], norm, fragPos, viewDir);"
    "   for (int i=1; i<3; i++){"
    "       result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);"
    "   }"
    "   vec4 textureColor = texture(textureSampler,textUV);"
    "   FragColor = textureColor * vec4(result, 1.0f);"
    "}"
    "   vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){"
    "       vec3 lightDir = normalize(light.lightPos - fragPos);"
    "       float diff = max(dot(normal, lightDir), 0.0);"
    "       vec3 reflectDir = reflect(-lightDir, normal);"
    "       float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);"
    "       vec3 ambientLight = light.ambience * light.lightColor;"
    "       vec3 diffuseLight = light.diffuse * diff * light.lightColor;"
    "       vec3 specularLight = light.specular * spec * light.lightColor;"
    "       return (ambientLight + diffuseLight + specularLight);"
    "}";
    
}
*/
const char* getShadowDepthVertexShaderSource()
{
    
    return
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;"
    ""
    "uniform mat4 lightSpaceMatrix;"
    "uniform mat4 worldMatrix=mat4(1.0f);"
    "uniform mat4 rotationMatrix_x = mat4(1.0f);"
    "uniform mat4 rotationMatrix_y = mat4(1.0f);"
    ""
    "void main()"
    "{"
    "   gl_Position = lightSpaceMatrix * rotationMatrix_x * rotationMatrix_y * worldMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);"

    "}";
}
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
const unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;

const char* getShadowDepthFragmentShaderSource(){
    return
    "#version 330 core\n"
    //"out vec4 FragColor;"
    "void main()"
    "{"
    //"   FragColor = vec4(0.0f,1.0f,0.0f, 1.0f);"
    //"   gl_FragDepth = gl_FragCoord.z"
    "}";
}

const char* getParticleVertexShaderSource()
{
    
    return
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;"
    "layout (location = 1) in vec3 aNormal;"
    "layout (location = 2) in vec3 aColor;"
    "layout (location = 3) in vec2 aUV;"
    ""
    "uniform mat4 worldMatrix;"
    "uniform mat4 viewMatrix = mat4(1.0f);"
    "uniform mat4 projectionMatrix = mat4(1.0f);"
    "uniform mat4 rotationMatrix_x = mat4(1.0f);"
    "uniform mat4 rotationMatrix_y = mat4(1.0f);"
    ""
    "out vec3 vertexColor;"
    "out vec3 normal;"
    ""
    "void main()"
    "{"
    "   vertexColor = aColor;"
    "   gl_Position = projectionMatrix * viewMatrix * rotationMatrix_x * rotationMatrix_y * worldMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
    "   normal = mat3(transpose(inverse(worldMatrix))) * aNormal;"
    "}";
}

const char* getParticleFragmentShaderSource()
{
    return
    "#version 330 core\n"
    "in vec3 vertexColor;"
    "in vec3 normal;"
    ""
    "out vec4 FragColor;"
    "uniform float opacity;"
    "void main()"
    "{"
    "   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, opacity);"
    "}";
}

int compileAndLinkShaders(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    // compile and link shader program
    // return shader program id
    // ------------------------------------
    
    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}

struct TexturedCubeVertex
{
    TexturedCubeVertex(vec3 _position = vec3(0.0f,0.0f,0.0f), vec3 _color = vec3(0.0f,0.0f,0.0f), vec2 _text = vec2(0.0f,0.0f), vec3 _norm = vec3(0.0f,0.0f,0.0f))
    : position(_position), color(_color), text(_text), norm(_norm) {}
    
    vec3 position;
    vec3 color;
    vec2 text;
    vec3 norm;
    
};

int createGrid(){
    
    TexturedCubeVertex Grid[]{
        
        TexturedCubeVertex(vec3(-49.0f, 0.0f, -49.0f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f), vec3(0.0f,1.0f,0.0f)),
        TexturedCubeVertex(vec3(-49.0f, 0.0f, -50.0f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f), vec3(0.0f,1.0f,0.0f)),
        TexturedCubeVertex(vec3(-50.0f, 0.0f, -50.0f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f), vec3(0.0f,1.0f,0.0f)),
        
        TexturedCubeVertex(vec3(-50.0f, 0.0f, -50.0f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f), vec3(0.0f,1.0f,0.0f)),
        TexturedCubeVertex(vec3(-50.0f, 0.0f, -49.0f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f), vec3(0.0f,1.0f,0.0f)),
        TexturedCubeVertex(vec3(-49.0f, 0.0f, -49.0f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f), vec3(0.0f,1.0f,0.0f))
    };
    
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    
    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Grid), Grid, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(TexturedCubeVertex),(void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(TexturedCubeVertex),(void*)sizeof(glm::vec3));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedCubeVertex),(void*)(2*sizeof(vec3)));
    glEnableVertexAttribArray(2);
    
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedCubeVertex),(void*)(2*sizeof(vec3)+sizeof(vec2)));
    glEnableVertexAttribArray(3);
    
    return vertexArrayObject;
}

int createGrid_VAO_x(){
    
    glm::vec3 GridPoints [] = {
        glm::vec3(-50.0f, 0.0f, -50.0f), glm::vec3(1.0f,1.0f,1.0f),
        glm::vec3(-50.0f, 0.0f, -49.0f), glm::vec3(1.0f,1.0f,1.0f),
    };
    
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GridPoints), GridPoints, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,2*sizeof(glm::vec3),(void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,2*sizeof(glm::vec3),(void*)sizeof(glm::vec3));
    glEnableVertexAttribArray(1);
    
    return vertexArrayObject;
}

int createGrid_VAO_z(){
    
    glm::vec3 GridPoints [] = {
        glm::vec3(-50.0f, 0.0f, -50.0f), glm::vec3(1.0f,1.0f,1.0f),
        glm::vec3(-49.0f, 0.0f, -50.0f), glm::vec3(1.0f,1.0f,1.0f),
    };
    
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GridPoints), GridPoints, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,2*sizeof(glm::vec3),(void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,2*sizeof(glm::vec3),(void*)sizeof(glm::vec3));
    glEnableVertexAttribArray(1);
    
    return vertexArrayObject;
    
}

int createAxisX_VAO(){
    
    glm::vec3 AxisPoints[] = {
        glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(5.0f,0.0f,0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
        
    };
    
    GLuint vertexArrayObject2;
    glGenVertexArrays(1, &vertexArrayObject2);
    glBindVertexArray(vertexArrayObject2);
    
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(AxisPoints), AxisPoints, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,2*sizeof(glm::vec3),(void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,2*sizeof(glm::vec3),(void*)sizeof(glm::vec3));
    glEnableVertexAttribArray(1);
    
    return vertexArrayObject2;
}
    
int createAxisY_VAO(){
    
    glm::vec3 AxisPoints[] = {
        glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f,5.0f,0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
    };
    
    GLuint vertexArrayObject2;
    glGenVertexArrays(1, &vertexArrayObject2);
    glBindVertexArray(vertexArrayObject2);
    
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(AxisPoints), AxisPoints, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,2*sizeof(glm::vec3),(void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,2*sizeof(glm::vec3),(void*)sizeof(glm::vec3));
    glEnableVertexAttribArray(1);
    
    
    return vertexArrayObject2;
}

int createAxisZ_VAO(){
    
    glm::vec3 AxisPoints[] = {
        glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f,0.0f,5.0f), glm::vec3(0.0f, 0.0f, 1.0f),
    };
    
    GLuint vertexArrayObject2;
    glGenVertexArrays(1, &vertexArrayObject2);
    glBindVertexArray(vertexArrayObject2);
    
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(AxisPoints), AxisPoints, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,2*sizeof(glm::vec3),(void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,2*sizeof(glm::vec3),(void*)sizeof(glm::vec3));
    glEnableVertexAttribArray(1);
    
    
    return vertexArrayObject2;
}

int* createCubeVertexArrayObject()
{
    
    TexturedCubeVertex vertexArray[] = {
        TexturedCubeVertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(-1.0f,0.0f,0.0f)),//left - red
        TexturedCubeVertex(glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f), vec3(-1.0f,0.0f,0.0f)),//-1,0,0
        TexturedCubeVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(-1.0f,0.0f,0.0f)),
        
        TexturedCubeVertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(-1.0f,0.0f,0.0f)),
        TexturedCubeVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(-1.0f,0.0f,0.0f)),
        TexturedCubeVertex(glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f), vec3(-1.0f,0.0f,0.0f)),
        
        TexturedCubeVertex(glm::vec3( 0.5f, 0.5f,-0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(0.0f,0.0f,-1.0f)), // far - blue
        TexturedCubeVertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(0.0f,0.0f,-1.0f)),//0,0,-1
        TexturedCubeVertex(glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f), vec3(0.0f,0.0f,-1.0f)),
        
        TexturedCubeVertex(glm::vec3( 0.5f, 0.5f,-0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(0.0f,0.0f,-1.0f)),
        TexturedCubeVertex(glm::vec3( 0.5f,-0.5f,-0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f), vec3(0.0f,0.0f,-1.0f)),
        TexturedCubeVertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(0.0f,0.0f,-1.0f)),
        
        TexturedCubeVertex(glm::vec3( 0.5f,-0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(0.0f,-1.0f,0.0f)), // bottom - turquoise
        TexturedCubeVertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(0.0f,-1.0f,0.0f)),//0,-1,0
        TexturedCubeVertex(glm::vec3( 0.5f,-0.5f,-0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f), vec3(0.0f,-1.0f,0.0f)),
        
        TexturedCubeVertex(glm::vec3( 0.5f,-0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(0.0f,-1.0f,0.0f)),
        TexturedCubeVertex(glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f), vec3(0.0f,-1.0f,0.0f)),
        TexturedCubeVertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(0.0f,-1.0f,0.0f)),
        
        TexturedCubeVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f), vec3(0.0f,0.0f,1.0f)),//near- green
        TexturedCubeVertex(glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(0.0f,0.0f,1.0f)), //0,0,1
        TexturedCubeVertex(glm::vec3( 0.5f,-0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f), vec3(0.0f,0.0f,1.0f)),
        
        TexturedCubeVertex(glm::vec3( 0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(0.0f,0.0f,1.0f)),
        TexturedCubeVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f), vec3(0.0f,0.0f,1.0f)),
        TexturedCubeVertex(glm::vec3( 0.5f,-0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f), vec3(0.0f,0.0f,1.0f)),
        
        TexturedCubeVertex(glm::vec3( 0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(1.0f,0.0f,0.0f)),// right - purple
        TexturedCubeVertex(glm::vec3( 0.5f,-0.5f,-0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(1.0f,0.0f,0.0f)),//1,0,0
        TexturedCubeVertex(glm::vec3( 0.5f, 0.5f,-0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f), vec3(1.0f,0.0f,0.0f)),
        
        TexturedCubeVertex(glm::vec3( 0.5f,-0.5f,-0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(1.0f,0.0f,0.0f)),
        TexturedCubeVertex(glm::vec3( 0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(1.0f,0.0f,0.0f)),
        TexturedCubeVertex(glm::vec3( 0.5f,-0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f), vec3(1.0f,0.0f,0.0f)),
        
        TexturedCubeVertex(glm::vec3( 0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(0.0f,1.0f,0.0f)), // top - yellow
        TexturedCubeVertex(glm::vec3( 0.5f, 0.5f,-0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f), vec3(0.0f,1.0f,0.0f)), //0,1,0
        TexturedCubeVertex(glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(0.0f,1.0f,0.0f)),
        
        TexturedCubeVertex(glm::vec3( 0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(0.0f,1.0f,0.0f)),
        TexturedCubeVertex(glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(0.0f,1.0f,0.0f)),
        TexturedCubeVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f), vec3(0.0f,1.0f,0.0f))
    };
    
    // Create a vertex array
    GLuint cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    
    int *vertexArrayObject = new int[3];
    vertexArrayObject[0] = cubeVAO;
    
    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(TexturedCubeVertex),(void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(TexturedCubeVertex),(void*)sizeof(vec3));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedCubeVertex),(void*)(2*sizeof(vec3)));
    glEnableVertexAttribArray(2);
    
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedCubeVertex),(void*)(2*sizeof(vec3)+sizeof(vec2)));
    glEnableVertexAttribArray(3);
    
    //Create Light source VAO
    GLuint lightVAO;
    glGenVertexArrays(1,&lightVAO);
    glBindVertexArray(lightVAO);
    
    vertexArrayObject[1] = lightVAO;
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(TexturedCubeVertex),(void*)0);
    glEnableVertexAttribArray(0);
    
    //Create Headlights VAO
    GLuint headVAO;
    glGenVertexArrays(1,&headVAO);
    glBindVertexArray(headVAO);
    
    vertexArrayObject[2] = headVAO;
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(TexturedCubeVertex),(void*)0);
    glEnableVertexAttribArray(0);
    
    return vertexArrayObject;
}

GLuint loadTexture(char* imagepath)
{
    // Load image using the Free Image library
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(imagepath, 0);
    FIBITMAP* image = FreeImage_Load(format, imagepath);
    FIBITMAP* image32bits = FreeImage_ConvertTo32Bits(image);
    
    // Get an available texture index from OpenGL
    GLuint texture = 0;
    glGenTextures(1, &texture);
    assert(texture != 0);
    
    // Set OpenGL filtering properties (bi-linear interpolation)
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Retrieve width and hight
    int width = FreeImage_GetWidth(image32bits);
    int height = FreeImage_GetHeight(image32bits);
    
    // This will upload the texture to the GPU memory
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height,
                 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image32bits));
    
    // Free images
    FreeImage_Unload(image);
    FreeImage_Unload(image32bits);
    
    return texture;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void setWorldMatrix(int shaderProgram, mat4 worldMatrix)
{
    glUseProgram(shaderProgram);
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}

void setViewMatrix(int shader, mat4 viewMatrix)
{
    glUseProgram(shader);
    GLuint viewMatrixLocation = glGetUniformLocation(shader, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}

void setProjectionMatrix(int shader, mat4 projectionMatrix)
{
    glUseProgram(shader);
    GLuint projectionMatrixLocation = glGetUniformLocation(shader, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void setRotationMatrix_x(int shader, mat4 rotationMatrix_x)
{
    GLuint rotationMatrixLocation = glGetUniformLocation(shader, "rotationMatrix_x");
    glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, &rotationMatrix_x[0][0]);
}

void setRotationMatrix_y(int shader, mat4 rotationMatrix_y)
{
    GLuint rotationMatrixLocation = glGetUniformLocation(shader, "rotationMatrix_y");
    glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, &rotationMatrix_y[0][0]);
}

void setLightSpaceMatrix(int shader, mat4 lightSpaceMatrix)
{
    glUseProgram(shader);
    GLuint lightSpaceMatrixLocation = glGetUniformLocation(shader, "lightSpaceMatrix");
    glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, &lightSpaceMatrix[0][0]);
}

void setShadowMapTexture(int shader, int texture) {
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"), texture);
}

int create_circle(int rad, float num){
    
    float theta=radians((360/num));
    
    vec3 cylinderVertices[144];
    vec3 cylinderColors[144];
    vec2 cylinderTextures[144];
    vec3 cylinderNormals[144];
    TexturedCubeVertex cylinder[sizeof(cylinderVertices)];
    
    int j=0;
    
    //front of wheel
    for (int i=0; i<(num*3);i+=3){
        cylinderVertices[i] = vec3(rad*cosf(theta*j),rad*sinf(theta*j),0.5f);
        cylinderVertices[i+1] = vec3(rad*cosf(theta*(j+1)),rad*sinf(theta*(j+1)),0.5f);
        cylinderVertices[i+2] = vec3(0.0f,0.0f,0.5f);
        j++;
    }
    //back
    for (int i=36; i<(num*6);i+=3){
        cylinderVertices[i] = vec3(rad*cosf(theta*j),rad*sinf(theta*j),-0.5f);
        cylinderVertices[i+1] = vec3(0.0f,0.0f,-0.5f);
        cylinderVertices[i+2] = vec3(rad*cosf(theta*(j+1)),rad*sinf(theta*(j+1)),-0.5f);
        j++;
    }
    
    //curved part of wheel
    for (int i=72; i<(num*12);i+=6){
        cylinderVertices[i] = vec3(rad*cosf(theta*(j+1)),rad*sinf(theta*(j+1)),-0.5f);
        cylinderVertices[i+1] = vec3(rad*cosf(theta*(j+1)),rad*sinf(theta*(j+1)),0.5f);
        cylinderVertices[i+2] = vec3(rad*cosf(theta*j),rad*sinf(theta*j),0.5f);
        cylinderVertices[i+3] = vec3(rad*cosf(theta*j),rad*sinf(theta*j),-0.5f);
        cylinderVertices[i+4] = vec3(rad*cosf(theta*(j+1)),rad*sinf(theta*(j+1)),-0.5f);
        cylinderVertices[i+5] = vec3(rad*cosf(theta*j),rad*sinf(theta*j),0.5f);
        j++;
    }
    //wheel colour
    for (int i=0; i<(num*12);i++){
        cylinderColors[i] = vec3(0.4f, 0.4f, 0.4f);
    }
    
    //textures
    for (int i=0; i<(num*3);i+=3){
        cylinderTextures[i] = vec2(1.0f,1.0f);
        cylinderTextures[i+1] = vec2(0.0f, 1.0f);
        cylinderTextures[i+2] = vec2(0.5f, 0.0f);
    }

    for (int i=36; i<(num*6);i+=3){
        cylinderTextures[i] = vec2(1.0f,1.0f);
        cylinderTextures[i+1] = vec2(0.5f,0.0f);
        cylinderTextures[i+2] = vec2(0.0f,1.0f);
    }
    
    for (int i=72; i<(num*12);i+=6){
        cylinderTextures[i] = vec2(0.0f,1.0f);
        cylinderTextures[i+1] = vec2(0.0f,0.0f);
        cylinderTextures[i+2] = vec2(1.0f,0.0f);
        cylinderTextures[i+3] = vec2(1.0f,1.0f);
        cylinderTextures[i+4] = vec2(0.0f,1.0f);
        cylinderTextures[i+5] = vec2(1.0f,0.0f);
    }
    
    //normals
    for (int i=0; i<(num*3);i+=3){
        cylinderNormals[i] = vec3(0.0f,0.0f,1.0f);
        cylinderNormals[i+1] = vec3(0.0f,0.0f,1.0f);
        cylinderNormals[i+2] = vec3(0.0f,0.0f,1.0f);
    }
    
    for (int i=36; i<(num*6);i+=3){
        cylinderNormals[i] = vec3(0.0f,0.0f,-1.0f);
        cylinderNormals[i+1] = vec3(0.0f,0.0f,-1.0f);
        cylinderNormals[i+2] = vec3(0.0f,0.0f,-1.0f);
    }
    
    int l=0;
    for (int i=72; i<(num*12);i+=6){
        cylinderNormals[i] = vec3(cosf(theta*l),sinf(theta*l),0.0f);
        cylinderNormals[i+1] = vec3(cosf(theta*l),sinf(theta*l),0.0f);
        cylinderNormals[i+2] = vec3(cosf(theta*l),sinf(theta*l),0.0f);
        cylinderNormals[i+3] = vec3(cosf(theta*l),sinf(theta*l),0.0f);
        cylinderNormals[i+4] = vec3(cosf(theta*l),sinf(theta*l),0.0f);
        cylinderNormals[i+5] = vec3(cosf(theta*l),sinf(theta*l),0.0f);
        l++;
    }
    
    //all together
    for (int i=0; i<(144); i++){
        cylinder[i] = TexturedCubeVertex(cylinderVertices[i], cylinderColors[i], cylinderTextures[i], cylinderNormals[i]);
    }
    
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cylinder), cylinder, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(TexturedCubeVertex),(void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(TexturedCubeVertex),(void*)sizeof(vec3));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(TexturedCubeVertex),(void*)(2*sizeof(vec3)));
    glEnableVertexAttribArray(2);
    
    glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,sizeof(TexturedCubeVertex),(void*)(2*sizeof(vec3)+sizeof(vec2)));
    glEnableVertexAttribArray(3);
    
    return vertexArrayObject;
}

enum carView {def,first,third, sec};

//------------------------------------------------------------------------------
//MAIN

int main(int argc, char*argv[])
{
    // Initialize GLFW and OpenGL version
    glfwInit();
    
    #if defined(PLATFORM_OSX)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #else
    // On windows, we set OpenGL version to 2.1, to support more hardware
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    #endif
    
    // Create Window and rendering context using GLFW, resolution is 800x600
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Comp371 - Project", NULL, NULL); //1024,968
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowAspectRatio(window, SCR_WIDTH, SCR_HEIGHT);
    
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << endl;
        glfwTerminate();
        return -1;
    }

    // Load Textures
    #if defined(PLATFORM_OSX)
    GLuint grassTextureID = loadTexture((char*)"/Users/gianlucalepore/Documents/COMP371/grasstexture.jpg");
    GLuint metalTextureID = loadTexture((char*)"/Users/gianlucalepore/Documents/COMP371/metaltexture.jpg");
    GLuint tireTextureID = loadTexture((char*)"/Users/gianlucalepore/Documents/COMP371/tiretexture.jpg");
    GLuint smokeTextureID = loadTexture((char*)"/Users/gianlucalepore/Documents/COMP371/Particle.png");
    #else
    GLuint grassTextureID = loadTexture((char*)"../Assets/Textures/grasstexture.jpg");
    GLuint metalTextureID = loadTexture((char*)"../Assets/Textures/metaltexture.jpg");
    GLuint metalTextureID = loadTexture((char*)"../Assets/Textures/tiretexture.jpg");
    GLuint smokeTextureID = loadTexture((char*)"../Assets/Textures/Particle.png");
    #endif
    
    // Black background
    //glViewport(0, 0, 800, 600);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Compile and link shaders here ...
    //Car with no lighting or texture
    int shaderProgram = compileAndLinkShaders(getCarVertexShaderSource(), getCarFragmentShaderSource());
    //Car with lighting and texture
    int textShaderProgram = compileAndLinkShaders(getCarTexturedVertexShaderSource(),getCarTexturedFragmentShaderSource());
    //Shader for light source
    int lightShaderProgram = compileAndLinkShaders(getLampVertexShaderSource(), getLampFragmentShaderSource());
    //Shader for regular grid with no light
    int gridShaderProgram = compileAndLinkShaders(getGridVertexShaderSource(), getGridFragmentShaderSource());
    //Shader for textured grid with light
    int gridTextShaderProgram = compileAndLinkShaders(getGridTexturedVertexShaderSource(), getGridTexturedFragmentShaderSource());
    //Shader for depth map
    int simpleDepthShaderProgram = compileAndLinkShaders(getShadowDepthVertexShaderSource(), getShadowDepthFragmentShaderSource());
    //Particle shader
    int particleShaderProgram  = compileAndLinkShaders(getParticleVertexShaderSource(), getParticleFragmentShaderSource());
    
    //Default camera positions
    glm::vec3 cameraPosition(0.0f, 18.0f, 55.0f);
    glm::vec3 cameraLookAt(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
    
    
    // Define and upload geometry to the GPU here ...
    
    int *vao = createCubeVertexArrayObject();
    int vao_cube = vao[0];
    int vao_light = vao[1];
    int vao_Z = createAxisZ_VAO();
    int vao_X = createAxisX_VAO();
    int vao_Y = createAxisY_VAO();
    int vao_grid_x = createGrid_VAO_x();
    int vao_grid_z = createGrid_VAO_z();
    int vao_grid = createGrid();
    int vao_circle = create_circle(1, 12);
    int vao_head = vao[2];
    int carshader = 0; //the car's active shader
    
    // Variables to be used later in tutorial
    float angle = 0;
    float rotationSpeed = 180.0f;  // 180 degrees per second
   
    //World orientation angle
    float worldAngle_x = 0;
    float worldAngle_y = 0;
    
    //Camera parameters
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;
    //float CameraSpeed = 8.0f;
    //int lastMouseLeftState = GLFW_RELEASE;
    
    //For Frame time calculations
    float lastFrameTime = glfwGetTime();
    double lastMousePosX;
    double lastMousePosY;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
    
    // Enable Backface culling
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    
    //View Matrix
    glUseProgram(shaderProgram);
    glm::mat4 viewMatrix_default = glm::mat4(1.0f);
    viewMatrix_default = glm::lookAt(cameraPosition,cameraPosition + cameraLookAt,cameraUp);
    setViewMatrix(shaderProgram, viewMatrix_default);
    
    //Projection Matrix
    float fov = 70.0f;
    glm::mat4 projectionMatrix = glm::perspective(fov, (float) SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    setProjectionMatrix(shaderProgram, projectionMatrix);
    
    //glUseProgram(textShaderProgram);
    setViewMatrix(textShaderProgram, viewMatrix_default);
    setProjectionMatrix(textShaderProgram, projectionMatrix);
    
    //glUseProgram(lightShaderProgram);
    setViewMatrix(lightShaderProgram, viewMatrix_default);
    setProjectionMatrix(lightShaderProgram, projectionMatrix);
    
    //glUseProgram(gridTextShaderProgram);
    setViewMatrix(gridTextShaderProgram, viewMatrix_default);
    setProjectionMatrix(gridTextShaderProgram, projectionMatrix);
    
    //glUseProgram(gridShaderProgram);
    setViewMatrix(gridShaderProgram, viewMatrix_default);
    setProjectionMatrix(gridShaderProgram, projectionMatrix);
    
    //float newfov = 70;
    
    //Class objects
    CarParts c[7] = {{0,0,0,0.0},{4,0,0,0.0},{8,0,0,0.0},{12,0,0,0.0},{16,0,0,0.0},{-16,0,0,0.0},{-10,0,0,0.0}};
    //vector<CarParts> c;
    //c.push_back(c(0,0,0,0.0));
    Collision col;
    Light l;
    
    //View variable
    carView view = def;
    
    //For toggling between shaders and views
    bool carTexture = false;
    bool gridTexture = false;
    
    //Light parameteres
    vec3 LampPos(0.0f, 30.0f, 0.0f); //
    vec3 rightLightPos(1.9f,0.85f,0.25f);
    vec3 leftLightPos(1.9f,0.85f,-0.25f);
    vec3 rightTailightPos(-1.65f,0.85f,0.25f);
    vec3 leftTailightPos(-1.65f,0.85f,-0.25f);
    
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 TailightColor = vec3 (1.0f,0.0f,0.0f);
    vec3 ambience = vec3(0.1f, 0.1f, 0.1f);
    vec3 Tailambience = vec3(0.01f,0.01f,0.01f);
    vec3 diffuse = vec3(1.0f, 1.0f, 1.0f);
    vec3 specular = vec3(0.5f, 0.5f, 0.5f);
    
    
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    //creates depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //attach depth texture as depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // Set texture unit # for shadow map
    
    
    // Entering Main Loop
    while(!glfwWindowShouldClose(window))
    {
        
        //Car front facing vector
        vec3 CarDir = vec3(cosf(radians(-(c[0].get_Orientation()))), -0.5f,sinf(radians(-(c[0].get_Orientation()))));
        
        //Sets first person or third person view of car
        if (view == carView::def) {
            
            mat4 viewMatrix_default = glm::mat4(1.0f);
            viewMatrix_default = glm::lookAt(cameraPosition,cameraPosition + cameraLookAt,cameraUp);
            setViewMatrix(shaderProgram, viewMatrix_default);
            setViewMatrix(textShaderProgram, viewMatrix_default);
            setViewMatrix(lightShaderProgram, viewMatrix_default);
            setViewMatrix(gridShaderProgram, viewMatrix_default);
            setViewMatrix(gridTextShaderProgram, viewMatrix_default);
        }
        
        else if (view == carView::first)
        {
        
            mat4 viewMatrix = lookAt(c[0].GetThirdPersonPosition(),c[0].GetThirdPersonPosition() + CarDir, cameraUp);
            setViewMatrix(shaderProgram, viewMatrix);
            setViewMatrix(textShaderProgram, viewMatrix);
            setViewMatrix(lightShaderProgram, viewMatrix);
            setViewMatrix(gridShaderProgram, viewMatrix);
            setViewMatrix(gridTextShaderProgram, viewMatrix);
         }
        
        else if (view == carView::third){
            
            mat4 viewMatrix = lookAt(c[0].GetFirstPersonPosition(),c[0].GetFirstPersonPosition() + CarDir, cameraUp);
            setViewMatrix(shaderProgram, viewMatrix);
            setViewMatrix(textShaderProgram, viewMatrix);
            setViewMatrix(lightShaderProgram, viewMatrix);
            setViewMatrix(gridShaderProgram, viewMatrix);
            setViewMatrix(gridTextShaderProgram, viewMatrix);
        }
        
        
        
        // Each frame, reset color of each pixel to glClearColor
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
        
        float dt = glfwGetTime() - lastFrameTime; //delta time
        lastFrameTime += dt;
        
        glm::mat4 worldMatrix = glm::mat4(1.0f);
        setWorldMatrix(gridTextShaderProgram, worldMatrix);
        setWorldMatrix(simpleDepthShaderProgram, worldMatrix);
        
        angle = (angle - rotationSpeed * dt);
        
        //-------------------------------------------------------
        
        
        //mat4 lightProjectionMatrix = perspective(radians(100.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, 0.1f, 150.0f);
        mat4 lightProjectionMatrix = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 100.f);
        mat4 lightViewMatrix = lookAt(vec3(15.0f, 20.0f, 0.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f, 1.0f, 0.0f));
        mat4 lightSpaceMatrix = lightProjectionMatrix * lightViewMatrix;
        setLightSpaceMatrix(simpleDepthShaderProgram, lightSpaceMatrix);
        setLightSpaceMatrix(gridTextShaderProgram, lightSpaceMatrix);
        
        //glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, &lightSpaceMatrix[0][0]);
        glUseProgram(simpleDepthShaderProgram);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        //glActiveTexture(GL_TEXTURE1);
    
        glCullFace(GL_FRONT);
        glBindVertexArray(vao_grid);
        
        c[0].DrawGrid(simpleDepthShaderProgram);
        for (int i=0; i<7; i++){
        glBindVertexArray(vao_cube);
        c[i].DrawBody(simpleDepthShaderProgram);
        c[i].DrawBonnet(simpleDepthShaderProgram);
        c[i].DrawRoof(simpleDepthShaderProgram);
        c[i].DrawTrunk(simpleDepthShaderProgram);
        c[i].DrawHeadLights(simpleDepthShaderProgram);
        glBindVertexArray(vao_circle);
        c[i].DrawWheels2(simpleDepthShaderProgram, angle);
        }
            //glBindVertexArray(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glCullFace(GL_BACK);
         
        // reset viewport
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        //-----------------------------------------------
        
        //vec3 front = vec3(cosf(c.get_Orientation()), 0.0f, sinf(c.get_Orientation()));
        
        glUseProgram(shaderProgram);
        
        //Axes World Matrix
        
        setWorldMatrix(shaderProgram, worldMatrix);
        setWorldMatrix(gridTextShaderProgram, worldMatrix);
        setWorldMatrix(gridShaderProgram, worldMatrix);
        
        glBindVertexArray(vao_X);
        glDrawArrays(GL_LINES, 0, 2);
        
        glBindVertexArray(vao_Y);
        glDrawArrays(GL_LINES, 0, 2);
        
        glBindVertexArray(vao_Z);
        glDrawArrays(GL_LINES, 0, 2);
        
        if (gridTexture == false){
            
            glBindVertexArray(vao_grid_x);
            c[0].DrawGrid_x(gridShaderProgram);
            
            glBindVertexArray(vao_grid_z);
            c[0].DrawGrid_z(gridShaderProgram);
        
        }
        else if (gridTexture == true){
            
            glUseProgram(gridTextShaderProgram);
            
            //l.SetObjectColor(gridTextShaderProgram, objectColor);
            l.SetCameraPos(gridTextShaderProgram, cameraPosition);
            
            l.SetLampAmbience(gridTextShaderProgram, ambience);
            l.SetLampDiffuse(gridTextShaderProgram, diffuse);
            l.SetLampSpecular(gridTextShaderProgram, specular);
            l.SetLampColor(gridTextShaderProgram, lightColor);
            l.SetLampPos(gridTextShaderProgram, LampPos);
            
            l.SetLeftAmbience(gridTextShaderProgram, ambience);
            l.SetLeftDiffuse(gridTextShaderProgram, diffuse);
            l.SetLeftSpecular(gridTextShaderProgram, specular);
            l.SetLeftColor(gridTextShaderProgram, lightColor);
            l.SetLeftPos(gridTextShaderProgram, leftLightPos);
            
            l.SetRightAmbience(gridTextShaderProgram, ambience);
            l.SetRightDiffuse(gridTextShaderProgram, diffuse);
            l.SetRightSpecular(gridTextShaderProgram, specular);
            l.SetRightColor(gridTextShaderProgram, lightColor);
            l.SetRightPos(gridTextShaderProgram, rightLightPos);
            
            l.SetRightTailAmbience(gridTextShaderProgram, Tailambience);
            l.SetRightTailDiffuse(gridTextShaderProgram, diffuse);
            l.SetRightTailSpecular(gridTextShaderProgram, specular);
            l.SetRightTailColor(gridTextShaderProgram, TailightColor);
            l.SetRightTailPos(gridTextShaderProgram, rightTailightPos);
            
            l.SetLeftTailAmbience(gridTextShaderProgram, Tailambience);
            l.SetLeftTailDiffuse(gridTextShaderProgram, diffuse);
            l.SetLeftTailSpecular(gridTextShaderProgram, specular);
            l.SetLeftTailColor(gridTextShaderProgram, TailightColor);
            l.SetLeftTailPos(gridTextShaderProgram, leftTailightPos);
            
            
            glActiveTexture(GL_TEXTURE0);
            GLuint textureLocation = glGetUniformLocation(gridTextShaderProgram, "textureSampler");
            glBindTexture(GL_TEXTURE_2D, grassTextureID);
            glUniform1i(textureLocation, 0);
            
            glActiveTexture(GL_TEXTURE1);
            textureLocation = glGetUniformLocation(gridTextShaderProgram, "shadowMap");
            glBindTexture(GL_TEXTURE_2D, depthMap);
            glUniform1i(textureLocation, 1);
            //setShadowMapTexture(gridTextShaderProgram, 0);
            
            glBindVertexArray(vao_grid);
            c[0].DrawGrid(gridTextShaderProgram);
        
        }
        
        if (carTexture == false){
    
            carshader = shaderProgram;
        }
        else if (carTexture ==true){
     
            carshader = textShaderProgram;
        }
        
        glUseProgram(carshader);
        glBindVertexArray(vao_cube);
        
        l.SetCameraPos(textShaderProgram, cameraPosition);
        l.SetLampAmbience(textShaderProgram, ambience);
        l.SetLampDiffuse(textShaderProgram, diffuse);
        l.SetLampSpecular(textShaderProgram, specular);
        l.SetLampColor(textShaderProgram, lightColor);
        l.SetLampPos(textShaderProgram, LampPos);
        
        
        //Draw car's parts
        //glUseProgram(simpleDepthShaderProgram);
        for(int i=0; i<8; i++){

        glUseProgram(carshader);
        glActiveTexture(GL_TEXTURE0);
        GLuint textureLocation = glGetUniformLocation(textShaderProgram, "textureSampler");
        glBindTexture(GL_TEXTURE_2D, metalTextureID);
        glUniform1i(textureLocation, 0);
        glBindVertexArray(vao_cube);
            
        //need to cal shader
        c[i].DrawBody(carshader);
        c[i].DrawBonnet(carshader);
        c[i].DrawRoof(carshader);
        c[i].DrawTrunk(carshader);
        

        //Draw wheels
        glBindVertexArray(vao_circle);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tireTextureID);
        glUniform1i(textureLocation, 0);
        c[i].DrawWheels2(carshader, angle);
    
        
        //Drawing Light Source
        glBindVertexArray(vao_light);
        glUseProgram(lightShaderProgram);
        
        //Drawing HeadLights
        c[i].DrawHeadLights(lightShaderProgram);
        
        c[0].SetBoxParts();
        c[1].SetBoxParts();
        col.CheckCollision(c[0], c[1]);
        
        
        //Drawing Smoke
        }
        
        l.DrawLight(lightShaderProgram);
        
        // End Frame
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        // Handle inputs -----------------------------------------------------------
        
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        
        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) // move camera to the left
        {
            cameraPosition.x -= 10 * dt;
            viewMatrix_default = glm::lookAt(cameraPosition,cameraPosition + cameraLookAt,cameraUp);
            
            setViewMatrix(shaderProgram, viewMatrix_default);
            setViewMatrix(textShaderProgram, viewMatrix_default);
            setViewMatrix(lightShaderProgram, viewMatrix_default);
            setViewMatrix(gridShaderProgram, viewMatrix_default);
            setViewMatrix(gridTextShaderProgram, viewMatrix_default);
        }
        
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) // move camera to the right
        {
            cameraPosition.x += 10 * dt;
            //viewMatrix = glm::lookAt(cameraPosition,cameraPosition + cameraLookAt,cameraUp);
            
            setViewMatrix(shaderProgram, viewMatrix_default);
            setViewMatrix(textShaderProgram, viewMatrix_default);
            setViewMatrix(lightShaderProgram, viewMatrix_default);
            setViewMatrix(gridShaderProgram, viewMatrix_default);
            setViewMatrix(gridTextShaderProgram, viewMatrix_default);
        }
        
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) // move camera up
        {
            cameraPosition.y -= 10 * dt;
            //viewMatrix = glm::lookAt(cameraPosition,cameraPosition + cameraLookAt,cameraUp);
           
            setViewMatrix(shaderProgram, viewMatrix_default);
            setViewMatrix(textShaderProgram, viewMatrix_default);
            setViewMatrix(lightShaderProgram, viewMatrix_default);
            setViewMatrix(gridShaderProgram, viewMatrix_default);
            setViewMatrix(gridTextShaderProgram, viewMatrix_default);
        }
        
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) // move camera down
        {
            cameraPosition.y += 10 * dt;
            //viewMatrix = glm::lookAt(cameraPosition,cameraPosition + cameraLookAt,cameraUp);
            setViewMatrix(shaderProgram, viewMatrix_default);
            setViewMatrix(textShaderProgram, viewMatrix_default);
            setViewMatrix(lightShaderProgram, viewMatrix_default);
            setViewMatrix(gridShaderProgram, viewMatrix_default);
            setViewMatrix(gridTextShaderProgram, viewMatrix_default);
        }
        
        //Random Location on grid
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
            
            int x=((rand() % 50)+1)-((rand() % 50)+1);
            int z=((rand() % 50)+1)-((rand() % 50)+1);
            c[0].SetPositions(x, 0, z);
        }
        
        //ResetPosition & world orientation
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS){
        
            c[0].SetPositions(0, 0, 0);
            c[0].SetOrientationAngle(0);
            mat4 rotationMatrix = mat4(1.0f);
            
            glUseProgram(shaderProgram);
            GLuint rotationMatrixLocation = glGetUniformLocation(shaderProgram, "rotationMatrix_x");
            glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, &rotationMatrix[0][0]);
            rotationMatrixLocation = glGetUniformLocation(shaderProgram, "rotationMatrix_y");
            glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, &rotationMatrix[0][0]);
            
            glUseProgram(textShaderProgram);
            rotationMatrixLocation = glGetUniformLocation(textShaderProgram, "rotationMatrix_x");
            glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, &rotationMatrix[0][0]);
            rotationMatrixLocation = glGetUniformLocation(textShaderProgram, "rotationMatrix_y");
            glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, &rotationMatrix[0][0]);
            
            glUseProgram(lightShaderProgram);
            rotationMatrixLocation = glGetUniformLocation(lightShaderProgram, "rotationMatrix_x");
            glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, &rotationMatrix[0][0]);
            rotationMatrixLocation = glGetUniformLocation(lightShaderProgram, "rotationMatrix_y");
            glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, &rotationMatrix[0][0]);
            
            glUseProgram(gridShaderProgram);
            setRotationMatrix_x(gridShaderProgram,rotationMatrix);
            setRotationMatrix_y(gridShaderProgram,rotationMatrix);
            
            glUseProgram(gridTextShaderProgram);
            setRotationMatrix_x(gridTextShaderProgram,rotationMatrix);
            setRotationMatrix_y(gridTextShaderProgram,rotationMatrix);
            
        }
        
        //Move car pos UP
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            
            int xpos=c[0].get_xPos();
            xpos+=(dt * 55);
            
            c[0].SetPositions(xpos, c[0].get_yPos(), c[0].get_zPos());
            leftLightPos = c[0].GetRightHeadLight();
            rightLightPos = c[0].GetLeftHeadLight();
            leftTailightPos = c[0].GetLeftTailight();
            rightTailightPos = c[0].GetRightTailight();
        }
        
        //Move car pos DOWN
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            
            int xpos=c[0].get_xPos();
            xpos-=(dt * 55) + 1;
            c[0].SetPositions(xpos, c[0].get_yPos(), c[0].get_zPos());
            leftLightPos = c[0].GetRightHeadLight();
            rightLightPos = c[0].GetLeftHeadLight();
            leftTailightPos = c[0].GetLeftTailight();
            rightTailightPos = c[0].GetRightTailight();
        }
        
        //Move car pos RIGHT
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            
            int zpos=c[0].get_zPos();
            zpos+=(dt * 55) + 1;
            c[0].SetPositions(c[0].get_xPos(), c[0].get_yPos(), zpos);
            leftLightPos = c[0].GetRightHeadLight();
            rightLightPos = c[0].GetLeftHeadLight();
            leftTailightPos = c[0].GetLeftTailight();
            rightTailightPos = c[0].GetRightTailight();
        }
        
        //Move car pos LEFT
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            
            int zpos=c[0].get_zPos();
            zpos-=(dt * 55) + 1 ;
            c[0].SetPositions(c[0].get_xPos(), c[0].get_yPos(), zpos);
            leftLightPos = c[0].GetRightHeadLight();
            rightLightPos = c[0].GetLeftHeadLight();
            leftTailightPos = c[0].GetLeftTailight();
            rightTailightPos = c[0].GetRightTailight();
        }
        
        //Changes Rendering mode to POINTS
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){
            
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        
        }
        
        //Changes Rendering mode to LINES
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
            
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            
        }
        
        //Changes Rendering mode to TRIANGLES
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS){
            
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        
        //Scale-UP
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS){
            
            float scale=c[0].get_Scale();
           
            scale+=dt * 20;
            
            c[0].SetScale(scale);
            
            leftLightPos = c[0].GetRightHeadLight();
            rightLightPos = c[0].GetLeftHeadLight();
            leftTailightPos = c[0].GetLeftTailight();
            rightTailightPos = c[0].GetRightTailight();
            
        }
        
        //Scale down
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
            
            float scale=c[0].get_Scale();
            
            scale-=dt * 20;
            
            c[0].SetScale(scale);
            leftLightPos = c[0].GetRightHeadLight();
            rightLightPos = c[0].GetLeftHeadLight();
            leftTailightPos = c[0].GetLeftTailight();
            rightTailightPos = c[0].GetRightTailight();
            
        }
        
        //change cars orientation
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
            
            float orientangle=c[0].get_Orientation();
            orientangle+=dt * 20;
            c[0].SetOrientationAngle(orientangle);
            leftLightPos = c[0].GetRightHeadLight();
            rightLightPos = c[0].GetLeftHeadLight();
            leftTailightPos = c[0].GetLeftTailight();
            rightTailightPos = c[0].GetRightTailight();
    
        }
        
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
            
            float orientangle=c[0].get_Orientation();
            orientangle-=dt * 20;
            c[0].SetOrientationAngle(orientangle);
            leftLightPos = c[0].GetRightHeadLight();
            rightLightPos = c[0].GetLeftHeadLight();
            leftTailightPos = c[0].GetLeftTailight();
            rightTailightPos = c[0].GetRightTailight();
        }
        
        //counter clockwise rotation around x axis
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
            
            worldAngle_x-=dt * 45;
            glm::mat4 rotationMatrix = glm::rotate(mat4(1.0f), glm::radians(worldAngle_x), vec3(1.0f, 0.0f, 0.0f));
            glUseProgram(shaderProgram);
            GLuint rotationMatrixLocation = glGetUniformLocation(shaderProgram, "rotationMatrix_x");
            glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, &rotationMatrix[0][0]);
            
            glUseProgram(textShaderProgram);
            rotationMatrixLocation = glGetUniformLocation(textShaderProgram, "rotationMatrix_x");
            glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, &rotationMatrix[0][0]);
            
            glUseProgram(lightShaderProgram);
            rotationMatrixLocation = glGetUniformLocation(lightShaderProgram, "rotationMatrix_x");
            glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, &rotationMatrix[0][0]);
            
            glUseProgram(gridShaderProgram);
            setRotationMatrix_x(gridShaderProgram, rotationMatrix);
            
            glUseProgram(gridTextShaderProgram);
            setRotationMatrix_x(gridTextShaderProgram, rotationMatrix);
            
        }
        
        //clockwise rotation around x axis
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
            
            worldAngle_x+=dt * 45;
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(worldAngle_x), glm::vec3(1.0f, 0.0f, 0.0f));
            glUseProgram(shaderProgram);
            GLuint rotationMatrixLocation = glGetUniformLocation(shaderProgram, "rotationMatrix_x");
            glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, &rotationMatrix[0][0]);
            
            glUseProgram(textShaderProgram);
            rotationMatrixLocation = glGetUniformLocation(textShaderProgram, "rotationMatrix_x");
            glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, &rotationMatrix[0][0]);
            
            glUseProgram(lightShaderProgram);
            rotationMatrixLocation = glGetUniformLocation(lightShaderProgram, "rotationMatrix_x");
            glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, &rotationMatrix[0][0]);
            
            glUseProgram(gridShaderProgram);
            setRotationMatrix_x(gridShaderProgram, rotationMatrix);
            
            glUseProgram(gridTextShaderProgram);
            setRotationMatrix_x(gridTextShaderProgram, rotationMatrix);
        }
        
        
        //Tilts camera
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
            
            double mousePosX, mousePosY;
            glfwGetCursorPos(window, &mousePosX, &mousePosY);
            
            double dy = mousePosY - lastMousePosY;
            
            lastMousePosX = mousePosX;
            lastMousePosY = mousePosY;
            
            
            //Convert to spherical coordinates
            const float cameraAngularSpeed = 60.0f;
            //cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
            cameraVerticalAngle   -= dy * cameraAngularSpeed * dt;
            
            // Clamp vertical angle to [-85, 85] degrees
            cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));
            
            if (cameraHorizontalAngle > 360)
            {
                cameraHorizontalAngle -= 360;
            }
            else if (cameraHorizontalAngle < -360)
            {
                cameraHorizontalAngle += 360;
            }
            
            float theta = glm::radians(cameraHorizontalAngle);
            float phi = glm::radians(cameraVerticalAngle);
            
            cameraLookAt = glm::vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
            glm::vec3 cameraSideVector = glm::cross(cameraLookAt, glm::vec3(0.0f, 1.0f, 0.0f));
            
            glm::normalize(cameraSideVector);
            
            mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt  , cameraUp);
            glUseProgram(shaderProgram);
            setViewMatrix(shaderProgram, viewMatrix);
            glUseProgram(textShaderProgram);
            setViewMatrix(textShaderProgram, viewMatrix);
            glUseProgram(lightShaderProgram);
            setViewMatrix(lightShaderProgram, viewMatrix);
            glUseProgram(gridShaderProgram);
            setViewMatrix(gridShaderProgram, viewMatrix);
            glUseProgram(gridTextShaderProgram);
            setViewMatrix(gridTextShaderProgram, viewMatrix);
        }
        
        //counter clockwise rotation around y axis
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
            
            worldAngle_y-=dt * 45;
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(worldAngle_y), glm::vec3(0.0f, 1.0f, 0.0f));
            glUseProgram(shaderProgram);
            GLuint rotationMatrixLocation = glGetUniformLocation(shaderProgram, "rotationMatrix_y");
            glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, &rotationMatrix[0][0]);
            
            glUseProgram(textShaderProgram);
            rotationMatrixLocation = glGetUniformLocation(textShaderProgram, "rotationMatrix_y");
            glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, &rotationMatrix[0][0]);
            
            glUseProgram(lightShaderProgram);
            rotationMatrixLocation = glGetUniformLocation(lightShaderProgram, "rotationMatrix_y");
            glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, &rotationMatrix[0][0]);
            
            glUseProgram(gridShaderProgram);
            setRotationMatrix_y(gridShaderProgram, rotationMatrix);
            
            glUseProgram(gridTextShaderProgram);
            setRotationMatrix_y(gridTextShaderProgram, rotationMatrix);
        }
        
        //clockwise rotation around y axis
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
            
            worldAngle_y+=dt * 45;
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(worldAngle_y), glm::vec3(0.0f, 1.0f, 0.0f));
            glUseProgram(shaderProgram);
            GLuint rotationMatrixLocation = glGetUniformLocation(shaderProgram, "rotationMatrix_y");
            glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, &rotationMatrix[0][0]);
            
            glUseProgram(textShaderProgram);
            rotationMatrixLocation = glGetUniformLocation(textShaderProgram, "rotationMatrix_y");
            glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, &rotationMatrix[0][0]);
            
            glUseProgram(lightShaderProgram);
            setRotationMatrix_y(lightShaderProgram, rotationMatrix);
            
            glUseProgram(gridShaderProgram);
            setRotationMatrix_y(gridShaderProgram, rotationMatrix);
            
            glUseProgram(gridTextShaderProgram);
            setRotationMatrix_y(gridTextShaderProgram, rotationMatrix);
        }
        
        //Zoom in and out
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS){
            
            double mousePosX, mousePosY;
            glfwGetCursorPos(window, &mousePosX, &mousePosY);
            
            lastMousePosX = mousePosX;
            lastMousePosY = mousePosY;
            
            fov += 0.005 * mousePosX;
            
            glm::mat4 projectionMatrix = glm::perspective(fov, (float) SCR_WIDTH/ (float)SCR_HEIGHT, 0.1f, 100.0f);
            GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
            glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
            projectionMatrixLocation = glGetUniformLocation(textShaderProgram, "projectionMatrix");
            glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        }
        
        //Pans camera
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
            
            double mousePosX, mousePosY;
            glfwGetCursorPos(window, &mousePosX, &mousePosY);
            lastMousePosX = mousePosX;
            lastMousePosY = mousePosY;
        }
        
        
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
        
            double mousePosX, mousePosY;
            glfwGetCursorPos(window, &mousePosX, &mousePosY);
            
            double dx = mousePosX - lastMousePosX;
            
            lastMousePosX = mousePosX;
            lastMousePosY = mousePosY;
        
            
            //Convert to spherical coordinates
            const float cameraAngularSpeed = 60.0f;
            cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
            //cameraVerticalAngle   -= dy * cameraAngularSpeed * dt;
            
            // Clamp vertical angle to [-85, 85] degrees
            cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));
            
            if (cameraHorizontalAngle > 360)
            {
                cameraHorizontalAngle -= 360;
            }
            else if (cameraHorizontalAngle < -360)
            {
                cameraHorizontalAngle += 360;
            }
            
            float theta = glm::radians(cameraHorizontalAngle);
            float phi = glm::radians(cameraVerticalAngle);
            
            cameraLookAt = glm::vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
            glm::vec3 cameraSideVector = glm::cross(cameraLookAt, glm::vec3(0.0f, 1.0f, 0.0f));
            
            glm::normalize(cameraSideVector);
        
            mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt  , cameraUp);
            glUseProgram(shaderProgram);
            setViewMatrix(shaderProgram, viewMatrix);
            
            glUseProgram(textShaderProgram);
            setViewMatrix(textShaderProgram, viewMatrix);
            
            glUseProgram(lightShaderProgram);
            setViewMatrix(lightShaderProgram, viewMatrix);
            
            glUseProgram(gridShaderProgram);
            setViewMatrix(gridShaderProgram, viewMatrix);
            
            glUseProgram(gridTextShaderProgram);
            setViewMatrix(gridTextShaderProgram, viewMatrix);
            
        }
        
        //Asg 2 functions ----------------------------------
        
        //toggles between Car shaders (Texture+Lighting or Regular)
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS){
            
            if (carTexture == false){
            
                carTexture = true;
            }
            else{
                carTexture = false;
            }
         }
        
        //toggles between grid shaders (Texture+Lighting or Regular)
        if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS){
             
             if (gridTexture == false){
                 
                 gridTexture = true;
             }
             else{
                 gridTexture = false;
             }
         }
        
        //Turns Wheels Right
        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS){
            
            float turnangle=c[0].get_Turn();
            
            turnangle+=dt * 30;
            
            c[0].SetTurnAngle(turnangle);
            
        }
        
        //Turn Wheels Left
        if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS){
            
            float turnangle=c[0].get_Turn();
            
            turnangle-=dt * 30;
            
            c[0].SetTurnAngle(turnangle);
            
        }
        
        //Turn car based on wheel position
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS){
            
            float orientation = c[0].get_Orientation();
            float turnangle = c[0].get_Turn();
            
            if (turnangle==0){
                
                float dirx = c[0].getFront().x;
                float dirz = c[0].getFront().z;
                
                int xpos= c[0].get_xPos();
                xpos += (dirx * 5);
                
                int zpos = c[0].get_zPos();
                zpos += (dirz * 5);
                
                c[0].SetPositions(xpos, c[0].get_yPos(), zpos);
            }
            
            else if(turnangle < 0.0f) {
                
                orientation -= dt * abs(turnangle) * 5;
    
                float dirx = c[0].getFront().x;
                float dirz = c[0].getFront().z;
                
                int xpos= c[0].get_xPos();
                xpos += (dirx * 2);
                
                int zpos = c[0].get_zPos();
                zpos += (dirz * 2);
                
                c[0].SetPositions(xpos, c[0].get_yPos(), zpos);
        
                c[0].SetOrientationAngle(orientation);
                
            }
            else if(turnangle > 0.0f){
                
                orientation += dt * abs(turnangle) * 5;
                
                float dirx = c[0].getFront().x;
                float dirz = c[0].getFront().z;
                
                int xpos= c[0].get_xPos();
                xpos += (dirx * 2);
                
                int zpos = c[0].get_zPos();
                zpos += (dirz * 2);
                
                c[0].SetPositions(xpos, c[0].get_yPos(), zpos);
                
                c[0].SetOrientationAngle(orientation);
            
            }
            
            leftLightPos = c[0].GetRightHeadLight();
            rightLightPos = c[0].GetLeftHeadLight();
            leftTailightPos = c[0].GetLeftTailight();
            rightTailightPos = c[0].GetRightTailight();
            
        }
        //Turn car based on wheel position in reverse
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
            
            float orientation = c[0].get_Orientation();
            float turnangle = c[0].get_Turn();
            
            if (turnangle==0){
                
                float dirx = c[0].getFront().x;
                float dirz = c[0].getFront().z;
                
                int xpos= c[0].get_xPos();
                xpos -= (dirx * 5);
                
                int zpos = c[0].get_zPos();
                zpos -= (dirz * 5);
                
                c[0].SetPositions(xpos, c[0].get_yPos(), zpos);
                
            }
            
            else if(turnangle < 0.0f) {
                
                orientation += dt * abs(turnangle) * 5;
                
                float dirx = c[0].getFront().x;
                float dirz = c[0].getFront().z;
                
                int xpos= c[0].get_xPos();
                xpos -= (dirx * 2);
                
                int zpos = c[0].get_zPos();
                zpos -= (dirz * 2);
                
                c[0].SetPositions(xpos, c[0].get_yPos(), zpos);
                
                c[0].SetOrientationAngle(orientation);
                
            }
            else if(turnangle > 0.0f){
                
                orientation -= dt * abs(turnangle) * 5;
                
                float dirx = c[0].getFront().x;
                float dirz = c[0].getFront().z;
                
                int xpos= c[0].get_xPos();
                xpos -= (dirx * 2);
                
                int zpos = c[0].get_zPos();
                zpos -= (dirz * 2);
                
                c[0].SetPositions(xpos, c[0].get_yPos(), zpos);
                
                c[0].SetOrientationAngle(orientation);
                
            }
            
            leftLightPos = c[0].GetRightHeadLight();
            rightLightPos = c[0].GetLeftHeadLight();
            leftTailightPos = c[0].GetLeftTailight();
            rightTailightPos = c[0].GetRightTailight();
            
        }
        
        //Change between first person and third person camera
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
            
            if (view == carView::first){
                
                view = carView::third;
            }
            else if (view == carView::third){
                
                view = carView::def;
            }
            else if (view == carView::def){
                
                view = carView::first;
            }
            
        }
        
       if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
           
           //for (int i=0; i<6; i++){
               
               //c[i].MoveRandom();
           
        
       }
        
    }
    
    // Shutdown GLFW
    glfwTerminate();
    
    return 0;

}


/*
#include <iostream>
#include <list>
#include <algorithm>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>

#include <FreeImageIO.h>


using namespace glm;
using namespace std;

class Projectile
{
public:
    Projectile(vec3 position, vec3 velocity, int shaderProgram) : mPosition(position), mVelocity(velocity)
    {
        mWorldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    }
    
    void Update(float dt)
    {
        mPosition += mVelocity * dt;
    }
    
    void Draw() {
        // this is a bit of a shortcut, since we have a single vbo, it is already bound
        // let's just set the world matrix in the vertex shader
        
        mat4 worldMatrix = translate(mat4(1.0f), mPosition) * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.2f));
        glUniformMatrix4fv(mWorldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    
private:
    GLuint mWorldMatrixLocation;
    vec3 mPosition;
    vec3 mVelocity;
};

const char* getVertexShaderSource()
{
    // For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
    return
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;"
    "layout (location = 1) in vec3 aColor;"
    ""
    "uniform mat4 worldMatrix;"
    "uniform mat4 viewMatrix = mat4(1.0);"  // default value for view matrix (identity)
    "uniform mat4 projectionMatrix = mat4(1.0);"
    ""
    "out vec3 vertexColor;"
    "void main()"
    "{"
    "   vertexColor = aColor;"
    "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
    "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
    "}";
}


const char* getFragmentShaderSource()
{
    return
    "#version 330 core\n"
    "in vec3 vertexColor;"
    "out vec4 FragColor;"
    "void main()"
    "{"
    "   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
    "}";
}

const char* getTexturedVertexShaderSource()
{
    // For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
    return
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;"
    "layout (location = 1) in vec3 aColor;"
    "layout (location = 2) in vec2 aUV;"
    ""
    "uniform mat4 worldMatrix;"
    "uniform mat4 viewMatrix = mat4(1.0);"  // default value for view matrix (identity)
    "uniform mat4 projectionMatrix = mat4(1.0);"
    ""
    "out vec3 vertexColor;"
    "out vec2 vertexUV;"
    ""
    "void main()"
    "{"
    "   vertexColor = aColor;"
    "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
    "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
    "   vertexUV = aUV;"
    "}";
}

const char* getTexturedFragmentShaderSource()
{
    return
    "#version 330 core\n"
    "in vec3 vertexColor;"
    "in vec2 vertexUV;"
    "uniform sampler2D textureSampler;"
    ""
    "out vec4 FragColor;"
    "void main()"
    "{"
    "   vec4 textureColor = texture( textureSampler, vertexUV );"
    "   FragColor = textureColor * vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
    "}";
}

int compileAndLinkShaders(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    // compile and link shader program
    // return shader program id
    // ------------------------------------
    
    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}

struct TexturedColoredVertex
{
    TexturedColoredVertex(vec3 _position, vec3 _color, vec2 _uv)
    : position(_position), color(_color), uv(_uv) {}
    
    vec3 position;
    vec3 color;
    vec2 uv;
};

// Textured Cube model
const TexturedColoredVertex texturedCubeVertexArray[] = {  // position,                            color
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)), //left - red
    TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)),
    
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f)),
    
    TexturedColoredVertex(vec3( 0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)), // far - blue
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f)),
    
    TexturedColoredVertex(vec3( 0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3( 0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
    
    TexturedColoredVertex(vec3( 0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)), // bottom - turquoise
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3( 0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),
    
    TexturedColoredVertex(vec3( 0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
    
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)), // near - green
    TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3( 0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)),
    
    TexturedColoredVertex(vec3( 0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)),
    TexturedColoredVertex(vec3( 0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)),
    
    TexturedColoredVertex(vec3( 0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)), // right - purple
    TexturedColoredVertex(vec3( 0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3( 0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)),
    
    TexturedColoredVertex(vec3( 0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3( 0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3( 0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f)),
    
    TexturedColoredVertex(vec3( 0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)), // top - yellow
    TexturedColoredVertex(vec3( 0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),
    
    TexturedColoredVertex(vec3( 0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f))
};

int createTexturedCubeVertexBufferObject()
{
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    
    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCubeVertexArray), texturedCubeVertexArray, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
                          3,                   // size
                          GL_FLOAT,            // type
                          GL_FALSE,            // normalized?
                          sizeof(TexturedColoredVertex), // stride - each vertex contain 2 vec3 (position, color)
                          (void*)0             // array buffer offset
                          );
    glEnableVertexAttribArray(0);
    
    
    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(TexturedColoredVertex),
                          (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
                          );
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2,                            // attribute 2 matches aUV in Vertex Shader
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(TexturedColoredVertex),
                          (void*)(2*sizeof(vec3))      // uv is offseted by 2 vec3 (comes after position and color)
                          );
    glEnableVertexAttribArray(2);
    
    return vertexBufferObject;
}

void setProjectionMatrix(int shaderProgram, mat4 projectionMatrix)
{
    glUseProgram(shaderProgram);
    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void setViewMatrix(int shaderProgram, mat4 viewMatrix)
{
    glUseProgram(shaderProgram);
    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}

void setWorldMatrix(int shaderProgram, mat4 worldMatrix)
{
    glUseProgram(shaderProgram);
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}

int loadTexture(char* imagepath)
{
    // Load image using the Free Image library
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(imagepath, 0);
    FIBITMAP* image = FreeImage_Load(format, imagepath);
    FIBITMAP* image32bits = FreeImage_ConvertTo32Bits(image);
    
    // Get an available texture index from OpenGL
    GLuint texture = 0;
    glGenTextures(1, &texture);
    assert(texture != 0);
    
    // Set OpenGL filtering properties (bi-linear interpolation)
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Retrieve width and hight
    int width = FreeImage_GetWidth(image32bits);
    int height = FreeImage_GetHeight(image32bits);
    
    // This will upload the texture to the GPU memory
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height,
                 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image32bits));
    
    // Free images
    FreeImage_Unload(image);
    FreeImage_Unload(image32bits);
    
    return texture;
}


int main(int argc, char*argv[])
{
    // Initialize GLFW and OpenGL version
    glfwInit();
    
#if defined(PLATFORM_OSX)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    // On windows, we set OpenGL version to 2.1, to support more hardware
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif
    
    // Create Window and rendering context using GLFW, resolution is 800x600
    GLFWwindow* window = glfwCreateWindow(800, 600, "Comp371 - Lab 03", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(window);
    
    
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    
    // Load Textures
#if defined(PLATFORM_OSX)
    GLuint brickTextureID = loadTexture((char*)"/Users/gianlucalepore/Documents/COMP371/grasstexture.jpg");
    GLuint cementTextureID = loadTexture((char*)"/Users/gianlucalepore/Documents/ COMP371/Lab01/Lab_Framework/Xcode/Assets/Textures/grasstexture.jpg");
#else
    GLuint brickTextureID = loadTexture("../Assets/Textures/brick.jpg");
    GLuint cementTextureID = loadTexture("../Assets/Textures/cement.jpg");
#endif
    
    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Compile and link shaders here ...
    int colorShaderProgram = compileAndLinkShaders(getVertexShaderSource(), getFragmentShaderSource());
    int texturedShaderProgram = compileAndLinkShaders(getTexturedVertexShaderSource(), getTexturedFragmentShaderSource());
    
    // Camera parameters for view transform
    vec3 cameraPosition(0.6f,1.0f,10.0f);
    vec3 cameraLookAt(0.0f, 0.0f, -1.0f);
    vec3 cameraUp(0.0f, 1.0f, 0.0f);
    
    // Other camera parameters
    float cameraSpeed = 1.0f;
    float cameraFastSpeed = 2 * cameraSpeed;
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;
    bool  cameraFirstPerson = true; // press 1 or 2 to toggle this variable
    
    // Spinning cube at camera position
    float spinningCubeAngle = 0.0f;
    
    // Set projection matrix for shader, this won't change
    mat4 projectionMatrix = glm::perspective(70.0f,            // field of view in degrees
                                             800.0f / 600.0f,  // aspect ratio
                                             0.01f, 100.0f);   // near and far (near > 0)
    
    // Set initial view matrix
    mat4 viewMatrix = lookAt(cameraPosition,  // eye
                             cameraPosition + cameraLookAt,  // center
                             cameraUp ); // up
    
    // Set View and Projection matrices on both shaders
    setViewMatrix(colorShaderProgram, viewMatrix);
    setViewMatrix(texturedShaderProgram, viewMatrix);
    
    setProjectionMatrix(colorShaderProgram, projectionMatrix);
    setProjectionMatrix(texturedShaderProgram, projectionMatrix);
    
    // Define and upload geometry to the GPU here ...
    int texturedCubeVBO = createTexturedCubeVertexBufferObject();
    
    // For frame time
    float lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
    
    // Other OpenGL states to set once
    // Enable Backface culling
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    
    // Container for projectiles to be implemented in tutorial
    list<Projectile> projectileList;
    
    glBindBuffer(GL_ARRAY_BUFFER, texturedCubeVBO);
    
    // Entering Main Loop
    while(!glfwWindowShouldClose(window))
    {
        // Frame time calculation
        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;
        
        // Each frame, reset color of each pixel to glClearColor
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Draw textured geometry
        glUseProgram(texturedShaderProgram);
        
        glActiveTexture(GL_TEXTURE0);
        GLuint textureLocation = glGetUniformLocation(texturedShaderProgram, "textureSampler");
        glBindTexture(GL_TEXTURE_2D, brickTextureID);
        glUniform1i(textureLocation, 0);                // Set our Texture sampler to user Texture Unit 0
        
        // Draw ground
        mat4 groundWorldMatrix = translate(mat4(1.0f), vec3(0.0f, -0.01f, 0.0f)) * scale(mat4(1.0f), vec3(1000.0f, 0.02f, 1000.0f));
        setWorldMatrix(texturedShaderProgram, groundWorldMatrix);
        
        glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0
        
        // Draw pillars
        glBindTexture(GL_TEXTURE_2D, cementTextureID);
        mat4 pillarWorldMatrix = translate(mat4(1.0f), vec3(0.0f, 10.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 20.0f, 2.0f));
        setWorldMatrix(texturedShaderProgram, pillarWorldMatrix);
        
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        for (int i=0; i<20; ++i)
        {
            for (int j=0; j<20; ++j)
            {
                // FIXME: it would be more efficient to set the cement texture and draw all pillars, then switch to brick texture and draw all pillar bases
                // use cement texture for pillar
                glBindTexture(GL_TEXTURE_2D, cementTextureID);
                pillarWorldMatrix = translate(mat4(1.0f), vec3(- 100.0f + i * 10.0f, 5.0f, -100.0f + j * 10.0f)) * scale(mat4(1.0f), vec3(1.0f, 10.0f, 1.0f));
                setWorldMatrix(texturedShaderProgram, pillarWorldMatrix);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                
                // use brick texture for base
                glBindTexture(GL_TEXTURE_2D, brickTextureID);
                pillarWorldMatrix = translate(mat4(1.0f), vec3(- 100.0f + i * 10.0f, 0.55f, -100.0f + j * 10.0f)) * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.1f, 1.1f, 1.1f));
                setWorldMatrix(texturedShaderProgram, pillarWorldMatrix);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
        
        // Draw colored geometry
        glUseProgram(colorShaderProgram);
        
        // Update and draw projectiles
        for (list<Projectile>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
        {
            it->Update(dt);
            it->Draw();
        }
        
        // Spinning cube at camera position
        spinningCubeAngle += 180.0f * dt;
        
        // Draw avatar in view space for first person camera
        // and in world space for third person camera
        if (cameraFirstPerson)
        {
            // Wolrd matrix is identity, but view transform like a world transform relative to camera basis
            // (1 unit in front of camera)
            //
            // This is similar to a weapon moving with camera in a shooter game
            mat4 spinningCubeViewMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, -1.0f)) *
            rotate(mat4(1.0f), radians(spinningCubeAngle), vec3(0.0f, 1.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(0.01f, 0.01f, 0.01f));
            
            setWorldMatrix(colorShaderProgram, mat4(1.0f));
            setViewMatrix(colorShaderProgram, spinningCubeViewMatrix);
        }
        else
        {
            // In third person view, let's draw the spinning cube in world space, like any other models
            mat4 spinningCubeWorldMatrix = translate(mat4(1.0f), cameraPosition) *
            rotate(mat4(1.0f), radians(spinningCubeAngle), vec3(0.0f, 1.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(0.1f, 0.1f, 0.1f));
            
            setWorldMatrix(colorShaderProgram, spinningCubeWorldMatrix);
        }
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        
        
        // End Frame
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        // Handle inputs
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) // move camera down
        {
            cameraFirstPerson = true;
        }
        
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) // move camera down
        {
            cameraFirstPerson = false;
        }
        
        
        // This was solution for Lab02 - Moving camera exercise
        // We'll change this to be a first or third person camera
        bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
        float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;
        
        
        // - Calculate mouse motion dx and dy
        // - Update camera horizontal and vertical angle
        double mousePosX, mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);
        
        double dx = mousePosX - lastMousePosX;
        double dy = mousePosY - lastMousePosY;
        
        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;
        
        // Convert to spherical coordinates
        const float cameraAngularSpeed = 60.0f;
        cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
        cameraVerticalAngle   -= dy * cameraAngularSpeed * dt;
        
        // Clamp vertical angle to [-85, 85] degrees
        cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));
        
        float theta = radians(cameraHorizontalAngle);
        float phi = radians(cameraVerticalAngle);
        
        cameraLookAt = vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
        vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));
        
        glm::normalize(cameraSideVector);
        
        
        // Use camera lookat and side vectors to update positions with ASDW
        if (glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS)
        {
            cameraPosition += cameraLookAt * dt * currentCameraSpeed;
        }
        
        if (glfwGetKey(window, GLFW_KEY_S ) == GLFW_PRESS)
        {
            cameraPosition -= cameraLookAt * dt * currentCameraSpeed;
        }
        
        if (glfwGetKey(window, GLFW_KEY_D ) == GLFW_PRESS)
        {
            cameraPosition += cameraSideVector * dt * currentCameraSpeed;
        }
        
        if (glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS)
        {
            cameraPosition -= cameraSideVector * dt * currentCameraSpeed;
        }
        
        // Set the view matrix for first and third person cameras
        // - In first person, camera lookat is set like below
        // - In third person, camera position is on a sphere looking towards center
        mat4 viewMatrix(1.0f);
        
        if (cameraFirstPerson)
        {
            viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp );
        }
        else
        {
            // Position of the camera is on the sphere looking at the point of interest (cameraPosition)
            float radius = 5.0f;
            vec3 position = cameraPosition - vec3(radius * cosf(phi)*cosf(theta),
                                                  radius * sinf(phi),
                                                  -radius * cosf(phi)*sinf(theta));
            
            viewMatrix = lookAt(position, cameraPosition, cameraUp);
        }
        
        setViewMatrix(colorShaderProgram, viewMatrix);
        setViewMatrix(texturedShaderProgram, viewMatrix);
        
        
        // Shoot projectiles on mouse left click
        // To detect onPress events, we need to check the last state and the current state to detect the state change
        // Otherwise, you would shoot many projectiles on each mouse press
        if (lastMouseLeftState == GLFW_RELEASE && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            const float projectileSpeed = 25.0f;
            projectileList.push_back(Projectile(cameraPosition, projectileSpeed * cameraLookAt, colorShaderProgram));
        }
        lastMouseLeftState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    }
    
    glfwTerminate();
    
    return 0;
}



const char* getGridTexturedFragmentShaderSource()
{
    return
    "#version 330 core\n"
    "in vec3 vertexColor;"
    "in vec3 normal;"
    "in vec3 fragPos;"
    "in vec2 textUV;"
    ""
    "uniform sampler2D textureSampler;"
    ""
    "uniform vec3 lightPos;"
    "uniform vec3 cameraPos;"
    "uniform vec3 lightColor;"
    "uniform vec3 objectColor;"
    ""
    "out vec4 FragColor;"
    "void main()"
    "{" //ambience
    "   float ambience = 0.1;"
    "   vec3 ambientLight = ambience * lightColor;"
    "" //diffuse
    "   vec3 norm = normalize(normal);"
    "   vec3 lightDir = normalize(lightPos - fragPos);"
    "   float diff = max(dot(norm, lightDir), 0.0);"
    "   vec3 diffuseLight = diff * lightColor;"
    "" //specular
    "   float specular = 0.5;"
    "   vec3 viewDir = normalize(cameraPos - fragPos);"
    "   vec3 reflectDir = reflect(-lightDir, norm);"
    "   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);"
    "   vec3 specularLight = specular * spec * lightColor;"
    ""
    "   vec4 textureColor = texture(textureSampler,textUV);"
    "   vec3 light = (ambientLight + diffuseLight + specularLight);"
    "   FragColor = textureColor * vec4(light, 1.0f);"
    "}";
}
*/
