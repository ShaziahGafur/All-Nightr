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

#ifndef WAVE_ELEM_H
#define WAVE_ELEM_H

#include "Node.h"

struct waveElem{
    Node* node;
    int edgeID;
    double travelTime;
    waveElem (Node* n, int edge_id, double time) {node = n; edgeID = edge_id; travelTime = time;}
};

struct compareTime{
    bool operator()(waveElem const& p1, waveElem const& p2){
        return (p1.travelTime > p2.travelTime);
    }
};

#endif /* WAVE_H */

