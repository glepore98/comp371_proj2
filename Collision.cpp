//
//  Collision.cpp
//  COMP371_Labs
//
//  Created by Gianluca Lepore on 2019-11-30.
//  Copyright © 2019 Concordia. All rights reserved.
//

#include "Collision.hpp"
#include "Car.hpp"
#include "Box.hpp"
#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <vector>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices

using namespace glm;

Collision::Collision(){
    
    }

void Collision::CheckCollision(CarParts& car1, CarParts& car2){
    
    //must move both cars' boxParts objects based on inverse transform
    //Gets Box Parts vectors containing
    vector<Box> car1_Boxes = car1.getBoxParts();
    vector<Box> car2_Boxes = car2.getBoxParts();
    
    for (vector<Box>::iterator it1 = car1_Boxes.begin(); it1 != car1_Boxes.end(); ++it1){
        for (vector<Box>::iterator it2 = car2_Boxes.begin(); it2 != car2_Boxes.end(); ++it2){
            
            GetCollision(*it1, *it2);
            
        }
    }
}

vec3 Collision::GetCollision(Box &box1, Box &box2){
    
    vector<vec3> box1_points = box1.GetBoxPoints();
    vector<vec3> box2_points = box2.GetBoxPoints();
    
    mat4 box1_inverse = inverse(box1.getTransform());  //gets the inverse of transform on points vector of the box
    mat4 box2_inverse = inverse(box2.getTransform());
    
    vector<vec3> new_box1_points;
    vector<vec3> new_box2_points;
    
    vec3 diff;
    
    //Inverse of box1 on box 2
    for (vector<vec3>::iterator it = box2_points.begin(); it != box2_points.end();it++) {
        
        vec3 newpoint = *it;
        new_box2_points.push_back(box1_inverse * vec4(newpoint,1.0f));
    }
    
    //Inverse of box2 on box1
    for (vector<vec3>::iterator it = box1_points.begin(); it != box1_points.end();it++) {
        
        vec3 newpoint = *it;
        new_box1_points.push_back(box2_inverse * vec4(newpoint,1.0f));
    }
    
    //cout<<5.0f;
    
    //for (vector<vec3>::iterator it = new_box2_points.begin(); it != new_box2_points.end();it++){
    for (int i=0; i < 8; i++){
        
        if ((new_box2_points[i].x < 0.5f) && (new_box2_points[i].x > -0.5f) && (new_box2_points[i].y < 0.5f) && (new_box2_points[i].y > -0.5f) && (new_box2_points[i].z < 0.5f) && (new_box2_points[i].z > -0.5f)){
            
        //if ((new_box1_points[i].x < 0.5f) && (new_box1_points[i].x > -0.5f) && (new_box1_points[i].y < 0.5f) && (new_box1_points[i].y > -0.5f) && (new_box1_points[i].z < 0.5f) && (new_box1_points[i].z > -0.5f)){
            
            cout<<"Collision Detected!"<<endl;
            
            diff = box1.getCenter() - box2.getCenter();
           
            //}
        }
    }
    //cout<<
    return diff;
}

//need snothet check collison fn that checks pts of the boxpart object
//each boxpart obj has iuts own boxpoiunts that need to be inverse transformed
