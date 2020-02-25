#ifndef GLOBALS_H
#define GLOBALS_H

#include "m1.h"
#include "globals.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include <cmath>
#include <string>
#include <map> 
#include <unordered_map> 
#include "streetStruct.h"
#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

//-----Global Variables------------------------------------------
//Vector --> key: [streetID] value: [StreetStruct]
//StreetStruct --> Members: [street name, street segments, intersections]
extern std::vector<streetStruct> StreetVector;

//Vector --> key: [intersection ID] value: [streetSegmentsVector]
extern std::vector<std::vector<int>> IntersectionStreetSegments;

//Hashtable --> key: [Node_Id] value: [OSMID]
extern std::unordered_map<OSMID, int> OSMID_to_node;

//Hashtable --> key: [OSMway] value: [length of way]
extern std::unordered_map<OSMID, double> OSMWay_lengths;

//Vector --> key: [feature ID] value: [Area]
extern std::vector<double> FeatureAreaVector;

//Vector --> key: [segment ID] value: [length]
extern std::vector<double> SegmentLengths;

//Vector --> key: [segment ID] value: [travel_time]
extern std::vector<double> SegmentTravelTime;

//Vector --> key: [intersection ID] value: [LatLon Coordinates]
extern std::vector<LatLon> IntersectionCoordinates;

//Multimap --> key: [Street Name] value: [Street Index]
extern std::multimap<std::string, int> StreetNames;


#endif /* GLOBALS_H */

