
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
//#include "drawMap.cpp"
#include "drawMap.h"

#define TAN_35 0.700  //used for determining direction
#define TAN_55 1.428  
#define ANGLE_Threshold 30

typedef std::pair<double, int> weightPair;

// ~~~ FUNCTION DECLARATIONS ~~~

// For Driving Path
bool breadthFirstSearch(int startID, int destID, const double turn_penalty);
std::vector<StreetSegmentIndex> bfsTraceBack(int destID); //Traces path from start to end, provides 

//For Walking Path
bool walkingPathBFS(int startID, int destID, const double turn_penalty, 
        const double walking_speed, const double walking_time_limit);
std::vector<StreetSegmentIndex> bfsWalkingTraceBack(int destID); //Traces path from start to end, provides 



Node* getNodeByID(int intersectionID);
double getDirectionAngle(int from, int to);
//void delay(int milliseconds);


//void highlightStreetSegment (ezgl::renderer *g, int ID);

// Printing Helper Functions
std::string printTime(double time);
std::string printDistance(double distance);

//          GLOBAL VARIABLES          //
//key : int intersectionID, value: pointer to node
std::unordered_map<int, Node*> nodesEncountered;
std::unordered_map<int, Node*> walkableNodes;
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
//                std::cout<<"Yo";

    //If path is found, traceback path and store street segments
    if (pathFound){
        path = bfsTraceBack(intersect_id_start); //trace forwards, starting from the starting ID
    }
    
//    std::cout<<std::endl;
//        std::cout<<"Time required: "<<std::to_string(compute_path_travel_time(path, turn_penalty));
//        std::cout<<"Hi";
    
    //delete nodes
    for (std::unordered_map<int, Node*>::iterator nodesIt = nodesEncountered.begin(); nodesIt != nodesEncountered.end(); ++nodesIt){
        //deleted nextNode
        delete (*nodesIt).second;
    }
//    std::cout<<std::flush;    
//    std::cout<<std::endl;
//    std::cout<<"Hi";
    nodesEncountered.clear();
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
    
    bool fullWalkPath = false;
    fullWalkPath = walkingPathBFS(start_intersection,  
                                end_intersection,
                                turn_penalty,
                                walking_speed, 
                                walking_time_limit);
    
    //dummy code for return
    
    std::vector<StreetSegmentIndex> drivingPath = find_path_between_intersections(start_intersection, end_intersection, turn_penalty);
    std::vector<StreetSegmentIndex> walkingPath;
    
    std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> vect = std::make_pair(drivingPath, walkingPath);
    
    return vect;
}


bool breadthFirstSearch(int startID, int destID, const double turn_penalty){
    
    bestPathTravelTime = 0;
    //Create Node for start Intersection
    Node* sourceNodePtr = new Node(startID, NO_EDGE, NO_TIME);
    nodesEncountered.insert({startID, sourceNodePtr}); //keep track of new start node and its ID (for deletion)
    
    //declare list which will contain queue of nodes to check 
    std::vector<wave> waveList; //change data structure to heap
    std::priority_queue<wave, std::vector<wave>, compareDirection> waveQueue; //hold all the weights with the IDs of the waves (to be accesed from vector)
    int waveIDTracker = 0; //keep track of IDs of waves
    
    //get direction from startID to destID -> it is the first ideal direction
    double idealDirection = getDirectionAngle(startID, destID);
    
     //put source node into wavefront
    wave sourceWave(sourceNodePtr, NO_EDGE, NO_TIME, idealDirection, waveIDTracker);
    waveList.push_back(sourceWave);
    waveQueue.push(sourceWave); //0 length for reaching edge, 0 for ID in waveList as its the first wave 
    waveIDTracker++; //advance to next ID of wave
    
    //variable to be used later to store intersection ID as an int
    int  outerIntersectID;
       
    //while there exists nodes in the queue, check these connected nodes
    while (!waveQueue.empty()){   
        //first deal with wave at top of list
        wave waveCurrent = waveList[waveQueue.top().waveIDTracker]; //based on the ID with smallest weighting in priority queue, get that wave
        //remove top wave, it is being checked
        waveQueue.pop();
        double waveCurrentTime = waveCurrent.travelTime;
        Node * waveCurrentNode = waveCurrent.node;
        idealDirection = waveCurrent.directionDif;
            
        //if better path was found (currently travelling by this wave had smaller time than the Node's prehistoric best time)
        if (waveCurrentTime < waveCurrentNode->bestTime){
            waveCurrentNode->crawlEnable = true;
            waveCurrentNode->bestTime = waveCurrentTime; //update Node's best time
            waveCurrentNode->reachingEdge = waveCurrent.edgeID; //update Node's reaching edge
//            std::cout<<"Better time found\n";
        }  
//        else if (waveCurrentTime == waveCurrentNode->bestTime){
//          std::cout<<"This Node is being travelled for first time\n";  
//        }
        
        //check if current node is destination node
        if (waveCurrent.node->ID == destID){
            bestPathTravelTime = waveCurrentTime; //set the time for the best path
            waveQueue = std::priority_queue<wave, std::vector<wave>, compareDirection >();
            waveList.clear();
            return true;
        }
        
        if (waveCurrentNode->crawlEnable){ //If better path found or new node, crawl to outer nodes
            
            //iterate through edges of current node to add the nodes they're going TO to the wavefront
            std::vector<int>edges = waveCurrentNode->outEdgeIDs;
            std::vector<int>::iterator it;
            
            for(it = edges.begin(); it != edges.end(); ++it){
//                std::cout<<"A seg id is:"<<(*it)<<"\n";
                //make this a helper function?
                //find "TO" intersection for segment and push node and edge used to get to node to bottom of wavefront
                InfoStreetSegment segStruct = getInfoStreetSegment(*it);
                if (segStruct.from == waveCurrentNode->ID){
                    //check if one-way
                    if (segStruct.oneWay)
                        continue; //path down this segment is invalid, skip to next segment
                    outerIntersectID = segStruct.to;
                }
                else{ //the inner Node = the 'to' of the segment
                    outerIntersectID = segStruct.from;
                }
                                
                std::unordered_map<int,Node*>::const_iterator nodeItr =  nodesEncountered.find(outerIntersectID);
                Node* outerNode;
                
                bool newNodeCreated = false;
                if (nodeItr ==nodesEncountered.end()){ //if node does not exist yet
                    outerNode = new Node(outerIntersectID, *it, waveCurrentTime);//create a new Node
                    nodesEncountered.insert({outerIntersectID, outerNode});
                    newNodeCreated = true;

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
                
                if(newNodeCreated) //IMPORTANT: Verify that this is necessary
                    outerNode->bestTime = newTravelTime;
                
                //calculate angle from outerNode to destID
                double outerNodeDirection = getDirectionAngle(outerIntersectID, destID);
                //idealDirection is from the innerNode's direction to destID
                double directionDif =  (idealDirection - outerNodeDirection);
                //check for values of directionDif
                if (directionDif <= 180){
                    directionDif = directionDif + 360;
                }
                else if (directionDif > 180){
                    directionDif = directionDif - 360;
                }
                
                wave currentWave(outerNode, *it, newTravelTime, directionDif, waveIDTracker);
                waveList.push_back(currentWave); //create new wavefront elemenet and add to queue
                waveQueue.push(currentWave); //0 length for reaching edge, 0 for ID in waveList as its the first wave 
                waveIDTracker++; //advance to next ID of wave

            }
            waveCurrentNode->crawlEnable = false; //crawling complete. Reset enable to false. 
        }
    }
    
    waveQueue = std::priority_queue<wave, std::vector<wave>, compareDirection >();
    waveList.clear();
    //if no path is found
    directionsText = "No path found";
    return false;
}

//bfsTraceBack is actually "tracing forward" since initially start and end IDs were flipped
//Creates message for directions of path
std::vector<StreetSegmentIndex> bfsTraceBack(int startID){ //startID is the node from which we start to "Trace back" from
    std::vector<StreetSegmentIndex> path;
    //variable to traverse nodes
//    Node* currentNode = NULL;
    Node * nextNode = getNodeByID(startID);
    //get reaching edge segment from destination Node
    int forwardSegID = nextNode->reachingEdge;

    //variable to store intersectionID
    int nextIntersectID = startID;
    //historic street seg & intersection needed for directions
    int previousIntersectID = NO_EDGE, middleIntersectID = nextIntersectID;//Set to -1 for non-existent values (since initially looking at first Node)
    std::string directionInstruction = ""; //a single line of the directions
    
    //Make instructions more condensed by combining redundant instructions that say "Continue Straight" for the same street
    int redundantStreetID = -1; //keep track of the redundant street ID
    bool continuingStraight = false; //flag to keep track if the directions continuously follow straight
    double distanceCombined = 0; //track the total distance to travel accross all redundant street segments
    
    while (forwardSegID != NO_EDGE){

        middleIntersectID = nextIntersectID;
        
//        std::cout<<"Forward seg id: "<<forwardSegID<<"\n";        
        path.push_back(forwardSegID); //this segment is part of the path
        segmentHighlight[forwardSegID].driving = true; //part of driving path
        segmentsHighlighted.push_back(forwardSegID); //add this segment to the list of those highlighted
        
//        currentNode = nextNode;
        
        //advance nextNode
        //find intersection-node the segment came to current node from and set it to next node
        InfoStreetSegment segStruct = getInfoStreetSegment(forwardSegID);
//        std::cout<<"forward street id: "<<segStruct.streetID<<std::endl;
        //save directions
//        directionsText = directionsText + getStreetName(segStruct.streetID)+ "\n";
        
        if (segStruct.to == nextIntersectID){
            nextNode = getNodeByID(segStruct.from);
        }
        else{
            nextNode = getNodeByID(segStruct.to);
        }
        
        nextIntersectID = nextNode->ID; //update intersectionID as the intersection at nextNode
        
        //At this point, next, current, and previousIntersectID are all set
        //There are 4 parts to direction:
        //1. "In __ km / m"
        //2. Navigation ("Turn Left/Right" / "Continue Straight" / "Make a U-turn" / "Head [bearing]" )
        //3. "on"
        //4. New street name
        
        //Order of parts computed: 2, 3, 4, 1
        directionInstruction = ""; //reset value of directionInstruction (single line in the directions)
        //part #2:
        if(previousIntersectID ==NO_EDGE){ //if this is the first street seg of path
            directionInstruction += "Head ";
            LatLon midInter = IntersectionCoordinates[middleIntersectID];
            LatLon nextInter = IntersectionCoordinates[nextIntersectID];
            
            std::pair <double, double> xyStart = latLonToCartesian(midInter);
            std::pair <double, double> xyNext = latLonToCartesian(nextInter); 
            double angle = getRotationAngle(xyStart, xyNext); //angle returned is in range [-180, 180]
            
            if (angle <= 30)
                directionInstruction += "East ";
            else if (angle <= 60)
                directionInstruction += "North East ";
            else if (angle <= 120)
                directionInstruction += "North ";
            else if (angle <= 150)
                directionInstruction += "North West";
            else if (angle <= 210)
                directionInstruction += "West ";
            else if (angle <= 240)
                directionInstruction += "South West";
            else if (angle <= 300)
                directionInstruction += "South ";
            else if (angle <= 330)
                directionInstruction += "South East";
            else //if (angle <= 180)
                directionInstruction += "East ";
            
        }//end of (if this is the first segment)
        
        else{    
            LatLon prevInter = IntersectionCoordinates[previousIntersectID];
            LatLon midInter = IntersectionCoordinates[middleIntersectID];
            LatLon nextInter = IntersectionCoordinates[nextIntersectID];
            
            std::pair <double, double> xyPrev = latLonToCartesian(prevInter);
            std::pair <double, double> xyMid = latLonToCartesian(midInter);
            std::pair <double, double> xyNext = latLonToCartesian(nextInter); 
            double angle1 = getRotationAngle(xyPrev, xyMid); //angle of first segment with respect to x, y axis
            double angle2 = getRotationAngle(xyMid, xyNext); //angle of second segment
            double angleDiff = angle2 - angle1; //compare the angle the next street with previous street
            
            bool flagBroken = false; //helper flag for when the street segment transitions from being redundant to NOT being redundant
            if (continuingStraight){
                if (segStruct.streetID==redundantStreetID && angleDiff > -15 && angleDiff <=15){ //if the continuingStraight flag should still be true
                    distanceCombined += SegmentLengths[forwardSegID]; //increment the total distance that was skipped (redundant)
                    
                    //attempt to skip to next iteration of while loop (skip to the next segment)
                    previousIntersectID = middleIntersectID; //advance prevoiusIntersectID
                    forwardSegID = nextNode->reachingEdge; 
                    continue;
                }
                else{ //If segment is no longer redundant, continuingStraight flag must be reset
                    flagBroken = true;
                    continuingStraight = false;
                    directionInstruction += "\nIn " + printDistance(distanceCombined)+", ";
                    distanceCombined = 0; //reset the total distance combined back to 0, for safety (redundancy has been dealt with)
                }
            }
           
            
            if (angleDiff > 165){ //next street is angled at > 165 degrres counter-clockwise 
                directionInstruction += "Make a Sharp Left ";//or U-turn
            }
            else if (angleDiff > 15){ //next street is angled between 15 and 165 degrres counter-clockwise (Left turn)
                directionInstruction += "Turn Left ";//Left turn
            }
            else if (angleDiff > -15 && !continuingStraight){ //next street is angled between 15 and -15 degrees (straight "turn")
                //Two possible cases. Cases determined using flagBroken:
                //Case 1: We reached a straight segment and consecutively, it is straight for the first time (if flagBroken is FALSE)
                //Case 2: Special case were street segments remain straight, however the name has been changed (e.g. College St to Carlton St) (if flagBroken is TRUE) 
                
                directionInstruction += "Continue Straight ";//required to print in both cases
                
                if (!flagBroken){ //in this case, we do NOT want to reset the process of setting the flag for continuingStraight and checking for redundancy. 
                    directionsText +=directionInstruction;
                    redundantStreetID = segStruct.streetID;
                    directionsText += "on " + getStreetName(redundantStreetID);
                    distanceCombined = SegmentLengths[forwardSegID];
                    continuingStraight = true;
                    
                    //attempt to skip to next iteration of while loop
                    previousIntersectID = middleIntersectID; //advance prevoiusIntersectID
                    forwardSegID = nextNode->reachingEdge;  
                    continue;
                }
            }
            else if (angleDiff > -165){ //Right Turn 
                directionInstruction += "Turn Right ";//Right turn
            }
            else{  //next street is angled < -165 degrres (clockwise) (Sharp right turn or U-turn)
                directionInstruction += "Make a Sharp Right ";//U-turn or Sharp Right turn
            }
        }
        //part #3:
        directionInstruction += "on ";
        
        //part #4:
        directionInstruction += getStreetName(segStruct.streetID);
        
        //part #1:
        directionInstruction +="\nIn "+printDistance(SegmentLengths[forwardSegID])+", ";
        
        directionsText+=directionInstruction;
        
        previousIntersectID = middleIntersectID; //advance prevoiusIntersectID

        //retrieve next segment (segment after nextNode)
        //invalid read of size 4
        forwardSegID = nextNode->reachingEdge;        
    }
    
    if (distanceCombined!=0) //special case if the path ends with a redundant street name. Part #1 (remaining distance) needs to be printed
        directionsText += "\nIn " + printDistance(distanceCombined)+", ";
    
    directionsText = "Directions:\n\n"+directionsText + "You will arrive at your destination. \nEstimated time: " 
            + printTime(bestPathTravelTime/60); //convert bestPathTravelTime from seconds to minutes
       
    
    return path;
}

bool walkingPathBFS(int startID, int destID, const double turn_penalty,
        const double walking_speed,const double walking_time_limit){

//    bestPathTravelTime = 0;
    //Create Node for start Intersection
    Node* sourceNodePtr = new Node(startID, NO_EDGE, NO_TIME);
    walkableNodes.insert({startID, sourceNodePtr}); //keep track of new start node and its ID (for deletion)
    
    //declare list which will contain queue of nodes to check 
    std::vector<wave> waveList; //change data structure to heap
    std::priority_queue<wave, std::vector<wave>, compareWalkingTime> waveQueue; //hold all the weights with the IDs of the waves (to be accesed from vector)
    int waveIDTracker = 0; //keep track of IDs of waves
    
    //get direction from startID to destID -> it is the first ideal direction
//    double idealDirection = getDirectionAngle(startID, destID);
    
     //put source node into wavefront
    wave sourceWave(sourceNodePtr, NO_EDGE, NO_TIME, 0, waveIDTracker);
    waveList.push_back(sourceWave);
    waveQueue.push(sourceWave); //0 length for reaching edge, 0 for ID in waveList as its the first wave 
    waveIDTracker++; //advance to next ID of wave
    
    //variable to be used later to store intersection ID as an int
    int  outerIntersectID;
       
    //while there exists nodes in the queue, check these connected nodes
    while (!waveQueue.empty()){  
        
        /*First, extract wave & node*/
        //extract wave at top of list
        wave waveCurrent = waveList[waveQueue.top().waveIDTracker]; //based on the ID with smallest weighting in priority queue, get that wave
        waveQueue.pop(); //remove top wave, it is being checked
        
        /*Extract Node  Characteristics*/
        Node * waveCurrentNode = waveCurrent.node; 
        double waveCurrentTime = waveCurrent.travelTime;
        
        /*Check Corner Case: Destination can be reached within walking time limit*/
        //check if current node is destination node
        if (waveCurrent.node->ID == destID){
            waveQueue = std::priority_queue<wave, std::vector<wave>, compareWalkingTime >();
            waveList.clear();
            return true;
        }
                        
        if (waveCurrentTime == walking_time_limit) //If Walking time limit has been "used up" for this node
            continue; //skip to next wave
            
        /*Assume that crawling can be performed on wave's Node (Node's travelling Time is < walking_time_limit */

        /*Visit each outernode and evaluate walking time. Then decide whether to create wave and add to queue*/
        std::vector<int>edges = waveCurrentNode->outEdgeIDs; //extract node's outer edges
        std::vector<int>::iterator it;
        for(it = edges.begin(); it != edges.end(); ++it){
            //find "TO" intersection for segment and push node and edge used to get to node to bottom of wavefront
            InfoStreetSegment segStruct = getInfoStreetSegment(*it);
            if (segStruct.from == waveCurrentNode->ID){
                //check if one-way
                if (segStruct.oneWay)
                    continue; //path down this segment is invalid, skip to next segment
                outerIntersectID = segStruct.to;
            }
            else{ //the inner Node = the 'to' of the segment
                outerIntersectID = segStruct.from;
            }

            std::unordered_map<int,Node*>::const_iterator nodeItr =  walkableNodes.find(outerIntersectID);
            Node* outerNode;

            if (nodeItr == walkableNodes.end()){ //if node does not exist yet
                
                //compute projected walking time for the node
                double newTravelTime;
                
                if (waveCurrentNode->reachingEdge==-1){//corner case: current node is the start node, so there doesn't exist a reaching edge
                    newTravelTime = SegmentLengths[*it]/walking_speed; //travel time is only the time of the current segment
                }
                
                else{//if previous segment (reaching edge) exists
                    std::vector<int> adjacentSegments; //hold street segmnts of the inner node and segment from inner to outer node
                    adjacentSegments.push_back(waveCurrentNode->reachingEdge); //segment to get to inner node
                    adjacentSegments.push_back(*it); //segment from inner to outer node
                    //calculate travel time of both adjacent segments, subtract off the 1st segment 
                    //result is the turn_penalty (if applicable) and 2nd street seg travel time               
                    newTravelTime = compute_path_walking_time(adjacentSegments, walking_speed, turn_penalty);
                    newTravelTime-= SegmentLengths[waveCurrentNode->reachingEdge]/walking_speed;
                    newTravelTime += waveCurrentTime;
                }
                
                if (newTravelTime > walking_time_limit) //Walking to this Node would take longer than walking_time_limit
                    continue;
                
               /* Node* */outerNode = new Node(outerIntersectID, *it, newTravelTime);//create a new Node
                walkableNodes.insert({outerIntersectID, outerNode});
                //if within walking time: create node, wave, blah blah
                //else: continue
                
                
              

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
               
                
                wave currentWave(outerNode, *it, newTravelTime, 0, waveIDTracker);
                waveList.push_back(currentWave); //create new wavefront elemenet and add to queue
                waveQueue.push(currentWave); //0 length for reaching edge, 0 for ID in waveList as its the first wave 
                waveIDTracker++; //advance to next ID of wave

            }
            waveCurrentNode->crawlEnable = false; //crawling complete. Reset enable to false. 
        }
    
    
    waveQueue = std::priority_queue<wave, std::vector<wave>, compareWalkingTime >();
    waveList.clear();
    //if no path is found
    directionsText = "No path found";
    return false;
    
    
}



Node* getNodeByID(int intersectionID){
    Node* nodeOfID;
    
    //Use global structure to access node
    nodeOfID = nodesEncountered.find(intersectionID) -> second;
    
    return nodeOfID;
}

std::string printDistance(double distance){
    std::string text = "";
    
    //first round distance up
    
    if (distance > 900){ //greater than 900 m (will represent directions in km)
        int km = (int) (distance/1000);
        km++;//round up
//        distance = km*1000;
        text = std::to_string((km))+" km";
    }
    else if((distance/100)>1){ //greater than 100 m
        int ha = (int) (distance/100);
        ha++;//round up
        distance = ha*100;
        text = std::to_string(((int)distance))+" m";
    } 
    else if((distance/10)>1){ //greater than 10 m
        int dam = (int) (distance/10);
        dam++;//round up
        distance = dam*10;
        text = std::to_string(((int)distance))+" m";

    } 
    else if ((distance)>1){
        int m = (int) (distance);
        m++;//round up
        distance = m;
        text = std::to_string(((int)distance))+" m";

    }
    else{ //very small distance, less than 1 m
        distance = 1;
        text = std::to_string((int)distance)+" m";

    }
    return text;
    
}

std::string printTime(double time){
    std::string text = "";
    if (time>60){
        int hrs = (int)(time/60);
        text+= std::to_string(hrs)+" hr ";
    }
    int mins = ((int)(time))%60;
    mins++; //round up
    text+=std::to_string(mins)+" min";
    return text;
}

//returns direction angles in radians
double getDirectionAngle(int from, int to){
    int degree;
    
    LatLon fromLL = IntersectionCoordinates[from];
    LatLon toLL = IntersectionCoordinates[to];
    
    std::pair < double, double > fromCart = latLonToCartesian (fromLL);
    std::pair < double, double > toCart = latLonToCartesian (toLL);
    
    degree = atan2(fromCart.second - toCart.second, fromCart.first - toCart.first);
    return degree;
}
