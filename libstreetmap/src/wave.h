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
    double directionDif;
    //double distancePercentage;
    double hN;
    int waveIDTracker;
    wave (Node* n, int id, double time, double dirDif, double heuristic, int IDTracker) {node = n; edgeID = id; travelTime = time; directionDif = dirDif; waveIDTracker = IDTracker; hN = heuristic;}
};

struct compareHeuristicFunction{
    bool operator()(wave const& p1, wave const& p2){
        return (p1.hN > p2.hN);
    }
};

struct compareWalkingTime{
    bool operator()(wave const& p1, wave const& p2){
        return (p1.travelTime > p2.travelTime);
    }
};

#define NO_EDGE -1  //no edge id
#define NO_TIME 0  //zero time
#define PERFECT_HEURISTIC 0  //perfect heuristic returns 0
#define NO_DIRECTION_DIFFERENCE 0   

#endif /* WAVE_H */

