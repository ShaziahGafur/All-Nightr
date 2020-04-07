
#ifndef M3_H
#define M3_H

#include "m1.h"
#include "globals.h"
#include "StreetsDatabaseAPI.h"
#include "globals.h"
#include <vector>
#include "m2.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "ezgl/point.hpp"


//returns direction angles in radians
double getDirectionAngle(int from, int to);

void highlightStreetSegment (ezgl::renderer *g, int ID);
void delay(int milliseconds);



//***********************************path functions**********************************************************
// Returns the time required to travel along the path specified, in seconds.
// The path is given as a vector of street segment ids, and this function can
// assume the vector either forms a legal path or has size == 0.  The travel
// time is the sum of the length/speed-limit of each street segment, plus the
// given turn_penalty (in seconds) per turn implied by the path.  If there is
// no turn, then there is no penalty. Note that whenever the street id changes
// (e.g. going from Bloor Street West to Bloor Street East) we have a turn.
double compute_path_travel_time(const std::vector<StreetSegmentIndex>& path, const double turn_penalty);
// Returns a path (route) between the start intersection and the end
// intersection, if one exists. This routine should return the shortest path
// between the given intersections, where the time penalty to turn right or
// left is given by turn_penalty (in seconds).  If no path exists, this routine
// returns an empty (size == 0) vector.  If more than one path exists, the path
// with the shortest travel time is returned. The path is returned as a vector
// of street segment ids; traversing these street segments, in the returned
// order, would take one from the start to the end intersection.
std::vector<StreetSegmentIndex> find_path_between_intersections(const IntersectionIndex intersect_id_start, const IntersectionIndex intersect_id_end, const double turn_penalty);
// Returns the time required to "walk" along the path specified, in seconds.
 // The path is given as a vector of street segment ids. The vector can be of
 // size = 0, and in this case, it the function should return 0. The walking
 // time is the sum of the length/ for each street segment, plus
 // the given turn penalty, in seconds, per turn implied by the path. If there
 // is no turn, then there is no penalty. As mentioned above, going from Bloor
 // Street West to Bloor street East is considered a turn 

std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> find_path_with_walk_to_pick_up(const IntersectionIndex start_intersection, 
                                                                             const IntersectionIndex end_intersection,
                                                                             const double turn_penalty,
                                                                             const double walking_speed, 
                                                                             const double walking_time_limit);

double compute_path_walking_time(const std::vector<StreetSegmentIndex>& path, const double walking_speed, const double turn_penalty);

// For Driving Path
bool breadthFirstSearch(int startID, int destID, const double turn_penalty);
//bfsTraceBack is actually "tracing forward" since initially start and end IDs were flipped
//Creates message for directions of path
std::vector<StreetSegmentIndex> bfsTraceBack(int destID); 
Node* getNodeByID(int intersectionID);


//For Walking Path
bool walkingPathBFS(int startID, int destID, const double turn_penalty, const double walking_speed, const double walking_time_limit);
Node* getWalkableNodeByID(int intersectionID);
std::vector<StreetSegmentIndex> walkBFSTraceBack(int pickupIntersectID); //Traces path from start to end, provides 

//void delay(int milliseconds);


//void highlightStreetSegment (ezgl::renderer *g, int ID);

// Printing Helper Functions
std::string printTime(double time);
std::string printDistance(double distance);

#endif /* M3_H */

