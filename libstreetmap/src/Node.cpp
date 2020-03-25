/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Node.h"
#include "globals.h"


Node::Node(int intersectionID, int reachingEdgeID, double shortestTime){
    ID = intersectionID; //Node's ID = intersection ID
    outEdgeIDs = find_street_segments_of_intersection(intersectionID);
    reachingEdge = reachingEdgeID;
    bestTime = shortestTime;
}

