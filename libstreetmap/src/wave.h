/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   wave.h
 * Author: georg157
 *
 * Created on March 22, 2020, 5:55 PM
 */

#ifndef WAVE_H
#define WAVE_H

#include "Node.h"

struct wave{
    Node* node;
    int edgeID;
    double travelTime;
    wave (Node* n, int id, double time) {node = n; edgeID = id; travelTime = time;}
};

#define NO_EDGE -1  //no edge id
#define NO_TIME 0  //zero time

#endif /* WAVE_H */

