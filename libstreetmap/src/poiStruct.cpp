/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   subwayStruct.cpp
 * Author: georg157
 *
 * Created on February 29, 2020, 7:22 AM
 */

#include "subwayStruct.h"
#include <string>

subwayStruct::subwayStruct() {
}

subwayStruct::~subwayStruct() {
}

void subwayStruct::addSubwayName (std::string name){
    subwayName = name;
}

void subwayStruct::addXYCoordinates (std::pair<double,double> coordinates){
    xyCoordinates = coordinates;
}