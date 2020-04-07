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
#include "segmentStruct.h"

//-----Global Variables------------------------------------------
//Vector --> key: [streetID] value: [StreetStruct]
//StreetStruct --> Members: [street name, street segments, intersections]
std::vector<streetStruct> StreetVector;

//Vector --> key: [intersection ID] value: [streetSegmentsVector]
std::vector<std::vector<int>> IntersectionStreetSegments;

//Hashtable --> key: [OSMID] value: [int]
std::unordered_map<OSMID, int> OSMID_to_node;

//Hashtable --> key: [OSMway] value: [length of way]
std::unordered_map<OSMID, double> OSMWay_lengths;

//Vector --> key: [feature ID] value: [Area]
std::vector<double> FeatureAreaVector;

//Vector --> key: [segment ID] value: [length]
std::vector<double> SegmentLengths;

//Vector --> key: [segment ID] value: [travel_time]
std::vector<double> SegmentTravelTime;

//Vector --> key: [intersection ID] value: [LatLon Coordinates]
std::vector<LatLon> IntersectionCoordinates;

//Multimap --> key: [Street Name] value: [Street Index]
std::multimap<std::string, int> StreetNames;

//Vector --> key: [segment ID] value: [segmentStruct]
std::vector<segmentStruct> segmentHighlight;

std::list<int> segmentsHighlighted; //for keeping track of which segment

std::string MapName;

float MaxSpeedLimit;
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
//Populating SegmentLengths
void populateSegmentLengths();
//Populating segment_travel_time
void populateSegmentTravelTime();
//Populating intersection Coordinates vector
void populateIntersectionCoordinates();
//Populating names of street with street index
void populateStreetNames();
//Populating street segment highlight
void populateSegmentHighlight();
//returns true if a given streetName begins with the same characters as a given prefix
bool isStreetName(std::string streetName, std::string prefix, int prefixLength);
//Used to extract map name as City Country, used in graphics (M3)
std::string getMapName(std::string fullpath);
//------------------------------------------------------------------

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
//
//Load map also calls functions to populate global data structures which will be used to make other functions faster
bool load_map(std::string map_streets_database_filename) {
    
    bool load_successful;

    //Check if streets database bin file loads successfully
    load_successful = loadStreetsDatabaseBIN(map_streets_database_filename);
    
    //If streets database loaded, create string for OSM filename and load OSM database
    if(load_successful){
        
        //String to load OSM filename
        std:: string map_streets_database_filename_OSM = map_streets_database_filename;
        
        //Remove ".street.bin" from string, concatenate ".osm.bin"
        if (!(map_streets_database_filename_OSM.empty())){
            map_streets_database_filename_OSM.resize(map_streets_database_filename.size()-11);
            map_streets_database_filename_OSM = map_streets_database_filename_OSM + "osm.bin";
        }
        //Load corresponding OSM database
        load_successful = loadOSMDatabaseBIN(map_streets_database_filename_OSM);
    }
    
    //If load_successful is still true, populate data structures
    if (load_successful){
        
        std::cout << map_streets_database_filename << "\n";
        //update the global map name variable
        MapName = getMapName(map_streets_database_filename);
        
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
    
        //Populating IntersectionCoordinates vector
        populateIntersectionCoordinates();
    
        //Populating street names hash table
        populateStreetNames();
    
        //Populate segment lengths
        populateSegmentLengths();
    
        //Populate segment travel times;
        populateSegmentTravelTime();
        
        //Populate segment highlights
        populateSegmentHighlight();
    
    }
    return load_successful;
}


void close_map() {
    
    //Clean-up your multi map here
    StreetNames.clear();
    
    StreetVector.clear();
    
    IntersectionStreetSegments.clear();
    
    OSMID_to_node.clear();
    
    OSMWay_lengths.clear();
    
    FeatureAreaVector.clear();
    
    SegmentLengths.clear();
    
    SegmentTravelTime.clear();
    
    IntersectionCoordinates.clear();
    
    //Call close functions from StreetsDatabase API
    closeStreetDatabase(); 
    closeOSMDatabase();
}

//Return: distance between 2 points (metres)
//Finds distance in metres between 2 LatLon points by converting them to x y coordinates
//Use distance formula to get distance and then scale it by multiplying by radius of the earth
double find_distance_between_two_points(std::pair<LatLon, LatLon> points){
    
    double latAvg, p1_y, p1_x, p2_y, p2_x, distanceBetweenTwoPoints;
    
    //Calculate average latitude of points
    latAvg = (points.first.lat()*DEGREE_TO_RADIAN + points.second.lat()*DEGREE_TO_RADIAN ) * 0.5;
    
    //Convert latitude and longitude to x and y coordinates
    p1_y = points.first.lat()*DEGREE_TO_RADIAN ;
    p1_x = points.first.lon()*DEGREE_TO_RADIAN *cos(latAvg);
    p2_y = points.second.lat()*DEGREE_TO_RADIAN ;
    p2_x = points.second.lon()*DEGREE_TO_RADIAN *cos(latAvg);
    
    
    //Use distance formula to find distance and then scale to size of Earth
    distanceBetweenTwoPoints = EARTH_RADIUS_METERS * sqrt(pow(p2_y - p1_y , 2)+ pow(p2_x - p1_x , 2));
  
    return distanceBetweenTwoPoints;
}

//Return: street segment length (metres)
//Uses SegmentLengths vector with index segment id argument (if it exists)
double find_street_segment_length(int street_segment_id){
    
    if ((street_segment_id < getNumStreetSegments()) && (street_segment_id >= 0)){
        return SegmentLengths[street_segment_id];
    }
    else{
        std::cout<<"Invalid street segment ID";
        return 0;
    }
}

//Return: travel time (in seconds)
//Uses SegmentTravelTime vector with index segment id argument (if it exists)
double find_street_segment_travel_time(int street_segment_id){
    
    if ((street_segment_id < getNumStreetSegments()) && (street_segment_id >= 0)){
        return SegmentTravelTime[street_segment_id];
    }
    else{
        std::cout<<"Invalid street segment ID";
        return 0;
    }
}

int find_closest_intersection(LatLon my_position){
//Function iterates through each intersection and compares its distance to my_position
//The closest ID is returned as closestIntersection 
//Ids are accessed from the IntersectionCoordinates structure

    //calculates distance to intersection 0, to begin comparison
    std::pair<LatLon, LatLon> path(my_position, IntersectionCoordinates[0]);     
    int shortestDistance = find_distance_between_two_points(path);
    
    int distance;
    int closestIntersection = 0;
    
    //start iterating distance to intersection 1, since we start with distance to intersection 0
    for (unsigned i = 1; i < getNumIntersections(); i++){
        //store next intersection into the path pair
        path.second = IntersectionCoordinates[i];     
        
        distance = find_distance_between_two_points(path);
        
        if (distance < shortestDistance){
            
            shortestDistance = distance;
            closestIntersection = i;
        }
    }
    
    return closestIntersection;
}

//Returns: vector of street segments of an intersection
//Uses intersectionStreetSegments vector with intersection id argument (if it exists)
std::vector<int> find_street_segments_of_intersection(int intersection_id){
    
    if ((intersection_id < getNumIntersections()) && (intersection_id >= 0)){
        return IntersectionStreetSegments[intersection_id];
    }
    else{
        std::cout<<"Invalid intersection ID";
        std::vector<int> emptyVector;
        return emptyVector;
    }
}

//Returns: vector of street names of intersection

std::vector<std::string> find_street_names_of_intersection(int intersection_id){
    
    //container for intersection street names
    std::vector<std::string> streetNamesOfIntersection;
    
    if ((intersection_id >= getNumIntersections()) || (intersection_id < 0)){
        std::cout<<"Invalid intersection ID";
        return streetNamesOfIntersection;
    }
    
    //Call find_street_segments_of_intersection to get segments of interest
    std::vector<int> streetsegmentOfIntersection = find_street_segments_of_intersection(intersection_id);
    
    //Iterate through all street segments to find street names
    for(std::vector<int>::iterator it = streetsegmentOfIntersection.begin(); it != streetsegmentOfIntersection.end(); ++it){
        
        //Retrieve current street segment info struct
        //Get it's streetId and push into return vector
        InfoStreetSegment streetSegmentInfoStruct = getInfoStreetSegment(*it);
        streetNamesOfIntersection.push_back(getStreetName(streetSegmentInfoStruct.streetID));
    }
    
    return streetNamesOfIntersection;
}

//Returns: true if you can get from intersection_ids.first to intersection_ids.second using a single street segment
//Compares street segment vectors of two intersections to find any commonalities
//Checks if going from intersection1 to intersection2 is allowed (i.e. legal- could be one-way)
bool are_directly_connected(std::pair<int, int> intersection_ids){    
    
    //function compares street segment vectors of two intersections to find any commonalities
    //then checks if going from intersection1 to intersection2 is allowed (i.e. legal)
    int maxIntersectionID = getNumIntersections(); 
 
    int intersection1 = intersection_ids.first; 
    int intersection2 = intersection_ids.second;
    
    if (intersection1 >= maxIntersectionID ||intersection2 >= maxIntersectionID ||intersection1<0||intersection2<0){
        std::cout<<"One or more Intersection IDs is invalid";
        return false;
    }
    //extracting the street segment vector of both intersections
  
    //Get the street segment vector of both intersections
    std::vector<int> intersection1_segments = IntersectionStreetSegments[intersection1];
    std::vector<int> intersection2_segments = IntersectionStreetSegments[intersection2];
    
    //Corner case: "to" and "from" are the same intersection
    if(intersection1 == intersection2) 
        return true;
    
    //Vector to check if two intersections share a street segment
    std::vector<int> commonStreetSegments;
    
    //Resizing commonStreetSegments so that it can be assigned values
    commonStreetSegments.resize(intersection1_segments.size() + intersection2_segments.size());  
   
    //Set_intersection is an STL function which finds elements common in two sets, and puts them into a third vector (last argument)  
    //Returns iterator which points to last match element added to commonStreetSegments
    std::vector<int>::iterator it;
    it = std::set_intersection(intersection1_segments.begin(), 
                        intersection1_segments.end(), 
                        intersection2_segments.begin(), 
                        intersection2_segments.end(),
                        commonStreetSegments.begin());
    
    //Resize commonStreetSegments to the actual number of intersections
    commonStreetSegments.resize(it-commonStreetSegments.begin());
    
    //Check if no common segments found
    if(commonStreetSegments.empty() == true)
        return false;
    
    //Struct to check if segments are one-way
    InfoStreetSegment info;
    
    for(unsigned i = 0; i < commonStreetSegments.size(); ++i){
        
        info = getInfoStreetSegment(commonStreetSegments[i]);
        
        //Check if street is one-way only
        if(info.oneWay == true){
            //If street is one-way, ensure that intersection1 is "from"
            if(info.from == intersection1)
                return true;
        }
        //Connecting street is not one-way
        else 
            return true;
    }
    //if only connecting street segments are one-way
    return false;
}


//Return: vector of all intersections reachable by traveling across one street segment from given intersection
std::vector<int> find_adjacent_intersections(int intersection_id){
    
    InfoStreetSegment info;

    std::vector<int> adjacentIntersections;
    std::set<int> adjacentIntersectionsSet;
    
    if ((intersection_id >= getNumIntersections()) || (intersection_id < 0)){
        std::cout<<"Invalid intersection ID";
        return adjacentIntersections;
    }
    
    //AdjacentIntersections set will "remove" duplicate entries
    //Each set will be copied into the adjacentIntersections vector
 
    //Retrieve vector of all the street segments of given intersection
    std::vector<int>  connectedStreetSegments = IntersectionStreetSegments[intersection_id];
    
    for(std::vector<int>::iterator it = connectedStreetSegments.begin(); it < connectedStreetSegments.end(); ++it){
        
        //Check if street is one way
        info = getInfoStreetSegment(*it);
        if (info.oneWay){
            //Check if 'from' intersection is the intersection_id and it is going TO the adjacent intersection; then add it to adjacentIntersections
            if (info.from == intersection_id){
                adjacentIntersectionsSet.insert(info.to);
            }
        }
        else{
            //Check if 'from' or 'to' intersection of street segment is intersection_id and push back the other into adjacentIntersections
            if (info.from == intersection_id)
                adjacentIntersectionsSet.insert(info.to);
         
            else
             adjacentIntersectionsSet.insert(info.from);
        }
    } 
    
    //Resize adjacentIntersections vector to size of adjacentIntersections set and then assign values from set into vector
    adjacentIntersections.resize(adjacentIntersectionsSet.size());
    adjacentIntersections.assign(adjacentIntersectionsSet.begin(), adjacentIntersectionsSet.end()); 
        
    return adjacentIntersections;
}

//Return: vector of all street segments for the given street
//Uses streetVector vector with street id argument (if it exists)
std::vector<int> find_street_segments_of_street(int street_id){
    
    if ((street_id >= getNumStreets()) || (street_id < 0)){
        std::cout<<"Invalid Street ID";
        std::vector<int> emptyVector;
        return emptyVector;
    }

    return StreetVector[street_id].streetSegments;
}
//Return: vector all intersections of the a given street
//Uses streetVector vector with street id argument (if it exists)
std::vector<int> find_intersections_of_street(int street_id){ 
    
    if ((street_id >= getNumStreets()) || (street_id < 0)){
        std::cout<<"Invalid Street ID";
        std::vector<int> emptyVector;
        return emptyVector;
    }
    
    return StreetVector[street_id].intersections;
}

//Return: vector of all intersection ids for two intersecting streets
//This function will typically return one intersection id.
std::vector<int> find_intersections_of_two_streets(std::pair<int, int> street_ids){
    //Vector to be returned (list of all intersections)
    std::vector<int> intersectionsOfTwoStreets;
    
    //Extracting the streedIds
    int streetId1 = street_ids.first;
    int streetId2 = street_ids.second;
    
    int maxStreetID = getNumStreets(); 
    
    if (streetId1 >= maxStreetID ||streetId2 >= maxStreetID ||streetId1<0||streetId2<0){
        std::cout<<"One or more Street IDs is invalid";
        return intersectionsOfTwoStreets;
    }
    //Extracting the street intersection vector of both streets
    std::vector<int> streetIntersections1 = StreetVector[streetId1].intersections;
    std::vector<int> streetIntersections2 = StreetVector[streetId2].intersections;
    
    //resizing intersectionsOfTwoStreets so that it can be assigned values
    intersectionsOfTwoStreets.resize(streetIntersections1.size() + streetIntersections2.size());    
    
    //Iterator used in subsequent set_intersection function 
    std::vector<int>::iterator it;
    
    //Set_intersection is an STL function which finds elements common in two sets, and
    //Returns them into a third vector
    it = std::set_intersection(streetIntersections1.begin(), 
                        streetIntersections1.end(), 
                        streetIntersections2.begin(), 
                        streetIntersections2.end(),
                        intersectionsOfTwoStreets.begin());
    
    //Resizes intersectionsOfTwoStreets to the actual number of intersections
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
    
    if (street_prefix.empty()){//if empty is string
        std::cout<<"Empty street prefix";
        return streetIdsFromPartialStreetName; //return empty vector
    }
    //remove spaces from street_prefix
    street_prefix.erase(std::remove_if(street_prefix.begin(), street_prefix.end(), isspace), street_prefix.end());
    
    //convert prefix into all lowercase
    std::transform(street_prefix.begin(), street_prefix.end(), street_prefix.begin(), 
            [](unsigned char c){return std::tolower(c); });
     
    int prefixLength = street_prefix.length(); //length of prefix  
    
    //iterator to be used to traverse StreetNames from lower bound 
    std::multimap<std::string, int>::iterator itr = StreetNames.lower_bound(street_prefix); 
    
    //check if the street in map begins with the partial street name
    //first argument is street name (streetName --> Key)
    //second is street_prefix, third is length
    
    bool foundAllStreets = false;
 
    //go through all the streets after lower bound and check if prefix matches start up street name
    //if it does, add street name to streetIdsFromPartialStreetName
    //when it doesn't match, exit while loop
    while (!foundAllStreets){
        if((itr != StreetNames.end()) && (isStreetName(itr -> first, street_prefix, prefixLength))){ //while StreetNames begin with the same characters as the street_prefix       
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
    
    if ((feature_id >= getNumFeatures()) || (feature_id < 0)){
        std::cout<<"Invalid Feature ID";
        return 0;
    }
    
    return FeatureAreaVector[feature_id];
}

//Returns the length of the OSMWay that has the given OSMID, in meters.
double find_way_length(OSMID way_id){
    return OSMWay_lengths.at(way_id);
}


//Populating StreetVector with street segments, intersections, and street name
void populateStreetVector(){
    //makes sure we can write into the StreetVector
    StreetVector.resize(getNumStreets());
    
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
        StreetVector[i].intersections.resize(intersections_by_street_vector[i].size());
        StreetVector[i].intersections.assign(intersections_by_street_vector[i].begin(), intersections_by_street_vector[i].end()); 
        //storing street segments
        StreetVector[i].streetSegments.resize(segments_by_street_vector[i].size());
        StreetVector[i].streetSegments.assign(segments_by_street_vector[i].begin(), segments_by_street_vector[i].end());
    }

    //assigning street names
    for (unsigned i = 0; i < getNumStreets(); i++){
        StreetVector[i].setStreetName(getStreetName(i));
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
            FeatureAreaVector.push_back(0);
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
        FeatureAreaVector.push_back(featureArea);
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
    IntersectionStreetSegments.resize(getNumIntersections());
    
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
        IntersectionStreetSegments[i] = streetSegmentsOfAnIntersection;
    }
    
}
//Populating SegmentLengths vector
void populateSegmentLengths(){
    
    SegmentLengths.resize(getNumStreetSegments());
    
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
        
        SegmentLengths[id] =  streetSegmentLength;
    }   
}

void populateSegmentTravelTime(){
    
    SegmentTravelTime.resize(getNumStreetSegments());
    
    double streetSegmentTravelTime, speedLimit_metersPerSec;
    //general segment info struct
    InfoStreetSegment segmentInfo;
    
    for (unsigned street_segment_id = 0; street_segment_id < getNumStreetSegments(); street_segment_id++){
        //reset travel time to 0
        streetSegmentTravelTime = 0;
        
        //Retrieve speed limit info in m/sec
        segmentInfo = getInfoStreetSegment(street_segment_id);
        speedLimit_metersPerSec = 1000.0*(segmentInfo.speedLimit)/ 3600.0;
        
        //global variable used in m3 heuristics
        if(segmentInfo.speedLimit > MaxSpeedLimit)
            MaxSpeedLimit = segmentInfo.speedLimit;
        
        //calculate travel time (time = distance/velocity)
        streetSegmentTravelTime = (SegmentLengths[street_segment_id] / speedLimit_metersPerSec);
        
        //put travel time into SegmentTravelTime vector
        SegmentTravelTime[street_segment_id] =  streetSegmentTravelTime;
    }   
}


//Populate IntersectionCoordinates vector
//key: intersectionId value: LatLon coordinates
void populateIntersectionCoordinates() {
    
   for(unsigned i = 0; i < getNumIntersections(); i++){
       LatLon intersectionLatLon = getIntersectionPosition(i);
       IntersectionCoordinates.push_back(intersectionLatLon);  
       
   }
}
//Populates StreetNames multi-map
//Key -> Name  value -> streetID
void populateStreetNames() {
   //variable to hold name of a single street, with removed spaces and characters all lowercase 
   std::string cachedStreetName;  
    
   for(unsigned streetIdx = 0; streetIdx < getNumStreets(); streetIdx++){ //loop through all streets of map
       //retrieve street name from street index
       cachedStreetName = getStreetName(streetIdx); 
       
       //remove spaces from streetName string
       cachedStreetName.erase(std::remove_if(cachedStreetName.begin(), cachedStreetName.end(), isspace), cachedStreetName.end());
       
       //convert name into all lowercase
       std::transform(cachedStreetName.begin(), cachedStreetName.end(), cachedStreetName.begin(), 
               [](unsigned char c){return std::tolower(c); });
               
       //add this string to the StreetNames multimap key --> [name], value --> [id]          
       StreetNames.insert(std::make_pair(cachedStreetName, streetIdx));

    }
}

//Populates SegmentHighlight vector
//Key -> segmentID value -> segmentStruct
void populateSegmentHighlight(){
    //set size of global struct
    segmentHighlight.resize(getNumStreetSegments());
    // Set value = segmentStruct with bools which are false in constructor because initially no directions are given
    //bools will be set to true in m3.cpp find_path function
    segmentStruct seg;
    
    //iterate through street segments 
    std::fill (segmentHighlight.begin(), segmentHighlight.end(), seg);
//    for (unsigned i = 0; i < getNumStreetSegments(); i++){
//        segmentHighlight[i] = seg;
//    }
}

//compares characters of street prefix and street name in streetName Map. streetName must begin with same character as prefix
//returns true if the given streetName begins with the prefix
//otherwise, returns false
bool isStreetName(std::string streetName, std::string prefix, int prefixLength){
    //if the prefix exceeds the street Name
    if (prefixLength > streetName.length())
        return false;
    //check characters one by one if prefix and street name have same characters 
    for (unsigned i = 0; i < prefixLength; i++){
        
        if (prefix[i] != streetName[i])
            return false; //mismatch found
    }
    return true; //streetName begins with that prefix
}

//Used to extract map name as city[SPACE]country, used in graphics (M3)
std::string getMapName(std::string fullpath){
    
    std::string city, country;
    
    //ignore the file directory, until the actual map name (format: city_country)
    std::size_t pos = fullpath.find("/");
    
    while (pos != std::string::npos){
        //remove all text leading up to the first "/"
        fullpath = fullpath.substr(pos);
        //remove "/"
        fullpath.erase(fullpath.begin());
        //find the next "/"
        pos = fullpath.find("/");
    }
    
    fullpath = fullpath.substr(0,fullpath.find("."));
    
    city = fullpath.substr(0,fullpath.find("_"));
    
    country = fullpath.substr(fullpath.find("_") + 1);
       
    fullpath = city + " " + country;
    
    return fullpath;
}