/* 
 * Copyright 2020 University of Toronto
 *
 * Permission is hereby granted, to use this software and associated 
 * documentation files (the "Software") in course work at the University 
 * of Toronto, or for personal use. Other uses are prohibited, in 
 * particular the distribution of the Software either publicly or to third 
 * parties.
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "m1.h"
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


// load_map will be called with the name of the file that stores the "layer-2"
// map data (the street and intersection data that is higher-level than the
// raw OSM data). This file name will always end in ".streets.bin" and you 
// can call loadStreetsDatabaseBIN with this filename to initialize the
// layer 2 API.
// If you need data from the lower level, layer 1, API that provides raw OSM
// data (nodes, ways, etc.) you will also need to initialize the layer 1 
// OSMDatabaseAPI by calling loadOSMDatabaseBIN. That function needs the 
// name of the ".osm.bin" file that matches your map -- just change 
// ".streets" to ".osm" in the map_streets_database_filename to get the proper
// name.


//-----Global Variables------------------------------------------
//StreetStruct --> Members: [street name, street segments, intersections]
streetStruct stubStreetStruct;

//Vector --> key: [streetID] value: [StreetStruct]
std::vector<streetStruct> streetVector;

//Vector --> key: [intersection ID] value: [streetSegmentsVector]
std::vector<std::vector<int>> intersectionStreetSegments;

//Hashtable --> key: [Node_Id] value: [OSMID]
std::unordered_map<OSMID, int> OSMID_to_node;

//Hashtable --> key: [OSMway] value: [length of way]
std::unordered_map<OSMID, double> OSMWay_lengths;

//Vector --> key: [feature ID] value: [Area]
std::vector<double> featureAreaVector;

//Vector --> key: [segment ID] value: [length]
std::vector<double> segment_lengths;

//Vector --> key: [segment ID] value: [travel_time]
std::vector<double> segmentTravelTime;

//Vector --> key: [intersection ID] value: [LatLon Coordinates]
std::vector<LatLon> intersectionCoordinates;

//Multimap --> key: [Street Name] value: [Street Index]
std::multimap<std::string, int> streetNames;
//----------------------------------------------------------------

//---Function Declarations----------------------------------------
//Populating Street Vector Nodes with streetsdatabaseAPI data
void populateStreetVector();
//Populating Feature Area Vector with area
void populateFeatureAreaVector();
//Populating Hashtable with OSMdatabaseAPI data
void populateOSMID_to_node();
//Populating OSMWay_lengths
void populateOSMWay_lengths();
//Populating streetSegmentsOfAnIntersection
void populateIntersectionStreetSegments();
//Populating segment_lengths
void populate_segment_lengths();
//Populating segment_travel_time
void populate_segment_travel_time();
//Populating intersection Coordinates vector
void populateIntersectionCoordinates();
//Populating names of street with street index
void populateStreetNames();
//returns true if a given streetName begins with the same characters as a given prefix
bool isStreetName(std::string streetName, std::string prefix, int prefixLength);
//------------------------------------------------------------------


bool load_map(std::string map_streets_database_filename) {
     //Indicates whether both maps have loaded successfully
    bool load_successful;

    //check if streets database bin file loads successfully
    load_successful = loadStreetsDatabaseBIN(map_streets_database_filename);
    
    //if streets database loaded, create string for OSM filename and load it
    if(load_successful){
        
        //string to load OSM filename
        std:: string map_streets_database_filename_OSM = map_streets_database_filename;
        
        //remove.street.bin from string, concatenate .osm.bin
        if (!(map_streets_database_filename_OSM.empty())){
            map_streets_database_filename_OSM.resize(map_streets_database_filename.size()-11);
            map_streets_database_filename_OSM = map_streets_database_filename_OSM + "osm.bin";
        }
        //load corresponding OSM database
        load_successful = loadOSMDatabaseBIN(map_streets_database_filename_OSM);
    }
    
    //if load_successful is still true, populate data structures
    if (load_successful){
        //Populating Street Vector Nodes with streetsdatabaseAPI data
        populateStreetVector();
    
        //Populating Feature Area Vector with area
        populateFeatureAreaVector();
    
        //Populating Hashtable with OSMdatabaseAPI data
        populateOSMID_to_node();
    
        //Populating Hashtable with OSMWay_lengths
        populateOSMWay_lengths();
    
        //Populating streetSegmentsOfAnIntersection
        populateIntersectionStreetSegments();
    
        //Populating intersectionCoordinates vector
        populateIntersectionCoordinates();
    
        //Populating street names hash table
        populateStreetNames();
    
        //Populate segment lengths
        populate_segment_lengths();
    
        //Populate segment travel times;
        populate_segment_travel_time();
    
    }
    return load_successful;
}


void close_map() {
    //Clean-up your map related data structures here
    
    streetNames.clear();
    
    closeStreetDatabase(); 
    closeOSMDatabase();
}
//result is in meters
double find_distance_between_two_points(std::pair<LatLon, LatLon> points){
    double latAvg, p1_y, p1_x, p2_y, p2_x, distanceBetweenTwoPoints;
    
    latAvg = (points.first.lat()*DEGREE_TO_RADIAN + points.second.lat()*DEGREE_TO_RADIAN ) * 0.5;
    p1_y = points.first.lat()*DEGREE_TO_RADIAN ;
    p1_x = points.first.lon()*DEGREE_TO_RADIAN *cos(latAvg);
    p2_y = points.second.lat()*DEGREE_TO_RADIAN ;
    p2_x = points.second.lon()*DEGREE_TO_RADIAN *cos(latAvg);
    
    distanceBetweenTwoPoints = EARTH_RADIUS_METERS * sqrt(pow(p2_y - p1_y , 2)+ pow(p2_x - p1_x , 2));
  
    return distanceBetweenTwoPoints;
}

//Returns the length of the given street segment in meters
double find_street_segment_length(int street_segment_id){
    //if street_segment)id is within existent range
    
    if ((street_segment_id < getNumStreetSegments()) && (street_segment_id >= 0)){
        return segment_lengths[street_segment_id];
    }
    else{
        return 0;
    }
}

//Returns the travel time to drive a street segment in seconds 
//(time = distance/speed_limit)
double find_street_segment_travel_time(int street_segment_id){
    
    return segmentTravelTime[street_segment_id];
}

//Returns the nearest intersection to the given position
int find_closest_intersection(LatLon my_position){

    std::pair<LatLon, LatLon> path(my_position, intersectionCoordinates[0]);     
    int shortestDistance = find_distance_between_two_points(path);
    
    int distance;
    int closestIntersection = 0;
    
    for (unsigned i = 1; i < getNumIntersections(); i++){
        
        path.second = intersectionCoordinates[i];     
        
        distance = find_distance_between_two_points(path);
        
        if (distance < shortestDistance){
            
            shortestDistance = distance;
            closestIntersection = i;
        }
    }
    
    return closestIntersection;
}

//Returns the street segments for the given intersection 
std::vector<int> find_street_segments_of_intersection(int intersection_id){
    return intersectionStreetSegments[intersection_id];
}

//Returns the street names at the given intersection (includes duplicate street 
//names in returned vector)
std::vector<std::string> find_street_names_of_intersection(int intersection_id){
    
    //container for intersection street names
    std::vector<std::string> streetNamesOfIntersection;
    
    //container for the intersection street segments
    std::vector<int> ssOfIntersection = find_street_segments_of_intersection(intersection_id);
    
    //go through all connecting street segments to find street names
    for(std::vector<int>::iterator it = ssOfIntersection.begin(); it != ssOfIntersection.end(); ++it){
        //retrieve struct with street segment info 
        InfoStreetSegment ssInfoStruct = getInfoStreetSegment(*it);
        
        //get street name from street ID
        streetNamesOfIntersection.push_back(getStreetName(ssInfoStruct.streetID));
    }
    
    return streetNamesOfIntersection;
}

//Returns true if you can get from intersection_ids.first to intersection_ids.second using a single 
//street segment
bool are_directly_connected(std::pair<int, int> intersection_ids){    
    //function compares street segment vectors of two intersections to find any commonalities
    //then checks if going from intersection1 to intersection2 is allowed (i.e. legal)
    
    int intersection1 = intersection_ids.first;
    
    int intersection2 = intersection_ids.second;
    
    //extracting the street segment vector of both intersections
    std::vector<int> intersection1_segments = intersectionStreetSegments[intersection1];
    std::vector<int> intersection2_segments = intersectionStreetSegments[intersection2];
    
    //corner case: "to" and "from" are the same
    if(intersection1 == intersection2) 
        return true;
    
    //vector to be returned (list of all intersections)
    std::vector<int> commonStreetSegments;
    
    //resizing commonStreetSegments so that it can be assigned values
    commonStreetSegments.resize(intersection1_segments.size() + intersection2_segments.size());  
   
    //iterator used in subsequent set_intersection function 
     std::vector<int>::iterator it;
    
    //set_intersection is an STL function which finds elements common in two sets, and
    //returns them into a third vector
    it = std::set_intersection(intersection1_segments.begin(), 
                        intersection1_segments.end(), 
                        intersection2_segments.begin(), 
                        intersection2_segments.end(),
                        commonStreetSegments.begin());
    
    //resizes commonStreetSegments to the actual number of intersections
    commonStreetSegments.resize(it-commonStreetSegments.begin());
    
    //check if no common segments found
    if(commonStreetSegments.empty() == true)
        return false;
    
    InfoStreetSegment info;
   
    //NOTE: Street segment could be 1-way, therefore can get from second to first, but not first to second!!!!
    for(unsigned i = 0; i < commonStreetSegments.size(); ++i){
        
        info = getInfoStreetSegment(commonStreetSegments[i]);
        
        //check if street is one-way only. If not, can just return true;
        if(info.oneWay == true){
            //if street is one-way, ensure that intersection1 is "from"
            if(info.from == intersection1)
                return true;
        }
        else //if connecting street is not one-way
            return true;
    }
    //if only connecting street segments are one-way
    return false;
}


//Returns all intersections reachable by traveling down one street segment 
//from given intersection (hint: you can't travel the wrong way on a 1-way street)
//the returned vector should NOT contain duplicate intersections
std::vector<int> find_adjacent_intersections(int intersection_id){

    InfoStreetSegment info;

    std::vector<int> adjacentIntersections;
    
    //adjacentIntersections set which will "remove" duplicate entries
    //at the end, each set will be copied into the adjacentIntersections vector
    std::set<int> adjacentIntersections_set;
    
 
    //retrieve vector of all the street segments attached to the given intersection
    std::vector<int>  connected_street_segments = intersectionStreetSegments[intersection_id];
    
    for(std::vector<int>::iterator it = connected_street_segments.begin(); it < connected_street_segments.end(); ++it){
        //check if street is one way
        info = getInfoStreetSegment(*it);
        
        if (info.oneWay){
            //check if its 'from' intersection is the intersection_id (it is going TO the adjacent intersection) 
            if (info.from == intersection_id){
                //add intersection to the adjacentIntersection vector
                adjacentIntersections_set.insert(info.to);
            }
        }
        else{   //not one-way
            //check if 'from' or 'to' intersection of street segment is intersection_id and push back the other one into adjacentIntersections
            if (info.from == intersection_id)
                adjacentIntersections_set.insert(info.to);
         
            else
             adjacentIntersections_set.insert(info.from);
        }
    } 
    
    adjacentIntersections.resize(adjacentIntersections_set.size());
    adjacentIntersections.assign(adjacentIntersections_set.begin(), adjacentIntersections_set.end()); 
        
    return adjacentIntersections;
}

//Returns all street segments for the given street
std::vector<int> find_street_segments_of_street(int street_id){
    
    return streetVector[street_id].streetSegments;
}
//Returns all intersections along the a given street
std::vector<int> find_intersections_of_street(int street_id){ 
    
    return streetVector[street_id].intersections;
}

//Return all intersection ids for two intersecting streets
//This function will typically return one intersection id.
std::vector<int> find_intersections_of_two_streets(std::pair<int, int> street_ids){
    //extracting the streedIds
    int streetId1 = street_ids.first;
    int streetId2 = street_ids.second;
    
    //extracting the street intersection vector of both streets
    std::vector<int> streetIntersections1 = streetVector[streetId1].intersections;
    std::vector<int> streetIntersections2 = streetVector[streetId2].intersections;
    
    //vector to be returned (list of all intersections)
    std::vector<int> intersectionsOfTwoStreets;
    //resizing intersectionsOfTwoStreets so that it can be assigned values
    intersectionsOfTwoStreets.resize(streetIntersections1.size() + streetIntersections2.size());    
    
    //iterator used in subsequent set_intersection function 
    std::vector<int>::iterator it;
    
    //set_intersection is an STL function which finds elements common in two sets, and
    //returns them into a third vector
    it = std::set_intersection(streetIntersections1.begin(), 
                        streetIntersections1.end(), 
                        streetIntersections2.begin(), 
                        streetIntersections2.end(),
                        intersectionsOfTwoStreets.begin());
    
    //resizes intersectionsOfTwoStreets to the actual number of intersections
    intersectionsOfTwoStreets.resize(it-intersectionsOfTwoStreets.begin());
    
    return intersectionsOfTwoStreets; 
}

//Returns all street ids corresponding to street names that start with the given prefix
//The function should be case-insensitive to the street prefix. You should ignore spaces.
//For example, both "bloor " and "BloOrst" are prefixes to "Bloor Street East".
//If no street names match the given prefix, this routine returns an empty (length 0) 
//vector.
//You can choose what to return if the street prefix passed in is an empty (length 0) 
//string, but your program must not crash if street_prefix is a length 0 string.
std::vector<int> find_street_ids_from_partial_street_name(std::string street_prefix){

    //vector to be returned(value:street id's that max prefix)
    std::vector<int> streetIdsFromPartialStreetName;
    
    if (street_prefix.empty())//if empty is string
        return streetIdsFromPartialStreetName; //return empty vector
    
    //remove spaces from street_prefix
    street_prefix.erase(std::remove_if(street_prefix.begin(), street_prefix.end(), isspace), street_prefix.end());
    
    //convert prefix into all lowercase
    std::transform(street_prefix.begin(), street_prefix.end(), street_prefix.begin(), 
            [](unsigned char c){return std::tolower(c); });
     
    int prefixLength = street_prefix.length(); //length of prefix  
    
    //iterator to be used to traverse streetNames from lower bound 
    std::multimap<std::string, int>::iterator itr = streetNames.lower_bound(street_prefix); 
    
    //check if the street in map begins with the partial street name
    //first argument is street name (streetName --> Key)
    //second is street_prefix, third is length
    
    bool foundAllStreets = false;
 
    //go through all the streets after lower bound and check if prefix matches start up street name
    //if it does, add street name to streetIdsFromPartialStreetName
    //when it doesn't match, exit while loop
    while (!foundAllStreets){
        if((itr != streetNames.end()) && (isStreetName(itr -> first, street_prefix, prefixLength))){ //while streetNames begin with the same characters as the street_prefix       
            streetIdsFromPartialStreetName.push_back(itr -> second);
            itr++;
        }
        else{
            foundAllStreets = true;
        }
    }
    return streetIdsFromPartialStreetName;
}

//Returns the area of the given closed feature in square meters
//Assume a non self-intersecting polygon (i.e. no holes)
//Return 0 if this feature is not a closed polygon.
double find_feature_area(int feature_id){
    return featureAreaVector[feature_id];
}

//Returns the length of the OSMWay that has the given OSMID, in meters.
double find_way_length(OSMID way_id){
    return OSMWay_lengths.at(way_id);
}


//Populating StreetVector with street segments, intersections, and street name
void populateStreetVector(){
    //makes sure we can write into the streetVector
    streetVector.resize(getNumStreets());
    
    //vectors of sets which "remove" duplicate entries
    //at the end, each set will be copied into the appropriate intersections/segment vector
    std::vector<std::set<int>> intersections_by_street_vector;
    std::vector<std::set<int>> segments_by_street_vector;
    
    intersections_by_street_vector.resize(getNumStreets());
    segments_by_street_vector.resize(getNumStreets());
  
    //assigning street segments to their respective street
    for (unsigned i = 0; i < getNumStreetSegments(); i++){
        
        InfoStreetSegment segmentInfo; 
        
        //creating street segment info struct
        segmentInfo = getInfoStreetSegment(i);
        //inserts street segment to correct segments_by_street_vector set
        segments_by_street_vector[segmentInfo.streetID].insert(i);
        
        //inserts street segment to correct intersections_by_street_vector set
        intersections_by_street_vector[segmentInfo.streetID].insert(segmentInfo.to);
        intersections_by_street_vector[segmentInfo.streetID].insert(segmentInfo.from);
    }
    
    //copying the sets into the vectors in StreetVector
    for(unsigned i = 0; i < getNumStreets(); i++){
        //storing intersections
        streetVector[i].intersections.resize(intersections_by_street_vector[i].size());
        streetVector[i].intersections.assign(intersections_by_street_vector[i].begin(), intersections_by_street_vector[i].end()); 
        //storing street segments
        streetVector[i].streetSegments.resize(segments_by_street_vector[i].size());
        streetVector[i].streetSegments.assign(segments_by_street_vector[i].begin(), segments_by_street_vector[i].end());
    }

    //assigning street names
    for (unsigned i = 0; i < getNumStreets(); i++){
        streetVector[i].setStreetName(getStreetName(i));
    }
    
}

//Populating vector by key: feature ID and value: area
void populateFeatureAreaVector(){
    //iterate through total number of features
    for (unsigned featureIdx = 0; featureIdx < getNumFeatures(); featureIdx++){
        
        //variables to help with area calculations
        double featureArea = 0, sum1 = 0, sum2 = 0, sumOfLatPoints = 0;
        double latAvg, p1_y, p1_x, p2_y, p2_x;
        LatLon latLonIterator;
        
        int numOfFeaturePoints = getFeaturePointCount(featureIdx);
        
        //find first and last points and check if features is polygon or polyline
        LatLon firstPoint = getFeaturePoint(0, featureIdx);  
        LatLon lastPoint = getFeaturePoint(numOfFeaturePoints-1, featureIdx); 

        // If the first point and the last point (idx getFeaturePointCount-1) are NOT the same location, the feature is a polyline
        //the area is zero
        if ((firstPoint.lat() != lastPoint.lat()) || (firstPoint.lon() != lastPoint.lon())) {
            featureAreaVector.push_back(0);
            continue;
        }
    
        //calculate latavg based on sum of all the points.lat() in the feature
        for (unsigned featurePointIndex = 0; featurePointIndex < numOfFeaturePoints - 1; featurePointIndex++){
            latLonIterator = getFeaturePoint(featurePointIndex, featureIdx);
            sumOfLatPoints = sumOfLatPoints + latLonIterator.lat();
        }
        latAvg = sumOfLatPoints * DEGREE_TO_RADIAN/ (numOfFeaturePoints-1);
                    
        //iterate through feature points to get sums of crosshatch values of adjacent points
        for (unsigned featurePointIdx = 1; featurePointIdx < numOfFeaturePoints; featurePointIdx++){
            
            //Declare and initialize adjacent LatLon points
            LatLon previousPoint = getFeaturePoint(featurePointIdx-1, featureIdx);
            LatLon nextPoint = getFeaturePoint(featurePointIdx, featureIdx);
            
            //convert LatLon points into x y coordinates
            p1_y = previousPoint.lat()*DEGREE_TO_RADIAN *EARTH_RADIUS_METERS;
            p1_x = previousPoint.lon()*DEGREE_TO_RADIAN *EARTH_RADIUS_METERS*cos(latAvg);
            p2_y = nextPoint.lat()*DEGREE_TO_RADIAN *EARTH_RADIUS_METERS;
            p2_x = nextPoint.lon()*DEGREE_TO_RADIAN *EARTH_RADIUS_METERS*cos(latAvg);
            
            //perform crosshatch, add to sum1 
            sum1 = sum1 + std::abs(p1_x * p2_y);
            //perform crosshatch, add to sum2         
            sum2 = sum2 + std::abs(p1_y * p2_x);
        }   
        //subtract: sum1 - sum2, divide by two
        featureArea = std::abs((sum1-sum2)/2);
        //add feature area into vector
        featureAreaVector.push_back(featureArea);
    }
   
}


 //(helper function for Way Unordered Map) Populates OSMID_to_node unordered_map
//Finds the corresponding OSMid of each node, creates a vector with OSMid as key and node as value
void populateOSMID_to_node(){
    //NOTE:  The node indexing used here (i) is different from the node indexing
    //used by streetsDatabase
    
    for (int i = 0; i < getNumberOfNodes(); i++){
        //create a pointer that enables accessing the node's OSMID
        const OSMNode* nodePtr = getNodeByIndex(i);
        //stores to unordered_map: key as OSMID and value as the Node ID
        OSMID_to_node.insert({nodePtr->id(),i}); 
    }
}
// Populates the OSMWay_lengths unordered_map
void populateOSMWay_lengths(){
    
    double wayLength;
    
    std::vector<OSMID> nodesInWay;
    
    //Retrieves OSMNodes and calculate total distance, for each way
    for (unsigned i = 0; i < getNumberOfWays(); i++){
        //initialize length of way to 0 
        wayLength = 0.0; 
        //creates a pointer that enables accessing the node's OSMID
        const OSMWay* wayPtr = getWayByIndex(i);
        //copies over values from getWayMembers, and changes nodesInWay's size accordingly
        nodesInWay = getWayMembers(wayPtr);
        
        //if way is just a point, assigns length of zero and skips to the next for-loop iteration
        if (nodesInWay.size() < 2) { 
            OSMWay_lengths.insert({wayPtr->id(),0});
            continue;
        }

        //otherwise, calculates length of the Way       
        //retrieves coordinates of Way nodes:
        //OSMID -> OSM Node index -> Node pointer -> Lat Lon Coordinates
        
        //extracts first Node and retrieves corresponding LatLon (this is the "left-edge" of the way segment) 
        LatLon LatLon_left = getNodeCoords(getNodeByIndex(OSMID_to_node.at(nodesInWay[0]))); 
        
        for (unsigned j = 1; j < nodesInWay.size(); j++){
             //calculate LatLon for right-edge of way segment
             //retrieves the next OSMID -> OSM Node index -> Node pointer -> Lat Lon Coordinates
            LatLon LatLon_right = getNodeCoords(getNodeByIndex(OSMID_to_node.at(nodesInWay[j])));
            
            std::pair<LatLon, LatLon> waySegment(LatLon_left, LatLon_right); //pair left-edge LatLon and right-edge LatLon
            wayLength += find_distance_between_two_points(waySegment); //calculate distance and add it to the total length (wayLength)
            LatLon_left = LatLon_right; //shift right-edge of current way segment to become the left-edge of the next way segment
        }
        
        OSMWay_lengths.insert({wayPtr->id(),wayLength});
    }
    
}
//Creating nested vector. Outer: intersection IDs. Inner: Each intersection has a vector of its street segment indices.
void populateIntersectionStreetSegments(){
   
    //set size of outer vector to number of intersections
    intersectionStreetSegments.resize(getNumIntersections());
    
    //iterate through number of intersections
    for(unsigned i = 0; i < getNumIntersections(); i++){
        //create a vector containing street segments of this specific intersection
        std::vector<int> streetSegmentsOfAnIntersection;
        //iterate through number of street segments for this intersection
        for(unsigned j = 0; j < getIntersectionStreetSegmentCount(i); j++){
            //add the StreetSegment id to vector of street segments
            streetSegmentsOfAnIntersection.push_back(getIntersectionStreetSegment(i, j));
        }
        //push whole vector of street segments for this intersection into outer vector of intersections
        intersectionStreetSegments[i] = streetSegmentsOfAnIntersection;
    }
    
}
//Populating segment_lengths vector
void populate_segment_lengths(){
    
    segment_lengths.resize(getNumStreetSegments());
    
    double streetSegmentLength;
    //general segment info struct
    InfoStreetSegment segmentInfo;
    
    for(unsigned id = 0; id < getNumStreetSegments(); id++){
    
        streetSegmentLength = 0;
        
        segmentInfo = getInfoStreetSegment(id);

        //get number of curve points in street segment
        int numCurvePoints = segmentInfo.curvePointCount;
        //get starting point coordinates
        LatLon from = getIntersectionPosition(segmentInfo.from);

        //if there are zero curve points then "to" will be set to the end of the street segment
        if(numCurvePoints > 0){

            //need variable that holds "to" positions
            LatLon to;

            for(int i = 0; i < numCurvePoints; i++){
                //get the curvePoint Position (latlon)
                to = getStreetSegmentCurvePoint(i, id);

                //make a points pair to send to the find_distance_between_two_points function
                std::pair<LatLon, LatLon> length(from, to);

                //calculate distance
                streetSegmentLength += find_distance_between_two_points(length);

                //update "from" point
                from = to;
            }
        }

        //for the last "from-to" distance
        LatLon finalTo = getIntersectionPosition(segmentInfo.to);   

        std::pair<LatLon, LatLon> length(from, finalTo);

        streetSegmentLength += find_distance_between_two_points (length);
        
        segment_lengths[id] =  streetSegmentLength;
    }   
}

void populate_segment_travel_time(){
    
    segmentTravelTime.resize(getNumStreetSegments());
    
    double streetSegmentTravelTime, speedLimit_metersPerSec;
    //general segment info struct
    InfoStreetSegment segmentInfo;
    
    for (unsigned street_segment_id = 0; street_segment_id < getNumStreetSegments(); street_segment_id++){
        //reset travel time to 0
        streetSegmentTravelTime = 0;
        
        //Retrieve speed limit info in m/sec
        segmentInfo = getInfoStreetSegment(street_segment_id);
        speedLimit_metersPerSec = 1000.0*(segmentInfo.speedLimit)/ 3600.0;
        
        //calculate travel time (time = distance/velocity)
        streetSegmentTravelTime = (segment_lengths[street_segment_id] / speedLimit_metersPerSec);
        
        //put travel time into segmentTravelTime vector
        segmentTravelTime[street_segment_id] =  streetSegmentTravelTime;
    }   
}


//Populate intersectionCoordinates vector
//key: intersectionId value: LatLon coordinates
void populateIntersectionCoordinates() {
    
   for(unsigned i = 0; i < getNumIntersections(); i++){
       LatLon intersectionLatLon = getIntersectionPosition(i);
       intersectionCoordinates.push_back(intersectionLatLon);  
       
   }
}
//Populates streetNames multi-map
//Key -> Name  value -> streetID
void populateStreetNames() {
   //variable to hold street names
   std::string cachedStreetName;  
    
   for(unsigned streetIdx = 0; streetIdx < getNumStreets(); streetIdx++){ //loop through all streets of map
       //retrieve street name from street index
       cachedStreetName = getStreetName(streetIdx); 
       
       //convert name into a form without spaces
       cachedStreetName.erase(std::remove_if(cachedStreetName.begin(), cachedStreetName.end(), isspace), cachedStreetName.end());
       
       //convert name into all lowercase
       std::transform(cachedStreetName.begin(), cachedStreetName.end(), cachedStreetName.begin(), 
               [](unsigned char c){return std::tolower(c); });
               
       //add this string to the StreetNames multimap         
       streetNames.insert(std::make_pair(cachedStreetName, streetIdx));
   }
}

//compares characters of street prefix and street name in streetName Map. streetName must begin with same character as prefix
bool isStreetName(std::string streetName, std::string prefix, int prefixLength){
    //if the prefix exceeds the street Name
    if (prefixLength > streetName.length())
        return false;
    //check one by one if prefix and street name have same characters 
    for (unsigned i = 0; i < prefixLength; i++){
        
        if (prefix[i] != streetName[i])
            return false; //mismatch found
    }
    return true; //streetName begins with that prefix
}