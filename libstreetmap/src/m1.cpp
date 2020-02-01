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
#include <unordered_map> 
#include "streetStruct.h"
#include <iostream>
#include <vector>
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
//Vector --> key: [intersection ID] value: [pair of LatLon Coordinates]
std::vector<LatLon> intersectionCoordinates;
//Hashtable --> key: [Street Name] value: [Street Index]
std::unordered_map<std::string, int> streetNames;
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
//Populating intersection Coordinates vector
void populateIntersectionCoordinates();
//Populating names of street with street index
void populateStreetNames();
//------------------------------------------------------------------


bool load_map(std::string map_streets_database_filename) {
    bool load_successful = false; //Indicates whether the map has loaded 
                                  //successfully

    load_successful = loadStreetsDatabaseBIN(map_streets_database_filename);
    //Make sure this load_successful is updated to reflect whetherloading the map succeeded or failed
    
    std:: string map_streets_database_filename_OSM = map_streets_database_filename;
    
    //remove.street.bin from string to add .osm.bin
    if (!(map_streets_database_filename_OSM.empty())){
        map_streets_database_filename_OSM.resize(map_streets_database_filename.size()-11);
        map_streets_database_filename_OSM = map_streets_database_filename_OSM + "osm.bin";
    }
    //load corresponding OSM database
    loadOSMDatabaseBIN(map_streets_database_filename_OSM);
    
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

   
    return load_successful;
}


void close_map() {
    //Clean-up your map related data structures here
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
    if (street_segment_id < getNumStreetSegments()){
        return segment_lengths[street_segment_id];
    }
    else{
        return 0;
    }
}

//Returns the travel time to drive a street segment in seconds 
//(time = distance/speed_limit)
double find_street_segment_travel_time(int street_segment_id){
    
    //Retrieve speed limit info in m/sec
    InfoStreetSegment segmentInfo = getInfoStreetSegment(street_segment_id);
    double speedLimit_metersPerSec = 1000.0*(segmentInfo.speedLimit)/ 3600.0;
    
    //calculate travel time (time = distance/velocity)
    double streetSegmentTravelTime = (segment_lengths[street_segment_id] / speedLimit_metersPerSec);
    
    return streetSegmentTravelTime;
}

//Returns the nearest intersection to the given position
int find_closest_intersection(LatLon my_position){

    std::pair<LatLon, LatLon> path(my_position, intersectionCoordinates[0]);     
    int shortestDistance = find_distance_between_two_points(path);
    
    int distance;
    int closestIntersection;
    
    for (unsigned i = 1; i < intersectionCoordinates.size(); i++){
        path = std::pair<LatLon, LatLon> (my_position, intersectionCoordinates[i]);     
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
    std::vector<int> ssOfIntersection;
    
    int street_segment_count = getIntersectionStreetSegmentCount(intersection_id);
    
    for (int i = 0; i < street_segment_count; i++){
        ssOfIntersection.push_back(getIntersectionStreetSegment(intersection_id, i));
    } 
    return ssOfIntersection;
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
//street segment (hint: check for 1-way streets too)
//corner case: an intersection is considered to be connected to itself

//NOTE: not sure why 1-way streets are relevant or how intersection would be connected to itself
bool are_directly_connected(std::pair<int, int> intersection_ids){
    bool directlyConnected;
    InfoStreetSegment streetSegmentStruct;
    //iterate through segments connected to first intersection
    for (int i = 0; i < getIntersectionStreetSegmentCount(intersection_ids.first); i++){
        //create street segment struct for segments connected to intersection
        streetSegmentStruct = getInfoStreetSegment(getIntersectionStreetSegment(intersection_ids.first, i));
        //check if the street segment is connected to second intersection- if it is, set directlyConnected to true
        if ((streetSegmentStruct.from == intersection_ids.second) || (streetSegmentStruct.to == intersection_ids.second)){
            directlyConnected = true;
        }
        else{
            directlyConnected = false;
        }
    }
    
    return directlyConnected;
}

//Returns all intersections reachable by traveling down one street segment 
//from given intersection (hint: you can't travel the wrong way on a 1-way street)
//the returned vector should NOT contain duplicate intersections
std::vector<int> find_adjacent_intersections(int intersection_id){
    std::vector<int> adjacentIntersections;
    InfoStreetSegment streetSegmentStruct;
    //iterate through street segments for given intersection
    for (int i = 0; i < getIntersectionStreetSegmentCount(intersection_id); i++){
        //get street segment info struct
        streetSegmentStruct = getInfoStreetSegment(getIntersectionStreetSegment(intersection_id, i));
        //check if street is one way
        if (streetSegmentStruct.oneWay){
            //check if its 'from' intersection is the intersection_id (it is going to the adjacent intersection)
            if (streetSegmentStruct.from == intersection_id){
                //add to intersection its going 'to' to adjacentIntersection vector
                adjacentIntersections.push_back(streetSegmentStruct.to);
            }
        }
        else{   //not one-way
            //check if 'from' or 'to' intersection of street segment is intersection_id and push back the other one into adjacentIntersections
            if (streetSegmentStruct.from == intersection_id){
                adjacentIntersections.push_back(streetSegmentStruct.to);
            }
            else{
                adjacentIntersections.push_back(streetSegmentStruct.from);
            }
        }
        
    }
    
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
    std::vector<int> intersectionsOfTwoStreets; 
    
    int streetId1 = street_ids.first;
    int streetId2 = street_ids.second;
    std::vector<int> streetIntersections1 = streetVector[streetId1].intersections;
    std::vector<int> streetIntersections2 = streetVector[streetId2].intersections;
    
    for (unsigned i = 0; i < streetIntersections2.size(); i++){ //for each intersection of Street 2
        if (std::find(streetIntersections1.begin(), streetIntersections1.end(), streetIntersections2[i])!=streetIntersections1.end()) //check if Street 2's intersection exists in the entire vector of intersectionsOfStreet1
            intersectionsOfTwoStreets.push_back(streetIntersections2[i]); //add the common intersection to the vector
    }
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
    std::vector<int> streetIdsFromPartialStreetName;
    
    if (streetNames.count(street_prefix)!=0) //takes full (regular) name
        streetIdsFromPartialStreetName.push_back(streetNames[street_prefix]);
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
    
    streetVector.resize(getNumStreets());
    
    //vectors of sets which "removes" duplicate entries
    //at the end, each set will be copied into the appropriate intersections/segmentSegments vector
    std::vector<std::set<int>> intersections_by_street_vector;
    std::vector<std::set<int>> segments_by_street_vector;
    
    intersections_by_street_vector.resize(getNumStreets());
    segments_by_street_vector.resize(getNumStreets());
  
    //assigning street segments to their respective street
    for (unsigned i = 0; i < getNumStreetSegments(); i++){
        
        InfoStreetSegment segmentInfo; 
        
        //creating street segment info struct
        segmentInfo = getInfoStreetSegment(i);
        //accessing streetID in order to assign to proper segments_by_street_vector set element
        segments_by_street_vector[segmentInfo.streetID].insert(i);
        
        //adding intersections to the appropriate intersections_by_street_vector set element
        intersections_by_street_vector[segmentInfo.streetID].insert(segmentInfo.to);
        intersections_by_street_vector[segmentInfo.streetID].insert(segmentInfo.from);
    }
    
    //copying the intersections sets into the intersection vectors in StreetVector
    for(unsigned i = 0; i < getNumStreets(); i++){
        streetVector[i].intersections.assign(intersections_by_street_vector[i].begin(), intersections_by_street_vector[i].end()); 
        streetVector[i].streetSegments.assign(segments_by_street_vector[i].begin(), segments_by_street_vector[i].end());
    }
   
   
    //assigning street names
    for (unsigned i = 0; i < getNumStreets(); i++){
        streetVector[i].setStreetName(getStreetName(i));
    }
    
}

//Populating vector by key: feature ID and value: area
void populateFeatureAreaVector(){
    
    for (unsigned featureIdx = 0; featureIdx < getNumFeatures(); featureIdx++){
        
        double featureArea;
    
        int numOfFeaturePoints = getFeaturePointCount(featureIdx);
        LatLon firstPoint = getFeaturePoint(0, featureIdx);
        LatLon nextPoint = getFeaturePoint(numOfFeaturePoints-1, featureIdx); //at first, this is the last point of the polygon

        double sum1 = 0, sum2 = 0;
        
        // If the first point and the last point (idx getFeaturePointCount-1) are NOT the same location, the feature is a polyline
        //the area is zero
        if (!((firstPoint.lat() == nextPoint.lat()) && (firstPoint.lon() == nextPoint.lon()))) {
            featureAreaVector.push_back(0);
            continue;
        }
//        //area of line = 0
//        else if (numOfFeaturePoints < 4) {
//            featureAreaVector.push_back(0);
//            continue;
//        }
        for (unsigned featurePointIdx = 0; featurePointIdx < numOfFeaturePoints - 1; featurePointIdx++){
            nextPoint = getFeaturePoint(featurePointIdx+1, featureIdx);
            //perform crosshatch, add to sum1 
            sum1+= (firstPoint.lon()*nextPoint.lat());
            //perform crosshatch, add to sum2         
            sum2+= (firstPoint.lat()*nextPoint.lon());
            firstPoint = nextPoint; //shift second point of current line segment as first point of next line segment
        }   
        //subtract: sum1 - sum2
        //divide by two
        featureArea = (sum1-sum2)/2;
        if (featureArea < 0)
            featureArea *= (-1);
        //take positive value;
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
        intersectionStreetSegments.push_back(streetSegmentsOfAnIntersection);
    }
    
}
//Populating segment_lengths vector
void populate_segment_lengths(){
    
    segment_lengths.resize(getNumStreetSegments());
    
    double streetSegmentLength = 0;
    //general segment info struct
    InfoStreetSegment segmentInfo;
    
    for(unsigned id = 0; id < getNumStreetSegments(); id++){
    
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
                streetSegmentLength += find_distance_between_two_points (length);

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

void populateIntersectionCoordinates() {
    
   for(unsigned i = 0; i < getNumIntersections(); i++){
       LatLon intersectionLatLon = getIntersectionPosition(i);
       intersectionCoordinates.push_back(intersectionLatLon);  
       
   }
}

void populateStreetNames() {
    
   for(unsigned streetIdx = 0; streetIdx < getNumStreets(); streetIdx++){
       streetNames.insert({getStreetName(streetIdx), streetIdx});
   }
}