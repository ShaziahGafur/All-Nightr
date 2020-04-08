
#ifndef M3A_H
#define M3A_H

//includes for m3 visualization
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "ezgl/point.hpp"
#include <chrono>
#include <thread>
//#include <list> //remove once wavefront data structure updated
#include "m3.h"
#include "globals.h"
#include "drawMap.h"
#include <math.h>

//M4 path finding helper functions
bool djikstraBFS(int startID, std::vector<std::pair<int, std::string>> pickUpDropOffNodes, const double turn_penalty);
std::vector<StreetSegmentIndex> find_path_djikstra(const IntersectionIndex intersect_id_start, const std::vector<std::pair<int, std::string>> pickUpDropOffNodes, const double turn_penalty);
std::vector<StreetSegmentIndex> djikstraBFSTraceBack(int destID);

//Driving Path Helper functions
bool breadthFirstSearch(int startID, int destID, const double turn_penalty);
std::vector<StreetSegmentIndex> bfsTraceBack(int destID);
Node* getNodeByID(int intersectionID);

//Walking Path Helper functions
bool walkingPathBFS(int startID, int destID, const double turn_penalty, const double walking_speed, const double walking_time_limit);
Node* getWalkableNodeByID(int intersectionID);
std::vector<StreetSegmentIndex> walkBFSTraceBack(int pickupIntersectID); //Traces path from start to end, provides 

double getDirectionAngle(int from, int to);

//Printing Helper Functions
std::string printTime(double time);
std::string printDistance(double distance);

//Clearing Data Structures
void clearWalkableNodes();
void clearNodesEncountered();

#endif /* M3A_H */

