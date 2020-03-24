
//includes for m3 visualization
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "ezgl/point.hpp"
#include <chrono>
#include <thread>
//#define VISUALIZE

#include "m3.h"
#include "globals.h"

//helper declarations
bool breadthFirstSearch(Node* sourceNode, int destID);
std::vector<StreetSegmentIndex> bfsTraceBack(int destID);
Node* getNodeByID(int intersectionID);

void highlightStreetSegment (ezgl::renderer *g, int ID);
void delay(int milliseconds);

//global variable
//key : int intersectionID, value: pointer to node
std::unordered_map< int, Node*> nodesEncountered;
int prevSegID = 0;

// Returns the time required to travel along the path specified, in seconds.
// The path is given as a vector of street segment ids, and this function can
// assume the vector either forms a legal path or has size == 0.  The travel
// time is the sum of the length/speed-limit of each street segment, plus the
// given turn_penalty (in seconds) per turn implied by the path.  If there is
// no turn, then there is no penalty. Note that whenever the street id changes
// (e.g. going from Bloor Street West to Bloor Street East) we have a turn.
double compute_path_travel_time(const std::vector<StreetSegmentIndex>& path, const double turn_penalty){    
    
    double travelTime = 0;
    //Error check if vector size is zero
    if (path.size() == 0){
        return travelTime;
    }
    
    //make streetinfo struct and streetID variable to retrieve streetID from street segment index
    InfoStreetSegment segStruct;
    int previousStreetID, nextStreetID; //street IDs of two consecutive street
    
    //Find number of turn penalties by finding # of turns -> Need to find street id from street segment to get # of turns
    //Use global structure?
    //Also get travel time of each segment and store in travelTime
    std::vector<StreetSegmentIndex>::const_iterator it = path.begin();
    
    //get first streetID
    segStruct = getInfoStreetSegment(*it);
    previousStreetID = segStruct.streetID;
    
    travelTime = find_street_segment_travel_time(*it);
    it++;
    
    //For all street segments after the first one, run loop
    while(it != path.end()){
        
        //First check if there was a turn
        segStruct = getInfoStreetSegment(*it);
        nextStreetID = segStruct.streetID;
        
        //check if streetID has changed, if yes -> add turn penalty and increment previous streetID
        if (previousStreetID != nextStreetID){
            travelTime = travelTime + turn_penalty;
            previousStreetID = nextStreetID;
        } 
        
        //Second, add travel time of the street segment
        travelTime = travelTime + find_street_segment_travel_time(*it);
                
        // advance to next segment
        it++;    
    }
    
    return travelTime;
}
 
// Returns a path (route) between the start intersection and the end
// intersection, if one exists. This routine should return the shortest path
// between the given intersections, where the time penalty to turn right or
// left is given by turn_penalty (in seconds).  If no path exists, this routine
// returns an empty (size == 0) vector.  If more than one path exists, the path
// with the shortest travel time is returned. The path is returned as a vector
// of street segment ids; traversing these street segments, in the returned
// order, would take one from the start to the end intersection.
 std::vector<StreetSegmentIndex> find_path_between_intersections(const IntersectionIndex intersect_id_start, const IntersectionIndex intersect_id_end, const double turn_penalty){
    
    bool pathFound = false;
    std::vector<StreetSegmentIndex> path;
    
    //make node object of starting intersection
    Node sourceNode(intersect_id_start);
    Node* sourceNodePtr = &sourceNode;
    
    //store source node into unordered_map
    int intersectID = intersect_id_start;
    nodesEncountered.insert({intersectID, sourceNodePtr});
    
    pathFound = breadthFirstSearch(sourceNodePtr, intersect_id_end);
    //If path is found, traceback path and store street segments
    if (pathFound){
        path = bfsTraceBack(intersect_id_end);
    }
    return path;        
}
        
// Returns the time required to "walk" along the path specified, in seconds.
 // The path is given as a vector of street segment ids. The vector can be of
 // size = 0, and in this case, it the function should return 0. The walking
 // time is the sum of the length/ for each street segment, plus
 // the given turn penalty, in seconds, per turn implied by the path. If there
 // is no turn, then there is no penalty. As mentioned above, going from Bloor
 // Street West to Bloor street East is considered a turn 
double compute_path_walking_time(const std::vector<StreetSegmentIndex>& path, 
                                 const double walking_speed, 
                                 const double turn_penalty){
    
     double travelTime = 0;
    //Error check if vector size is zero
    if (path.size() == 0){
        return travelTime;
    }
    
    //make streetinfo struct and streetID variable to retrieve streetID from street segment index
    InfoStreetSegment segStruct;
    int previousStreetID, nextStreetID; //street IDs of two consecutive street
    
    //Find number of turn penalties by finding # of turns -> Need to find street id from street segment to get # of turns
    //Use global structure?
    //Also get travel time of each segment and store in travelTime
    std::vector<StreetSegmentIndex>::const_iterator it = path.begin();
    
    //get first streetID
    segStruct = getInfoStreetSegment(*it);
    previousStreetID = segStruct.streetID;
    
    //get first segment's travel walking speed
    double length = 0;
    length = SegmentLengths[*it]; //distance of segment
    travelTime = length/walking_speed; //time = distance / speed
    it++;
    
    //For all street segments after the first one, run loop
    while(it != path.end()){
        
        //First check if there was a turn
        segStruct = getInfoStreetSegment(*it);
        nextStreetID = segStruct.streetID;
        
        //check if streetID has changed, if yes -> add turn penalty and increment previous streetID
        if (previousStreetID != nextStreetID){
            travelTime = travelTime + turn_penalty;
            previousStreetID = nextStreetID;
        } 
        
        //Second, add travel walking time of the street segment
        length = SegmentLengths[*it];
        travelTime = travelTime + length/walking_speed;
                
        // advance to next segment
        it++;    
    }
    
    return travelTime;
}
        


std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> find_path_with_walk_to_pick_up(const IntersectionIndex start_intersection, 
                                                                             const IntersectionIndex end_intersection,
                                                                             const double turn_penalty,
                                                                             const double walking_speed, 
                                                                             const double walking_time_limit){
    std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> vect;
    
    return vect;
}


bool breadthFirstSearch(Node* sourceNode, int destID){
    //declare list which will contain nodes and edges on the wavefront
    std::list<wave> wavefront;
    
    //variable to be used later to store intersection ID as an int
    int intersectionID;
    //put source node into wavefront
    wavefront.push_back(wave(sourceNode, NO_EDGE));
    
    //while wavefront isn't empty, check connected nodes
    while (!wavefront.empty()){
        //current wave is top on list
        wave waveCurrent = wavefront.front();
        //remove top wave, it is being checked
        wavefront.pop_front();
        
        //store the edge from previous node to this node(reaching edge of top wave) as the reaching edge of the  current node so far
        waveCurrent.node->reachingEdge = waveCurrent.edgeID;
        
        //check if current node is destination node
        if (waveCurrent.node->ID == destID){
            return true;
        }
        
        //iterate through edges of current node to add the nodes they're going TO to the wavefront
        std::vector<int>edges = waveCurrent.node-> outEdgeIDs;
        std::vector<int>::iterator it;
        for(it = edges.begin(); it != edges.end(); ++it){
            //make this a helper function?
            //find "TO" intersection for segment and push node and edge used to get to node to bottom of wavefront
            InfoStreetSegment segStruct = getInfoStreetSegment(*it);
            if (segStruct.from == waveCurrent.node->ID){
                intersectionID = segStruct.to;
            }
            else{
                intersectionID = segStruct.from;
            }
            Node n(intersectionID);
            Node* nptr = &n;
            //push pointer to this node and the segment used to get to it is the edge
            wavefront.push_back(wave(nptr, *it));
             
            //insert node formed into unordered map
            nodesEncountered.insert({intersectionID, nptr});
            
        }
        //if no path is found
        return false;
    }
    
}

std::vector<StreetSegmentIndex> bfsTraceBack(int destID){
    std::list<StreetSegmentIndex> path;

    //variable to traverse nodes
    Node* currentNode = getNodeByID(destID);
    //get reaching edge segment from destination Node
    prevSegID = currentNode->reachingEdge;
    
    //variable to store intersectionID
    int intersectionID = destID;
    
    while (prevSegID != NO_EDGE){
        path.push_front(prevSegID);         //note: push_front is very bad efficiency -> try to use push back instead later and reverse the algorithm?
        
        //find intersection-node the segment came to current node from and set it to current node
        InfoStreetSegment segStruct = getInfoStreetSegment(prevSegID);
        if (segStruct.to == intersectionID){
            currentNode = getNodeByID(segStruct.from);
        }
        else{
            currentNode = getNodeByID(segStruct.to);
        }
        
       //update the previous segment ID (prevSegID) by setting it to the reaching edge of the current node
        prevSegID = currentNode->reachingEdge;        
    }  
    //convert list to a vector
    std::vector<StreetSegmentIndex> pathVect;
    pathVect.reserve(path.size());
    std::copy(std::begin(path), std::end(path), std::back_inserter(pathVect));
    
    return pathVect;
}


Node* getNodeByID(int intersectionID){
    Node* nodeOfID;
    
    //Use global structure to access node
    nodeOfID = nodesEncountered.find(intersectionID) -> second;
    
    return nodeOfID;
}
/*
#ifdef VISUALIZE


    ezgl::application* app;
    ezgl::renderer *g = app -> get_renderer();
    highlightStreetSegment (g, prevSegID);
    app -> flush_drawing();
    delay(50);

#endif


void delay(int milliseconds){
    std::chrono::milliseconds duration(milliseconds);
    std::this_thread::sleep_for(duration);
}

void highlightStreetSegment (ezgl::renderer *g, int ID){
    
}*/