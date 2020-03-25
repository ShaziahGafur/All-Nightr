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
#include "poiStruct.h"
#include "wave.h"
#include "Node.h"
#include "segmentStruct.h"
#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

//-----Global Variables------------------------------------------

//0. Map Loading

//name of curretnly loaded map
extern std::string MapName;
//
////close current map
//extern void close_map();

//loads another map
//extern bool load_map(std::string map_streets_database_filename); 

//1. Street Data
//Vector --> key: [streetID] value: [StreetStruct]
//StreetStruct --> Members: [street name, street segments, intersections]
extern std::vector<streetStruct> StreetVector;

//Multimap --> key: [Street Name] value: [Street Index]
extern std::multimap<std::string, int> StreetNames;

//Vector --> key: [intersection ID] value: [streetSegmentsVector]
extern std::vector<std::vector<int>> IntersectionStreetSegments;


//2. OSM Data
//Hashtable --> key: [Node_Id] value: [OSMID]
extern std::unordered_map<OSMID, int> OSMID_to_node;

//Hashtable --> key: [OSMway] value: [length of way]
extern std::unordered_map<OSMID, double> OSMWay_lengths;

//Hashtable --> key: [OSMway] value: [road type]
//Determines the road type (e.g. residential, motorway, primary) from a way's OSMID
extern std::unordered_map<OSMID, std::string> WayRoadType;


//3. Features

//Vector --> key: Feature Type (e.g. 0 = Unknown, 1 = Park...) value: vector containing feature IDs
extern std::vector<std::vector<int>> FeatureTypes;

//Hashtable --> key: [OSMID Node] value: [struct with xy coordinates and name]
//extern std::unordered_map<OSMID, subwayStruct> publicTransportation;

//Vector --> key: [feature ID] value: [Area]
extern std::vector<double> FeatureAreaVector;


//4. Segments & Intersections

//Vector --> key: [segment ID] value: [length]
extern std::vector<double> SegmentLengths;

//Vector --> key: [segment ID] value: [travel_time]
extern std::vector<double> SegmentTravelTime;

//Vector --> key: [intersection ID] value: [LatLon Coordinates]
extern std::vector<LatLon> IntersectionCoordinates;

//Vector --> key: [segment ID] value: [segmentStruct]
extern std::vector<segmentStruct> segmentHighlight;
#endif /* GLOBALS_H */

