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
#include "streetStruct.h"

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
bool load_map(std::string map_streets_database_filename) {
    bool load_successful = false; //Indicates whether the map has loaded 
                                  //successfully

    std::cout << "load_map: " << map_streets_database_filename << std::endl;
    //
    //Load your map related data structures here
    //
//    load_successful = loadStreetsDatabaseBIN(map_streets_database_filename);
    //load_successful = true; //Make sure this is updated to reflect whether
                            //loading the map succeeded or failed
    
    streetStruct stubStreetStruct;
    
    InfoStreetSegment segmentInfo; 
    
    //creating "vector of Street Vectors" - S.G
    std::vector<streetStruct> streetVector(getNumStreets(), stubStreetStruct);
    
    //assigning street segments to their respective street
    for (unsigned i = 0; i < getNumStreetSegments(); i++){
        //creating street segment info struct
        segmentInfo = getInfoStreetSegment(i);
        //accessing streetID in order to assign to proper streetStruct vector element
        streetVector[segmentInfo.streetID].addStreetSegment(i);
        //assigning intersections to their respective street  -> duplicates will be dealt with...
        streetVector[segmentInfo.streetID].addIntersection(segmentInfo.to);
        streetVector[segmentInfo.streetID].addIntersection(segmentInfo.from);
         
        
    }
   
    //assigning street names
    for (unsigned i = 0; i < getNumStreets(); i++){
        streetVector[i].setStreetName(getStreetName(i));
    }
    
    
    return load_successful;
}


void close_map() {
    //Clean-up your map related data structures here
    
}

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
    
    double streetSegmentLength = 0;
    
    InfoStreetSegment segmentInfo = getInfoStreetSegment(street_segment_id);
    
    //get number of curve points in street segment
    int numCurvePoints = segmentInfo.curvePointCount;
    
    LatLon from = getIntersectionPosition (segmentInfo.from);
    
    //if there are zero curve points then "to" will be set to the end of the street segment
    if(numCurvePoints > 0){
        
        //need variable to hold "to" positions
        LatLon to;
        
        //***NOTE: HAVE TO ASK ABOUT NUMCURVEPOINTS INDICES (already posted on Piazza)***
        // -M
        for(int i = 0; i < numCurvePoints; i++){
            //get the curvePoint Position (latlon)
            to = getStreetSegmentCurvePoint(i, street_segment_id);
            
            //make a points pair to send to the find_distance_between_two_points function
            std::pair<LatLon, LatLon> length(from, to);
            
            //calculate distance
            streetSegmentLength += find_distance_between_two_points (length);\

            //update "from" point
            from = to;
        }
    }
    
    //for the last "from-to" distance
    LatLon finalTo = getIntersectionPosition(segmentInfo.to);   
    
    std::pair<LatLon, LatLon> length(from, finalTo);

    streetSegmentLength += find_distance_between_two_points (length);
    
    return streetSegmentLength;
}

//Returns the travel time to drive a street segment in seconds 
//(time = distance/speed_limit)
double find_street_segment_travel_time(int street_segment_id){
    
    //Retrieve speed limit info in m/sec
    InfoStreetSegment segmentInfo = getInfoStreetSegment(street_segment_id);
    double speedLimit_metersPerSec = 1000.0*(segmentInfo.speedLimit)/ 3600.0;
    
    //calculate travel time (time = distance/velocity)
    double streetSegmentTravelTime = (speedLimit_metersPerSec / find_street_segment_length(street_segment_id));
    
    return streetSegmentTravelTime;
}

//Returns the nearest intersection to the given position
int find_closest_intersection(LatLon my_position){
    int closestIntersection;
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
bool are_directly_connected(std::pair<int, int> intersection_ids){
    bool directlyConnected;
    InfoStreetSegment streetSegmentStruct;
    //iterate through segments connected to first intersection
    for (int i = 0; i < getIntersectionStreetSegmentCount(intersection_ids.first); i++){
        //create street segment struct for segments connected to intersection
        streetSegmentStruct = getInfoStreetSegment(getIntersectionStreetSegment(intersection_ids.first, i));
        //check if the street segment is connected to second intersection
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
    return adjacentIntersections;
}

//Returns all street segments for the given street
std::vector<int> find_street_segments_of_street(int street_id){
    std::vector<int> streetSegmentsOfStreet;
    return streetSegmentsOfStreet;
    
    
}
//Returns all intersections along the a given street
std::vector<int> find_intersections_of_street(int street_id){   
    std::vector<int> intersectionsOfStreet;
    
    std::vector<int> streetSegmentsofStreet = find_street_segments_of_street(street_id);
    if (streetSegmentsofStreet.size()==0) //if no street segments exist for the street
        return intersectionsOfStreet; //return empty vector
        
    intersectionsOfStreet.push_back(
        getInfoStreetSegment(streetSegmentsofStreet[0]).to //retrieve first street segment of street, get the "to" intersection id, add it to the intersectionsOfStreet vector
    );
    
    for (unsigned i = 0; i < streetSegmentsofStreet.size(); i++){ //for each street segment of the street found
        intersectionsOfStreet.push_back(
        getInfoStreetSegment(streetSegmentsofStreet[i]).from //retrieve street segment's "from" intersection id, add it to the intersectionsOfStreet vector
    );
    }
    
    return intersectionsOfStreet;
}

//Return all intersection ids for two intersecting streets
//This function will typically return one intersection id.
std::vector<int> find_intersections_of_two_streets(std::pair<int, int> street_ids){ 
    std::vector<int> intersectionsOfTwoStreets; 
    
    std::vector<int> intersectionsOfStreet1 = find_intersections_of_street(street_ids.first); //extract intersections of first street
    std::vector<int> intersectionsOfStreet2 = find_intersections_of_street(street_ids.second);//extract intersections of second street 
    for (unsigned i = 0; i < intersectionsOfStreet2.size(); i++){ //for each intersection of Street 2
        if (std::find(intersectionsOfStreet1.begin(), intersectionsOfStreet1.end(), intersectionsOfStreet2[i])!=intersectionsOfStreet1.end()) //check if Street 2's intersection exists in the entire vector of intersectionsOfStreet1
            intersectionsOfTwoStreets.push_back(intersectionsOfStreet2[i]); //add the common intersection to the vector
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
    return streetIdsFromPartialStreetName;
}

//Returns the area of the given closed feature in square meters
//Assume a non self-intersecting polygon (i.e. no holes)
//Return 0 if this feature is not a closed polygon.
double find_feature_area(int feature_id){
    double featureArea;
    
    int numOfFeaturePoints = getFeaturePointCount(feature_id);
    LatLon firstPoint = getFeaturePoint(0, feature_id);
    LatLon secondPoint = getFeaturePoint(numOfFeaturePoints-1, feature_id);
    
    if (!(firstPoint.lat()==secondPoint.lat()&&firstPoint.lon()==secondPoint.lon())) //if not closed polygon
        return 0;
    
    
    
    return featureArea;
}

//Returns the length of the OSMWay that has the given OSMID, in meters.
//To implement this function you will have to  access the OSMDatabaseAPI.h 
//functions.
double find_way_length(OSMID way_id){
//    
//    OSMWay* wayPtr = getWayByIndex(5); //dummy id. Must pass in index using way_id
//    std::vector<OSMID> OSMNodeIDs = getWayMembers(wayPtr);
////    
    double wayLength = 0;
//
//    if (OSMNodeIDs.size()==0)
//        return wayLength;
//        
//    LatLon OSMNodeLatLon1 = getNodeCoords(getNodeByIndex(OSMNodeIDs[0])); //extracts first OSM Node ID and retrieves corresponding LatLon (this is the "left-edge" of the way segment)
//    
//    for (unsigned i = 1; i < OSMNodeIDs.size(); i++){ //for the "right-edge" of each way segment
//        
//        LatLon OSMNodeLatLon2 = getNodeCoords(getNodeByIndex(OSMNodeIDs[i])); //calculate LatLon for right-edge of way segment
//        
//        std::pair<LatLon, LatLon> waySegment(OSMNodeLatLon1, OSMNodeLatLon2); //pair left-edge LatLon and right-edge LatLon
//        wayLength += find_distance_between_two_points(waySegment); //calculate distance and add it to the total length (wayLength)
//        OSMNodeLatLon1 = OSMNodeLatLon2; //shift right-edge of way segment to become the left-edge of the next way segment
//    }
    return wayLength;
}