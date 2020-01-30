/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   streetStruct.cpp
 * Author: georg157
 * 
 * Created on January 29, 2020, 6:28 PM
 */

#include "streetStruct.h"
#include <string>

streetStruct::streetStruct() {
}

streetStruct::~streetStruct() {
}
    
void streetStruct::addStreetSegment(int segmentId){
    streetSegments.push_back(segmentId);
}
    
void streetStruct::addIntersection(int intersectionId){
    intersections.push_back(intersectionId);
}
    
void streetStruct::setStreetName(std::string name){
    streetName = name;
}
