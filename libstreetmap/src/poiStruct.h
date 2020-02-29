/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   subwayStruct.h
 * Author: georg157
 *
 * Created on February 29, 2020, 7:19 AM
 */

#include <string>

#ifndef POISTRUCT_H
#define POISTRUCT_H

class poiStruct{
    public:
        poiStruct();
        
        ~poiStruct();
        
        void addName (std::string name);
        
        void addXYCoordinates (std::pair<double,double> coordinates);
        
        std::pair<double, double> xyCoordinates;
        
        std::string Name;
    
};

#endif /* SUBWAYSTRUCT_H */

