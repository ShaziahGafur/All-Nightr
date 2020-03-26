
//includes for m3 visualization
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "ezgl/point.hpp"
#include <chrono>
#include <thread>
//#define VISUALIZE
#include <list> //remove once wavefront data structure updated
#include "m3.h"
#include "globals.h"

//helper declarations
bool breadthFirstSearch(int startID, int destID, const double turn_penalty);
std::vector<StreetSegmentIndex> bfsTraceBack(int destID);
Node* getNodeByID(int intersectionID);

void highlightStreetSegment (ezgl::renderer *g, int ID);
void delay(int milliseconds);

//global variable
//key : int intersectionID, value: pointer to node
std::unordered_map<int, Node*> nodesEncountered;
double bestPathTravelTime;
//int prevSegID = 0;

std::string directionsText;

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
    
//    //make node object of starting intersection
//    Node sourceNode(intersect_id_start);
//    Node* sourceNodePtr = &sourceNode;
//    
//    store source node into unordered_map
//    int intersectID = intersect_id_start;
//    nodesEncountered.insert({intersectID, sourceNodePtr});
    
    //start and end intersections flipped 
    //to allow "Tracing forwards rather than "Tracing backwards"
    pathFound = breadthFirstSearch(intersect_id_end, intersect_id_start, turn_penalty); 
    
//    std::cout<<"Path found: "<<pathFound<<std::endl;
    //If path is found, traceback path and store street segments
    if (pathFound){
        path = bfsTraceBack(intersect_id_start); //trace forwards, starting from the starting ID
    }
    //delete wavefront data structures
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


bool breadthFirstSearch(int startID, int destID, const double turn_penalty){
    
    bestPathTravelTime = 0;
    //Create Node for start Intersection
    Node* sourceNodePtr = new Node(startID, NO_EDGE, NO_TIME);
    nodesEncountered.insert({startID, sourceNodePtr}); //keep track of new start node and its ID (for deletion)
    
    //declare list which will contain queue of nodes to check 
    std::list<wave> wavefront; //change data structure to heap
     //put source node into wavefront
    wavefront.push_back(wave(sourceNodePtr, NO_EDGE, NO_TIME));
    
    //variable to be used later to store intersection ID as an int
    int  outerIntersectID;
       
    //while there exists nodes in the queue, check these connected nodes
    while (!wavefront.empty()){   
        //first deal with wave at top of list
        wave waveCurrent = wavefront.front();
        //remove top wave, it is being checked
        wavefront.pop_front();
        double waveCurrentTime = waveCurrent.travelTime;
        Node * waveCurrentNode = waveCurrent.node;
        
//        bool betterPathFlag = true;
        
        //if better path was found (currently travelling by this wave had smaller time than the Node's prehistoric best time)
//        std::cout<<"Best time: "<<waveCurrentNode->bestTime<<"\n";
//        std::cout<<"Best time: "<<waveCurrentNode->crawlEnable<<"\n";
//        std::cout<<"Best time: "<<waveCurrentNode->reachingEdge<<"\n";
//        std::cout<<"Wave Current time: "<<waveCurrentTime<<"\n";
//        std::cout<<"Wave Current Edge Id: "<<waveCurrent.edgeID<<"\n";
        
//        std::cout<<"Works here!\n";
        
        if (waveCurrentTime < waveCurrentNode->bestTime){
//            betterPathFlag = true;
            waveCurrentNode->crawlEnable = true;
            waveCurrentNode->bestTime = waveCurrentTime; //update Node's best time
            waveCurrentNode->reachingEdge = waveCurrent.edgeID; //update Node's reaching edge
            
        }  
        
        //check if current node is destination node
        if (waveCurrent.node->ID == destID){
            bestPathTravelTime = waveCurrentTime; //set the time for the best path
            return true;
        }
        
        if (waveCurrentNode->crawlEnable){ //If better path found, crawl to outer nodes
//            std::cout<<"Crawling will occur"<<"\n";
            
            //iterate through edges of current node to add the nodes they're going TO to the wavefront
            std::vector<int>edges = waveCurrentNode->outEdgeIDs;
            std::vector<int>::iterator it;
            for(it = edges.begin(); it != edges.end(); ++it){
//                std::cout<<"A seg id is:"<<(*it)<<"\n";
                //make this a helper function?
                //find "TO" intersection for segment and push node and edge used to get to node to bottom of wavefront
                InfoStreetSegment segStruct = getInfoStreetSegment(*it);
                if (segStruct.from == waveCurrentNode->ID){
                    outerIntersectID = segStruct.to;
                }
                else{ //the inner Node = the 'to' of the segment
                    //check if one-way
                    if (segStruct.oneWay)
                        continue; //path down this segment is invalid, skip to next segment
                    outerIntersectID = segStruct.from;
                }
                
//                std::cout<<"outer Intersect ID: "<<outerIntersectID<<"\n";
                
                std::unordered_map<int,Node*>::const_iterator nodeItr =  nodesEncountered.find(outerIntersectID);
                Node* outerNode;
                
                if (nodeItr ==nodesEncountered.end()){ //if node does not exist yet
                    outerNode = new Node(outerIntersectID, *it, waveCurrentTime);//create a new Node
                    nodesEncountered.insert({outerIntersectID, outerNode});

                }
                else{ //if node exists, use the existing node
                    outerNode = nodeItr->second;
                }
                
                double newTravelTime;
                if (waveCurrentNode->reachingEdge==-1){//corner case: current node is the start node, so there doesn't exist a reaching edge
                    newTravelTime = SegmentTravelTime[*it]; //travel time is only the time of the current segment
                }
                else{//if previous segment (reaching edge) exists
                    std::vector<int> adjacentSegments; //hold street segmnts of the inner node and segment from inner to outer node
                    adjacentSegments.push_back(waveCurrentNode->reachingEdge); //segment to get to inner node
                    adjacentSegments.push_back(*it); //segment from inner to outer node
                    //calculate travel time of both adjacent segments, subtract off the 1st segment 
                    //result is the turn_penalty (if applicable) and 2nd street seg travel time               
                    newTravelTime = compute_path_travel_time(adjacentSegments, turn_penalty) - SegmentTravelTime[waveCurrentNode->reachingEdge]; 
                    newTravelTime += waveCurrentTime;
                }               

                wavefront.push_back(wave(outerNode, *it, newTravelTime)); //create new wavefront elemenet and add to queue
//                std::cout<<"Successful iteration\n";

            }
            waveCurrentNode->crawlEnable = false; //crawling complete. Reset enable to false. 
        }
    }
    
    //if no path is found
    directionsText = "No path found";
    return false;
}

//bfsTraceBack is actually "tracing forward" since initially start and end IDs were flipped
//Creates message for directions of path
std::vector<StreetSegmentIndex> bfsTraceBack(int startID){ //startID is the node from which we start to "Trace back" from
    std::vector<StreetSegmentIndex> path;
    //variable to traverse nodes
    Node* currentNode = NULL;
    Node * nextNode = getNodeByID(startID);
    //get reaching edge segment from destination Node
    int forwardSegID = nextNode->reachingEdge;
//    std::cout<<"Forward seg id: "<<forwardSegID<<"\n";

    //variable to store intersectionID
    int intersectionID = startID;
    int previousIntersectID, previousSegID; //historic street seg needed for directions
    
    while (forwardSegID != NO_EDGE){
//        std::cout<<"Forward seg id: "<<forwardSegID<<"\n";
        path.push_back(forwardSegID);        
        currentNode = nextNode;
        
        //advance nextNode
        //find intersection-node the segment came to current node from and set it to next node
        InfoStreetSegment segStruct = getInfoStreetSegment(forwardSegID);
//        std::cout<<"forward street id: "<<segStruct.streetID<<std::endl;
        //save directions
        directionsText = directionsText + getStreetName(segStruct.streetID)+ "\n";
        
        if (segStruct.to == intersectionID){
            nextNode = getNodeByID(segStruct.from);
        }
        else{
            nextNode = getNodeByID(segStruct.to);
        }
        
        intersectionID = nextNode->ID; //update intersectionID as the intersection at nextNode
        
        //delete the current node. No longer needed
        delete currentNode;

        //retrieve next segment (segment after nextNode)
        forwardSegID = nextNode->reachingEdge;        
    }
    delete nextNode;
    
    directionsText = directionsText + "\n\nYou arrived at your destination. Estimated time: "<<bestPathTravelTime<<std::endl<<" mins.";
    std::cout<<"Directions are:\n"<<directionsText<<std::endl;
    
    
//    //convert list to a vector
//    std::vector<StreetSegmentIndex> pathVect;
//    pathVect.reserve(path.size());
//    std::copy(std::begin(path), std::end(path), std::back_inserter(pathVect));
    
    return path;
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