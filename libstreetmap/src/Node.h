/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   node.h
 * Author: georg157
 *
 * Created on March 22, 2020, 6:04 PM
 */

#ifndef NODE_H
#define NODE_H

#include <vector>
class Node{
    
public:
    int ID;
    std::vector<int> outEdgeIDs;
    int reachingEdge;
    
    Node(int intersectionID);
    
};

#endif /* NODE_H */


