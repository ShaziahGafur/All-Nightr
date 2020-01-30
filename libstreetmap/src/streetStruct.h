/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   streetStruct.h
 * Author: georg157
 *
 * Created on January 29, 2020, 6:28 PM
 */

#ifndef STREETSTRUCT_H
#define STREETSTRUCT_H

#include <string>
#include <vector>

class streetStruct {
public:

    streetStruct();
    
    virtual ~streetStruct();
    
    
private:
    std::vector<int> streetSegments;
    
    std::vector<int> intersections;
    
    std::string streetName;
    
    double streetId;
};

#endif /* STREETSTRUCT_H */

