#include "m1.h"
#include "globals.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "m2.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "ezgl/point.hpp"
#include "intersection_data.h"
#include "poiStruct.h"
#include "poiStruct.cpp"
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <sstream>

/************  GLOBAL VARIABLES  *****************/
enum RoadType {
    unclassified = 0,
    motorway, 
    trunk ,
    primary,
    secondary, 
    tertiary, 
    residential
};
//nightmode street color scheme
ezgl::color Colour_unclassified(114, 111, 85, 255); //yellow (1 = least opaque)
ezgl::color Colour_motorway(100, 38, 7); // orange (3 = lightest)
ezgl::color Colour_trunk(129, 68, 6, 255); // orange (2)
ezgl::color Colour_primary(174, 133, 40, 255); // orange (1 = darkest)
ezgl::color Colour_secondary(154, 92, 0, 255); //yellow (3 = lightest)
ezgl::color Colour_tertiary(152, 122, 0, 255); // yellow (2 )
ezgl::color Colour_residential(114, 111, 85, 255); // yellow (1 = darkest)

//Vector --> key: [intersection ID] value: [intersection_data struct]
std::vector<intersection_data> intersections;

//Hashtable --> key: [OSMway] value: [Road Type (e.g. 0 = Unknown, 1 = motorway...)]
std::unordered_map<OSMID, RoadType> WaybyRoadType;

//Hashtable --> key: [feature id (POLYGONS ONLY)] value: [centroid (x,y)]
std::unordered_map< int, ezgl::point2d > FeatureCentroids;

//Vector --> key: [feature id] value: [vector of featurepoints in (x,y) coordinates --> std::vector<ezgl::point2d>]
std::vector<std::vector<ezgl::point2d>> Featurepoints_xy;

//Vector --> key: [type], value = vector: [point of interest structs]
std::vector<std::vector<poiStruct>> PointsOfInterest (3);

//Vector --> key: [Feature Type (e.g. 0 = Unknown, 1 = Park...)] value: [std::vector<int> feature IDs]
std::vector<std::vector<int>> FeatureIds_byType;

//Vector --> key: [n/a] value: [std::vector<int> intersections highlighted]
std::vector<int> Highlighted_intersections;

//variable used to determine drawing thresholds
double scale_factor = 1;

//average latitude of map, value set in draw_map_blank_canvas
float latAvg; 
//corners of the map, values set in draw_map
double max_lat;
double min_lat;
double max_lon;
double min_lon;

/************  FUNCTION DECLARATIONS  ***********/
void draw_map_blank_canvas ();
void draw_main_canvas (ezgl::renderer *g);

//  CONVERSIONS //
double lon_from_x (double x);
double lat_from_y (double x);
double y_from_lat (double lat);
double x_from_lon (double lon);
std::pair < double, double > latLonToCartesian (LatLon latLonPoint);
double getRotationAngle(std::pair <double, double> xyFrom, std::pair <double, double> xyTo);

//  DRAWING //
void drawFeature_byType(int feature_type, ezgl::renderer *g);
void drawFeatures(ezgl::renderer *g);
void draw_feature_names(ezgl::renderer *g);
void draw_streets(ezgl::renderer *g);
void draw_street_name(ezgl::renderer *g, std::pair<double, double> & xyFrom, std::pair<double, double> & xyTo, double& segmentLength, std::string& streetName, bool oneWay);
void draw_intersections(ezgl::renderer *g);  
void clearIntersection_highlights();
//int intersectionThreshold(int interIndex);

// POPULATING GLOBAL VARIABLES //
void populatePointsOfInterest();
void populateWaybyRoadType();
void populateFeatureIds_byType();
void populateFeaturepoints_xy();
void populateFeatureCentroids();

ezgl::point2d compute2DPolygonCentroid(std::vector<ezgl::point2d> &vertices, double& area);
ezgl::point2d find_PolyLine_Middle(int featureId);
double feature_max_width (int numPoints, int featureId);


void act_on_mouse_click( ezgl:: application* app, GdkEventButton* event, double x_click, double y_click);
void find_button(GtkWidget *widget, ezgl::application *application);
void initial_setup(ezgl::application *application, bool /*new_window*/);

//void find_map_bounds(double& max_lat, double& min_lat, double& max_lon, double& min_lon);

/************************************************/


/**
 * This function sets up fundamental variables for drawing the map, 
 * such as max & min lat and lons of the map for developing a scale for the map and 
 * populating global variables.
 * After, this function calls draw_map_blank_canvas(), which manages setting up the application & environment for the map to function
 */

void draw_map(){
    
    //find the maximum and minimum lat & lons of the map based on intersections 
    max_lat = getIntersectionPosition(0).lat(); 
    min_lat = max_lat;
    max_lon = getIntersectionPosition(0).lon();
    min_lon = max_lon;
    
    //initialize global intersections vector
    int numIntersections = getNumIntersections();
    intersections.resize(numIntersections);
    
    //variable used in (long,lat) -> (x,y) conversion,  required for finding latAvg
    double sumLat = 0;
    
    //populate the intersections vector, calculates min and max lat/lon positions, and determines total sum of lats for latAvg
    for(int i = 0; i < numIntersections; i++){

        //get position from streetsdatabaseAPI function
        intersections[i].position = getIntersectionPosition(i);
        
        //get name from streetsdatabaseAPI function
        intersections[i].name = getIntersectionName(i);
        
        //compare the lat/lon positions to update max/min
        max_lat = std::max(max_lat, intersections[i].position.lat());
        min_lat = std::min(min_lat, intersections[i].position.lat());
        max_lon = std::max(max_lon, intersections[i].position.lon());
        min_lon = std::min(min_lon, intersections[i].position.lon());
        
        //update the sum of latitude
        sumLat = sumLat + intersections[i].position.lat();
    }
        
    //convert latsum to radians
    sumLat = sumLat * DEGREE_TO_RADIAN;
    
    //update the global variable with calculated lat average
    latAvg = sumLat/(numIntersections);
    
    //convert min & max lat/lons
    min_lon = x_from_lon(min_lon);
    min_lat = y_from_lat(min_lat);
    max_lon = x_from_lon(max_lon);
    max_lat = y_from_lat(max_lat);
    
    //populate all global variables
    populateWaybyRoadType();
    populateFeatureIds_byType();
    populatePointsOfInterest();
    populateFeaturepoints_xy();
    draw_map_blank_canvas();
}

/*
 * Sets up environment of application for map to reside in
 */
void draw_map_blank_canvas (){       
    ezgl::application::settings settings;
    settings.main_ui_resource = "libstreetmap/resources/main.ui";
    settings.window_identifier = "MainWindow";
    settings.canvas_identifier = "MainCanvas";

    ezgl::application application(settings);
    
   
    ezgl::rectangle initial_world({min_lon, min_lat},{max_lon, max_lat}); 
    
    application.add_canvas("MainCanvas", draw_main_canvas, initial_world);
    application.run(initial_setup,act_on_mouse_click,NULL,NULL);
}

/*
 * Draws each major map components in the following order:
 * (1) Features
 * (2) Streets
 * (3) Points of Interest
 * (4) Intersections
 * 
 */
void draw_main_canvas (ezgl::renderer *g){
    
    //Determine the amount that screen is zoomed in 
    ezgl::rectangle zoom_rect = g->get_visible_world(); //retrieves in xy, the coordinates of the visible screen
    double zoom = zoom_rect.width(); //width of zoom rectangle
    //global variable: ratio of zoom rectangle  width with map scale's width to develo percentage of the full map shown in the window
    scale_factor = zoom/(max_lon - min_lon); //Percentage. 1 = 100% (Auto fit). 0.05 = very zoomed in
    
    //Use these for creating thresholds for zooming
//    std::cout<<"\nscale_factor: "<<scale_factor;
//    std::cout<<"\nzoom: "<<zoom;
    
    
    //Drawing Background
    //***********************************************************************************
    g->draw_rectangle({min_lon, min_lat},{max_lon, max_lat});
    g->set_color (32, 32, 32, 255);
    g->fill_rectangle({min_lon,min_lat}, {max_lon, max_lat});
    
//  Draw all types of features  
    drawFeatures(g);
    
//   Drawing Streets
    draw_streets(g);

    //Draw Feature Names
    draw_feature_names(g);
     
    //Draw POIs
//    //***********************************************************************************
//    
//    bool enable_poi = true;
//    if (scale_factor > 0.5)
//        enable_poi = false;
//    
//    //Extract vectors from PointsOfInterest vector to make it easier to parse through each type separately
//    std::vector<poiStruct> police = PointsOfInterest[0];
//    std::vector<poiStruct> hospitals = PointsOfInterest[1];
//    std::vector<poiStruct> fire_station = PointsOfInterest[2];
//    
//    //Declare iterator to go through each vector
//    std::vector<poiStruct>::iterator it = police.begin();
//    
//    //Variables to extract data from poi struct
//    poiStruct poiData;
//    std::pair<double,double> xyCoordinates;
//    std::string poiName;
//    
//    //loop through police vector, extract data and draw names of police stations on map
//    while(it != police.end()){
//        poiData = *it;
//        xyCoordinates = poiData.xyCoordinates;
//        poiName = poiData.Name;
//        
//        g->set_color (ezgl::BLACK);
//        if (enable_poi)
//            g->draw_text({xyCoordinates.first, xyCoordinates.second}, poiName);
//        
//        it++;
//    }
//    
//    //loop through hospital vector, extract data and draw names of police stations on map
//    it = hospitals.begin();
//    while(it != hospitals.end()){
//        poiData = *it;
//        xyCoordinates = poiData.xyCoordinates;
//        poiName = poiData.Name;
//        
//        g->set_color (ezgl::BLACK);
//        if (enable_poi)
//            g->draw_text({xyCoordinates.first, xyCoordinates.second}, poiName, 10, 10);
//        
//        it++;
//    }
//    
//    //loop through fire_station vector, extract data and draw names of police stations on map
//    it = fire_station.begin();
//    while(it != fire_station.end()){
//        poiData = *it;
//        xyCoordinates = poiData.xyCoordinates;
//        poiName = poiData.Name;
//        
//        g->set_color (ezgl::BLACK);
//        if (enable_poi)
//            g->draw_text({xyCoordinates.first, xyCoordinates.second}, poiName, 10, 10);
//        
//        it++;
//    }
    
    //Drawing Intersections
    draw_intersections(g);

}

std::pair < double, double > latLonToCartesian (LatLon latLonPoint){
    //convert LatLon points into x y coordinates
    double y = latLonPoint.lat()*DEGREE_TO_RADIAN *EARTH_RADIUS_METERS;
    double x = latLonPoint.lon()*DEGREE_TO_RADIAN *EARTH_RADIUS_METERS*cos(latAvg);
    std::pair < double, double> cartesian (x, y);
    return cartesian;
}

double x_from_lon (double lon){
    //convert Lon into x coordinate, return x 
    return lon*DEGREE_TO_RADIAN *EARTH_RADIUS_METERS*cos(latAvg);
}

double y_from_lat (double lat){
    //convert LatLon points into x y coordinates
    return lat*DEGREE_TO_RADIAN *EARTH_RADIUS_METERS;
}

double lon_from_x (double x){
    //convert Lon into x coordinate, return x 
    return x / (DEGREE_TO_RADIAN * EARTH_RADIUS_METERS * cos(latAvg));
}

double lat_from_y (double y){
    //convert LatLon points into x y coordinates
    return y / (DEGREE_TO_RADIAN * EARTH_RADIUS_METERS);
}

double getRotationAngle(std::pair <double, double> xyFrom, std::pair <double, double> xyTo){
    
    double rotationAngle = atan2(xyFrom.second - xyTo.second, xyFrom.first - xyTo.first) / DEGREE_TO_RADIAN;

    if (rotationAngle > 90) {
        rotationAngle = rotationAngle - 180;
    }
    if (rotationAngle < -90) {
        rotationAngle = rotationAngle + 180;
    }
    
    return rotationAngle;
}

void populatePointsOfInterest(){
    
    //store value and name of POI
    std::string value, name;
    poiStruct poiData;
    LatLon latlon;
    std::pair<double, double> xy;
    double x, y;
    std::vector<poiStruct> police, hospital, fire_station;
    //iterate through points of interest using layer 1
    for (unsigned poiIterator = 0; poiIterator < getNumPointsOfInterest(); poiIterator++){
        value = getPointOfInterestType(poiIterator);
        
        if (value == "police"){
            name = getPointOfInterestName(poiIterator);
            latlon = getPointOfInterestPosition(poiIterator);
            
            //conversion to cartesian
            x = x_from_lon (latlon.lon());
            y = y_from_lat (latlon.lat());
            xy = std::make_pair(x,y);
            poiData.addName(name);
            poiData.addXYCoordinates(xy);
            
            police.push_back(poiData);
        }
        else if (value == "hospital"){
            name = getPointOfInterestName(poiIterator);
            latlon = getPointOfInterestPosition(poiIterator);
            
            x = x_from_lon (latlon.lon());
            y = y_from_lat (latlon.lat());
            xy = std::make_pair(x,y);
            
            poiData.addName(name);
            poiData.addXYCoordinates(xy);
            
            hospital.push_back(poiData);
        }
        else if (value == "fire_station"){
            name = getPointOfInterestName(poiIterator);
            latlon = getPointOfInterestPosition(poiIterator);
            
            x = x_from_lon (latlon.lon());
            y = y_from_lat (latlon.lat());
            xy = std::make_pair(x,y);
            
            poiData.addName(name);
            poiData.addXYCoordinates(xy);
            
            fire_station.push_back(poiData);
        }
    }
    
    PointsOfInterest[0] = police;
    PointsOfInterest[1] = hospital;
    PointsOfInterest[2] = fire_station;
}

/*
 * Searches through all OSM Entities and their tags to match OSMIDs with RoadType (e.g. motorway, primary roads, residential, etc.)
 * Populates global variable WaybyRoadType
 * The "highway" OSM key is used to access the road type
 */
void populateWaybyRoadType(){
    std::string key,value;
    RoadType road_type;
    
    //Searches through all OSM Ways and their keys to determine road types
    for (unsigned i = 0; i < getNumberOfWays(); i++){
        //creates a pointer that enables accessing the way's OSMID
        const OSMWay* wayPtr = getWayByIndex(i);
        key="N/A"; //in case the way has no keys
        for(int j = 0; j<getTagCount(wayPtr); ++j){
            std::tie(key,value) = getTagPair(wayPtr,j);
            if (key == "highway")
                break;
        }
        if (key == "highway"){ //if highway key exists
            if(value == "motorway")
                road_type = motorway;
            else if (value == "trunk")
                road_type = trunk;
            else if (value == "primary")
                road_type = primary;
            else if (value == "secondary")
                road_type = secondary;
            else if (value == "tertiary")
                road_type = tertiary;
            else if (value == "residential")
                road_type = residential;
            else road_type = unclassified;
            
            WaybyRoadType.insert({wayPtr->id(),road_type}); //take the value of key and store it in global variable with OSMID
        }
        
    }
    
    //Searches through all OSM Relations and their keys to determine road types
    //Repeats same process as above with Ways, except searches through relations
    for (unsigned i = 0; i < getNumberOfRelations(); i++){
        //creates a pointer that enables accessing the relation's OSMID
        const OSMRelation* relationPtr = getRelationByIndex(i);
        key="N/A"; //in case the way has no keys
        for(int j=0;j<getTagCount(relationPtr); ++j){
            std::tie(key,value) = getTagPair(relationPtr,j);
            if (key == "highway")
                break;
        }
        if (key == "highway"){ //if highway key exists
           if(value == "motorway")
                road_type = motorway;
            else if (value == "trunk")
                road_type = trunk;
            else if (value == "primary")
                road_type = primary;
            else if (value == "secondary")
                road_type = secondary;
            else if (value == "tertiary")
                road_type = tertiary;
            else if (value == "residential")
                road_type = residential;
            else road_type = unclassified;
            
            WaybyRoadType.insert({relationPtr->id(),road_type}); //take the value of key and store it in global variable with OSMID
        }
        
    }
    
    //Searches through all OSM Nodes and their keys to determine road types
    //Repeats same process for node 
    for (unsigned i = 0; i < getNumberOfNodes(); i++){
        //creates a pointer that enables accessing the node's OSMID
        const OSMNode* nodePtr = getNodeByIndex(i);
        key="N/A"; //in case the way has no keys
        for(int j=0;j<getTagCount(nodePtr); ++j){
            std::tie(key,value) = getTagPair(nodePtr,j);
            if (key == "highway")
                break;
        }
        if (key == "highway"){ //if highway key exists
            if(value == "motorway")
                road_type = motorway;
            else if (value == "trunk")
                road_type = trunk;
            else if (value == "primary")
                road_type = primary;
            else if (value == "secondary")
                road_type = secondary;
            else if (value == "tertiary")
                road_type = tertiary;
            else if (value == "residential")
                road_type = residential;
            else road_type = unclassified;
            
            WaybyRoadType.insert({nodePtr->id(),road_type}); //take the value of key and store it in global variable with OSMID
        }
        
    }  
}

void populateFeatureIds_byType(){
    FeatureIds_byType.resize(10);
        for(size_t featureId = 0; featureId < getNumFeatures(); ++featureId){

            FeatureType feature_type = getFeatureType(featureId);
            if (feature_type>9||feature_type<0)
                std::cout<<"Error: Invalid Feature Type detected\n";
            else{
                FeatureIds_byType[feature_type].push_back(featureId);
            }
        }  
}

//Helper function called by DrawFeatures()
//Draws all features of a specifc type (e.g Park, Beach, Lake, etc.)
void drawFeature_byType(int feature_type, ezgl::renderer *g){  
    
    g->set_line_width(3);
    //before drawing, sets colour appropriate to the feature
    switch(feature_type){
            
            case     Unknown   : g->set_color (64,64,64, 255);
                                 break;
            case     Park      : g->set_color (0,51,25, 255);
                                 break;
            case     Beach     : g->set_color (102,51,0, 255);
                                 break;       
            case     Lake      : g->set_color (0,0,51, 255);
                                 break;
            case     River     : g->set_color (0,0,102, 255);
                                 break;
            case     Island    : g->set_color (0,51,25, 255);
                                 break;
            case     Building  : g->set_color (64,64,64, 255);
                                 break;
            case     Greenspace: g->set_color (0,51,25, 255);
                                 break;
            case     Golfcourse: g->set_color (0,51,25, 255);
                                 break;
            case     Stream    : g->set_color (0,0,102, 255);
                                 break;
            
            default: g->set_color (64,64,64, 255);
        }
    int featureId, numOfFeaturePoints;
    //goes through FeatureIds_byType vector, retrieves all featureIds of a certain type
    //draws features as either polylines or polygons
    //uses Featurepoints_xy vector
    for(size_t idx = 0; idx < FeatureIds_byType[feature_type].size(); ++idx){

        featureId = FeatureIds_byType[feature_type][idx];
        numOfFeaturePoints = getFeaturePointCount(featureId);
        //point2d has no default constructor
        ezgl::point2d xyPrevious(0,0);
        ezgl::point2d xyNext(0,0);

        // If the feature area is 0, feature is a polyline (uses FeatureAReaVector from m1)
        if (FeatureAreaVector[featureId] == 0) {
            
            //iterate through feature points to get (x,y) coordinates  in order to draw the polyline)
            for (unsigned featurePointId = 0; featurePointId < (numOfFeaturePoints - 1); featurePointId++){

                xyPrevious = Featurepoints_xy[featureId][featurePointId];
                xyNext = Featurepoints_xy[featureId][featurePointId + 1];

                //draw line between feature points
                g->draw_line({xyPrevious.x , xyPrevious.y}, {xyNext.x, xyNext.y});
            }   
        }
        //otherwise, it is a polygon area
        else{
            //ezgl function which takes a vector<point2d> (i.e (x,y) coordinates for each point that defines the feature outline)
            g->fill_poly(Featurepoints_xy[featureId]);
        }
            
  
//            double max_width = feature_max_width (numOfFeaturePoints, featureId);
//            
//            //std::cout << "max_width " << max_width << "\n";
//            
//            //std::cout << feature_centroid.x << feature_centroid.y << "\n";
//            
//            if( getFeatureName(featureId) != "<noname>")
//                g->draw_text(feature_centroid, getFeatureName(featureId), max_width, max_width);
//            
//  
//            g->fill_rectangle({feature_centroid.x,feature_centroid.y}, {feature_centroid.x + 10, feature_centroid.y + 10});
//  
            //float width = max_width;

            //g->fill_rectangle({feature_centroid.x-(width/2),feature_centroid.y-(width/2)}, {feature_centroid.x + (width/2), feature_centroid.y + (width/2)});
    }
}

void act_on_mouse_click( ezgl:: application* app, GdkEventButton* event, double x_click, double y_click){
    //x_click and y_click are the world coordinates where the mouse was clicked
    //will convert to latlon then use find_closest_intersection
    //std::cout << "x: " << x_click << "y: " << y_click << std::endl;
    LatLon lat_lon_click = LatLon(lat_from_y (y_click), lon_from_x (x_click));
    
    int closestInt_id = find_closest_intersection(lat_lon_click);
    
    clearIntersection_highlights();
    
    Highlighted_intersections.push_back(closestInt_id);
    intersections[closestInt_id].highlight = true;
    
    app->update_message (getIntersectionName(closestInt_id));
    
    app->refresh_drawing();
            
}

void initial_setup(ezgl::application *application, bool new_window)
{

  //Create a Find button and link it with find_button callback function.
    //number indicates the order of the button (0->top)
  application->create_button("Find", 0, find_button);


}

void find_button(GtkWidget* widget, ezgl::application *application){
    //two string variables needed to interpret input
    std::string street1, street2;
    std::pair<int, int> twoStreets;
    std::vector<int> streetIntersections;
    
    // Get the GtkEntry object
    GtkEntry* text_entry = (GtkEntry *) application->get_object("TextInput");
    
    // Get the text written in the widget
    const char* text = gtk_entry_get_text(text_entry);

    //convert string into a stream
    std::istringstream iss(text);
    if (text==NULL){
        std::cout<<"Empty Find parameters\n";
        return;
    }
       
    //get street names to be used find function
    iss >> street1; 
    if (street1.find_first_not_of(' ') == std::string::npos) 
        return;
    std::getline(iss, street2);
    if (street2.find_first_not_of(' ') == std::string::npos)
        return;
    street2 = street2.substr(street2.find("and") + 4); 
//    if (street2.find(" & ")< street2.length()){ //found an & in the string
//        iss >> street2;  
//        std::getline(iss, street2);
//    }
//    else{
//         street2 = street2.substr(street2.find("and") + 4); //
//    }
    
    //if one of the street names is invalid (whitespace only)
    if (street2.find_first_not_of(' ') == std::string::npos)
        return;
    
    //obtains all of the possible streetIds that match the entered street names
    std::vector<int> street_ids_1 = find_street_ids_from_partial_street_name(street1);
    std::vector<int> street_ids_2 = find_street_ids_from_partial_street_name(street2);
    
    int street1_id = 0;
    int street2_id = 0;
    //nested for-loop which finds intersections between all streetId's returned by partial_street_name function
    //outer for-loop looks through all matches in 

    for(int i = 0; i < street_ids_1.size() && (streetIntersections.empty() == true); i++){
        for(street2_id = 0; street2_id < street_ids_2.size() && (streetIntersections.empty() == true) ; street2_id++){
            street1_id = i; //store old value of i before incrementation
            
            //load twoStreets pair
            twoStreets.first = street_ids_1[street1_id];
            twoStreets.second = street_ids_2[street2_id];

            streetIntersections = find_intersections_of_two_streets(twoStreets);
        }
    }
    
    //an un empty streetIntersections vector indicates that common intersections were found for the predicted streets
    
    std::string intersectionNames = "";
        
    for(int i = 0; i < streetIntersections.size(); i++){
        intersections[streetIntersections[i]].highlight = true;
        intersectionNames+=getIntersectionName(streetIntersections[i]);
        if (i+1!=streetIntersections.size())
            intersectionNames+=", ";
    }
    
    //Suggested Street names  
    std::string suggested_streets = "";
    
//    street2_id++; //advance to next possible street for street 2
    //finish iteration on street1 (i.e. for that same street1 value, check all other possibilities on street 2)
    for(int j = street2_id; j < street_ids_2.size(); j++){

        //load twoStreets pair
        twoStreets.first = street_ids_1[street1_id];

        twoStreets.second = street_ids_2[j];
        streetIntersections = find_intersections_of_two_streets(twoStreets);
        if (streetIntersections.empty()==false){

            suggested_streets+=getStreetName(twoStreets.first)+" & "+getStreetName(twoStreets.second)+"\n";
            streetIntersections.clear();    
        }
    }
    street1_id++;
    
    //check all possibilities for other values of street 1, and for each street1, check with all combinations of street 2
    for(int i = street1_id; i < street_ids_1.size(); i++){
        for(int j = 0; j < street_ids_2.size(); j++){
            
            //load twoStreets pair
            twoStreets.first = street_ids_1[i];

            twoStreets.second = street_ids_2[j];
            streetIntersections = find_intersections_of_two_streets(twoStreets);
            if (streetIntersections.empty()==false){
                
                suggested_streets+=getStreetName(twoStreets.first)+" & "+getStreetName(twoStreets.second)+"\n";
                streetIntersections.clear();    
            }
        }
    }
    
    if (suggested_streets!=""){
        std::cout<<"\nDid you mean?\n\n";
        std::cout<<suggested_streets;
    }
//    //for safety
//    streetIntersections.clear(); 
    application->update_message (intersectionNames); 
    // Redraw the graphics
    application->refresh_drawing();
}

//vertices are the feature points in xy coordinates, vertexCount = number of feature points
ezgl::point2d compute2DPolygonCentroid(std::vector< ezgl::point2d > & vertices, double& area)
{
    ezgl::point2d centroid(0, 0);
    double x0 = 0.0; // Current vertex X
    double y0 = 0.0; // Current vertex Y
    double x1 = 0.0; // Next vertex X
    double y1 = 0.0; // Next vertex Y
    double a = 0.0;  //multiplication factor (xi*yi+1 - xi+1yi))

    // For all vertices
    for (int i = 0; (i+1) < vertices.size(); ++i){
        x0 = vertices[i].x;
        x1 = vertices[i+1].x;
        
        y0 = vertices[i].y;
        y1 = vertices[i+1].y;
        
        a = x0*y1 - x1*y0;
        centroid.x += (x0 + x1)*a;
        centroid.y += (y0 + y1)*a;
    }

    centroid.x /= (6.0 * area);
    centroid.y /= (6.0 * area);  
    
    return centroid;
}
//Finds greatest length along the centre of the polygon in which text can fit
double feature_max_width (int numPoints, int featureId){
    
    //find the maximum and minimum intersections of the map
    double feat_max_lon = getFeaturePoint(0, featureId).lon();
    double feat_min_lon = feat_max_lon;
    
    for(int i = 0; i < numPoints; i++){

        //get position from streetsdatabaseAPI function
        LatLon featurePointLatLon = getFeaturePoint(i, featureId);
        
        //compare the lon position to update max/min
        feat_max_lon = std::max(feat_max_lon, featurePointLatLon.lon());
        feat_min_lon = std::min(feat_min_lon, featurePointLatLon.lon());
    }
    
    LatLon firstPoint (0, feat_min_lon);
    
    LatLon secondPoint (0, feat_max_lon);
    
    std::pair<LatLon, LatLon> length(firstPoint, secondPoint);
    
    return find_distance_between_two_points(length);
    
}

void draw_feature_names(ezgl::renderer *g){
    int feature_type;

    g->set_text_rotation(0);

    //helpful for debugging
    //std::cout << "max_width " << max_width << "\n";
    //std::cout << feature_centroid.x << feature_centroid.y << "\n";
    //float width = max_width;
    //g->fill_rectangle({feature_centroid.x-(width/2),feature_centroid.y-(width/2)}, {feature_centroid.x + (width/2), feature_centroid.y + (width/2)});

    //loops through featureIds to retrieve name and correlates it with FeatureCentroids hashtable to get position
//    for( std::unordered_map< int, ezgl::point2d >::iterator it = FeatureCentroids.begin();  it != FeatureCentroids.begin(); it++){
      for( int featureidx = 0;  featureidx < getNumFeatures(); featureidx++){      
        feature_type = getFeatureType(featureidx);
                
        
        if( (feature_type != Unknown) && (getFeatureName(featureidx) != "<noname>") && (scale_factor < 0.10) ){
                
            switch(feature_type){

                case     Park      : g->set_color (76,153,0, 255);
                                     break;
                case     Beach     : g->set_color (160,160,160, 255);
                                     break;       
                case     Lake      : g->set_color (0,128,255, 255);
                                     break;
                case     River     : g->set_color (0,128,255, 255);
                                     break;
                case     Island    : g->set_color (255,255,255, 255);
                                     break;
                case     Building  : g->set_color (255,255,255, 255);
                                     break;
                case     Greenspace: g->set_color (76,153,0, 255);
                                     break;
                case     Golfcourse: g->set_color (76,153,0, 255);
                                     break;
                case     Stream    : g->set_color (0,128,255, 255);
                                     break;

                default: g->set_color (224,224,224, 255);
            } 
            //draw feature name in centre of polygon
            g->draw_text(FeatureCentroids.at(featureidx), getFeatureName(featureidx));
        }
    }
}

//returns approximate middle feature point of a given polyline feature in (x,y) coordinates
ezgl::point2d find_PolyLine_Middle(int featureId){
    
    int numFeaturePoints = getFeaturePointCount(featureId);
    
    //if even, will not return meddle exactly, but close enough
    int median =  numFeaturePoints / 2;
    
    //retrieve feature point using StreetsDatabase function
    LatLon middlePoint_latlon = getFeaturePoint(median, featureId);
    
    //convert into x,y coordinates
    std::pair<double, double> middlePoint_xy = latLonToCartesian(middlePoint_latlon);
    
    //return as ezgl::point2d
    return ezgl::point2d(middlePoint_xy.first, middlePoint_xy.second);
    
}

//function called in darw_main_canvas(), which draws all features in pre-determined order using helper function: drawFeatrues_byType)
void drawFeatures(ezgl::renderer *g){
    //Draws in the following order:
        //  Lake (Least important, drawn first)
        //  Park
        //  Island,
        //  Park,
        //  Greenspace
        //  Beach
        //  Golfcourse,
        //  Building
        //  River
        //  Stream
        //  Unknown = 0,(Most important, drawn last)

    g->set_line_dash(ezgl::line_dash::none);
    
    //color set in sub_function
    drawFeature_byType(Park, g);
    drawFeature_byType(Greenspace, g);
    drawFeature_byType(Lake, g);
    drawFeature_byType(Island, g);
    drawFeature_byType(Beach, g);
    drawFeature_byType(Golfcourse, g);
    drawFeature_byType(River, g);
    drawFeature_byType(Stream, g);
    
    if(scale_factor <  0.1){    
        drawFeature_byType(Building, g);
        drawFeature_byType(Unknown, g);
    }
}

//function draws all streets on map
//uses StreetVector from m1.cpp

void draw_streets(ezgl::renderer *g){
    
    //variables needed to draw streets
    int segmentID;
    RoadType roadType;
    //enabler based on zoom
    bool enableDraw = true;
    std::vector<int> segments;
    int numCurvePoints;
    struct InfoStreetSegment segmentInfo;
    
    //variables needed to draw street names
    std::string streetName;
    double segmentLength;
    std::pair <double, double> xyFrom;
    std::pair <double, double> xyTo;
    
    
    g->set_line_cap(ezgl::line_cap::round);
    //loops through StreetVector to retrieve street segments of each street
    for (int streetIdx = 0; streetIdx < StreetVector.size(); streetIdx++ ){ 
        //retrieve streetSegements vector from StreetVector struct (defined in m1)
        segments = StreetVector[streetIdx].streetSegments;

        streetName = getStreetName(streetIdx);
        
        //Draw each segment of each street
        for (int i = 0; i < segments.size(); i++ ){
            
            enableDraw = true;
            
            //retrieve segmentId from segments vector
            segmentID = segments[i];
   
            segmentLength = SegmentLengths[segmentID]; 
            
            segmentInfo = getInfoStreetSegment(segmentID); //retrieve all info of segment
            
            numCurvePoints = segmentInfo.curvePointCount;
            
            //retrieve segment road type from WaybyRoadType hashtable
            roadType = WaybyRoadType.at(segmentInfo.wayOSMID); 
            
            //scale_factor used to set a variety of line widths and displays of roads    
            switch(roadType){

              //Motorways are always drawn, regardless of zoom level
                case motorway     : g->set_line_width (2.9);
                                    if (scale_factor <  0.2)
                                        g->set_line_width (3.5);//change thickness of road drawn depending on zoom level
                                    g->set_color(Colour_motorway);
                                    break;
            
                case trunk        : g->set_color (Colour_trunk);
                                    g->set_line_width (2.8);
                                    if (scale_factor <  0.2)
                                        g->set_line_width (3.5);//change thickness of road drawn depending on zoom level
                                    break;
     
                case primary      : g->set_color (Colour_primary);
                                    if (scale_factor > 0.60) //only enable drawing these streets if zoomed in enough
                                        enableDraw = false;
                                    g->set_line_width (2.75);
                                    if (scale_factor <  0.2)
                                        g->set_line_width (3.5);//change thickness of road drawn depending on zoom level
                                    break;
                                    
                case secondary    : g->set_color (Colour_secondary);
                                    g->set_line_width (2.6);
                                    if (scale_factor > 0.40) //only enable drawing these streets if zoomed in enough
                                        enableDraw = false;
                                    if (scale_factor <  0.02)
                                        g->set_line_width (3);//change thickness of road drawn depending on zoom level
                                    break;
                                    
                case tertiary     : if (scale_factor > 0.30) //only enable drawing these streets if zoomed in enough
                                        enableDraw = false;
                                    g->set_color (Colour_tertiary);
                                    g->set_line_width (2.25);
                                    if (scale_factor <  0.02)
                                      g->set_line_width (3);//change thickness of road drawn depending on zoom level
                                    break;
            
                case residential  : if (scale_factor > 0.05)//only enable drawing these streets if zoomed in enough
                                        enableDraw = false;
                                    g->set_color (Colour_residential);
                                    g->set_line_width (2);
                                    if (scale_factor <  0.02)
                                        g->set_line_width (3);//change thickness of road drawn depending on zoom level
                                    break;
                                    
                case unclassified : if (scale_factor > 0.05)//only enable drawing these streets if zoomed in enough
                                        enableDraw = false;
                                    g->set_color (Colour_unclassified);
                                    g->set_line_width (2);
                                    if (scale_factor <  0.02)
                                        g->set_line_width (3);//change thickness of road drawn depending on zoom level
                                    break;
                                    
                default           : if (scale_factor > 0.30)
                                        enableDraw = false;//only enable drawing these streets if zoomed in enough
                                    g->set_line_width (2);
                                    g->set_color (Colour_unclassified);
            }     
            
            if (enableDraw){
                
                xyFrom = latLonToCartesian(intersections[segmentInfo.from].position);
                xyTo = latLonToCartesian(intersections[segmentInfo.to].position);

                 //if segment is a straight line
                if (numCurvePoints == 0){

                        g->set_line_dash(ezgl::line_dash::none);
                        g->draw_line({xyFrom.first, xyFrom.second}, {xyTo.first, xyTo.second});

                        if(streetName != "<unknown>"){// "<unknown>" street name not drawn
                                if (!(roadType ==motorway&& scale_factor > 0.6)){ //motorway names will not show unless zoomed in a little (makes the default display look cleaner)
                                draw_street_name(g, xyFrom, xyTo, segmentLength, streetName, segmentInfo.oneWay);
                            }
                        }
                }
                        
                else{ //segment is curved

                    //First draw street's line, then the text
                    //first deal with all curves from segment's "from" intersection to the last curve point

                    //first curve of the segment
                    LatLon pointsLeft  = getIntersectionPosition(segmentInfo.from);
                    LatLon pointsRight = getStreetSegmentCurvePoint(0, segmentID);

                    //also determine curve segment that is the longest
                    double maxCurveLength = 0;
                    //track the place where the max curve length exists
                    int maxCurvePosition = 0; // 0 means the max is between segment's from and the first curve point. 1 means the max is between the 1st and 2nd curve points
                    maxCurveLength = find_distance_between_two_points(std::pair <LatLon, LatLon>(pointsLeft, pointsRight));
                    
                    std::pair <double, double> xyLeft = latLonToCartesian(pointsLeft);
                    std::pair <double, double> xyRight = latLonToCartesian(pointsRight);

                    g->draw_line({xyLeft.first, xyLeft.second}, {xyRight.first, xyRight.second});

                    for (int curvePointIndex = 0; curvePointIndex < numCurvePoints - 1; curvePointIndex++){
                        pointsLeft = pointsRight;
                        pointsRight = getStreetSegmentCurvePoint(curvePointIndex + 1, segmentID);
                        
                        double curveLength = find_distance_between_two_points(std::pair <LatLon, LatLon>(pointsLeft, pointsRight));
                        if (curveLength > maxCurveLength){
                            maxCurveLength = curveLength;
                            maxCurvePosition = curvePointIndex + 1;
                        }
                        
                        xyLeft = latLonToCartesian(pointsLeft);
                        xyRight = latLonToCartesian(pointsRight);

                        g->draw_line({xyLeft.first, xyLeft.second}, {xyRight.first, xyRight.second});
                    }

                    //then, deal with the last curve point to the segment's "to" intersection
                    pointsLeft = pointsRight;
                    pointsRight = getIntersectionPosition(segmentInfo.to);

                    double curveLength = find_distance_between_two_points(std::pair <LatLon, LatLon>(pointsLeft, pointsRight));
                        if (curveLength > maxCurveLength){
                            maxCurveLength = curveLength;
                            maxCurvePosition = numCurvePoints;
                        }

                    xyLeft = latLonToCartesian(pointsLeft);
                    xyRight = latLonToCartesian(pointsRight); 

                    g->draw_line({xyLeft.first, xyLeft.second}, {xyRight.first, xyRight.second});
                    
                    //Draw street name
                    
                    //if zoomed out too far, do not draw street name of major roads (many cities have several motorways adjacent, which causes street names to look squished)
                    if (!((roadType ==motorway ||roadType ==trunk)&& scale_factor > 0.075)){

                        if(streetName != "<unknown>"){// "<unknown>" street name not drawn
                            std::pair <double, double> xyLeftMax;// =  latLonToCartesian(getStreetSegmentCurvePoint(maxCurvePosition, segmentID));
                            std::pair <double, double> xyRightMax;// =  latLonToCartesian(getStreetSegmentCurvePoint(maxCurvePosition + 1, segmentID));
    //                        
                            if (maxCurvePosition==0){
                                xyLeftMax = latLonToCartesian(getIntersectionPosition(segmentInfo.from));
                            }
                            else
                                xyLeftMax = latLonToCartesian(getStreetSegmentCurvePoint(maxCurvePosition - 1, segmentID));

                            if (maxCurvePosition==numCurvePoints){
                                xyRightMax = latLonToCartesian(getIntersectionPosition(segmentInfo.to));
                            }
                            else
                                 xyRightMax =  latLonToCartesian(getStreetSegmentCurvePoint(maxCurvePosition, segmentID));
    //                        
                            double xMiddleOfSegment = (xyLeftMax.first + xyRightMax.first)/2;
                            double yMiddleOfSegment = (xyLeftMax.second + xyRightMax.second)/2;
                            std::string streetSegName = streetName; //saves copy of street name
                            if (segmentInfo.oneWay){
                                std::string direction_symbol = ">"; //symbol for one way street
                                if (xyFrom.first > xyTo.first) {
                                         direction_symbol = "<"; //reverse direction
                                    }
                                streetSegName = direction_symbol + "    " + streetSegName + "    " + direction_symbol;
                            }

                            g->set_text_rotation(getRotationAngle(xyLeftMax, xyRightMax));
                            g->set_color(ezgl::WHITE);
                            g->draw_text({xMiddleOfSegment, yMiddleOfSegment}, streetSegName, segmentLength, segmentLength);
                        }
                    }
                }
            }
        }
    }
}

/**
 * Draws street name for only a STRAIGHT street segment. If segment is curved, drawing of street name happens in draw_streets
 * @param g
 * @param xyFrom
 * @param xyTo
 * @param numCurvePoints
 * @param segmentLength
 * @param streetName
 * @param oneWay
 */
void draw_street_name(ezgl::renderer* g, std::pair<double, double>& xyFrom, std::pair<double, double>& xyTo, double& segmentLength, std::string& streetName, bool oneWay)
{
    //Variables for drawing text of street segments
    //uses find_street_segment_length from m1
    double xMiddleOfSegment, yMiddleOfSegment;

    //Greater screen_ratio = more reprinting of street name and arrows: < (for oneway streets)
    double screen_ratio = segmentLength / scale_factor; //screen_ratio is the available length of the street segment on screen.
    g ->set_color(224, 224, 224, 255);

        xMiddleOfSegment = 0.5 * (xyFrom.first + xyTo.first);
        yMiddleOfSegment = 0.5 * (xyFrom.second + xyTo.second);
        
        //set rotation for text to draw
        g->set_text_rotation(getRotationAngle(xyFrom, xyTo));

        //set default direction for one way street
        std::string direction_symbol = ">"; //symbol for one way street
        if (oneWay) {
            if (xyFrom.first > xyTo.first) {
                direction_symbol = "<"; //reverse direction
            }
        }

        //if there is only enough space to draw direction
        if (screen_ratio < 5000) {
            if (oneWay) {
                g->draw_text({ xMiddleOfSegment,
                                    yMiddleOfSegment },
                          direction_symbol, segmentLength, segmentLength);
            }
        }

        //if only enough space to draw name
        else if (screen_ratio < 20000) {
            g->draw_text({ xMiddleOfSegment,
                                yMiddleOfSegment },
                      streetName, segmentLength, segmentLength);
        }

        //draw:  [direction] [name] [direction]
        else if (screen_ratio < 30000) {
            g->draw_text({ xMiddleOfSegment,
                                yMiddleOfSegment },
                      streetName, segmentLength, segmentLength);
            if (oneWay) {
                g->draw_text({ (xMiddleOfSegment + xyFrom.first) / 2,
                                    (yMiddleOfSegment + xyFrom.second) / 2 },
                          direction_symbol, segmentLength, segmentLength);
                g->draw_text({ (xMiddleOfSegment + xyTo.first) / 2,
                                    (yMiddleOfSegment + xyTo.second) / 2 },
                          direction_symbol, segmentLength, segmentLength);
            }
        }

        //draw:  [direction] [name] [direction] [name] [direction]
        else if (screen_ratio < 50000) {
            g->draw_text({ (2 * xMiddleOfSegment + xyFrom.first) / 3,
                                (2 * yMiddleOfSegment + xyFrom.second) / 3 },
                      streetName, segmentLength, segmentLength);
            g->draw_text({ (2 * xMiddleOfSegment + xyTo.first) / 3,
                                (2 * yMiddleOfSegment + xyTo.second) / 3 },
                      streetName, segmentLength, segmentLength);

            if (oneWay) {
                g->draw_text({ xMiddleOfSegment,
                                    yMiddleOfSegment },
                          direction_symbol, segmentLength, segmentLength);
                g->draw_text({ (((2 * xMiddleOfSegment + xyFrom.first) / 3) + xyFrom.first) / 2,
                                    (((2 * yMiddleOfSegment + xyFrom.second) / 3) + xyFrom.second) / 2 },
                          direction_symbol, segmentLength, segmentLength);
                g->draw_text({ (((2 * xMiddleOfSegment + xyTo.first) / 3) + xyTo.first) / 2,
                                    (((2 * yMiddleOfSegment + xyTo.second) / 3) + xyTo.second) / 2 },
                          direction_symbol, segmentLength, segmentLength);
            }
        }

        //draw:  [direction] [name] [direction] [name] [direction] [name] [direction]
        else {
            g->draw_text({ xMiddleOfSegment,
                                yMiddleOfSegment },
                      streetName, segmentLength, segmentLength);
            g->draw_text({ (xMiddleOfSegment + xyFrom.first) / 2,
                                (yMiddleOfSegment + xyFrom.second) / 2 },
                      streetName, segmentLength, segmentLength);
            g->draw_text({ (xMiddleOfSegment + xyTo.first) / 2,
                                (yMiddleOfSegment + xyTo.second) / 2 },
                      streetName, segmentLength, segmentLength);

            if (oneWay) {
                g->draw_text({ (((xMiddleOfSegment + xyFrom.first) / 2) + xyFrom.first) / 2,
                                    (((yMiddleOfSegment + xyFrom.second) / 2) + xyFrom.second) / 2 },
                          direction_symbol, segmentLength, segmentLength);
                g->draw_text({ (((xMiddleOfSegment + xyFrom.first) / 2) + xMiddleOfSegment) / 2,
                                    (((yMiddleOfSegment + xyFrom.second) / 2) + yMiddleOfSegment) / 2 },
                          direction_symbol, segmentLength, segmentLength);
                g->draw_text({ (((xMiddleOfSegment + xyTo.first) / 2) + xMiddleOfSegment) / 2,
                                    (((yMiddleOfSegment + xyTo.second) / 2) + yMiddleOfSegment) / 2 },
                          direction_symbol, segmentLength, segmentLength);
                g->draw_text({ (((xMiddleOfSegment + xyTo.first) / 2) + xyTo.first) / 2,
                                    (((yMiddleOfSegment + xyTo.second) / 2) + xyTo.second) / 2 },
                          direction_symbol, segmentLength, segmentLength);
            }
        }
        //for safety, set rotation back to zero
        g->set_text_rotation(0);
    
}

void draw_intersections(ezgl::renderer *g){
    
     if (scale_factor <=0.01) {
         
        for(size_t i = 0; i < intersections.size(); ++i){

          double x = intersections[i].position.lon();
          double y = intersections[i].position.lat();

          //must convert lat lon values to cartesian (refer to tutorial slides)
          x = x_from_lon(x);
          y = y_from_lat(y);

    //      int threshold = intersectionThresrhold(i);
    //      if (threshold==0||(threshold==1&&scale_factor <= 0.30)||(scale_factor <=0.01) )
    //          enableDraw = true;
    //      else 
    //          enableDraw = false;

          float width;
          if (scale_factor > 0.005)
              width =  2;
          else if (scale_factor > 0.001)
              width =  1;
    //      else if (scale_factor > 0.0001)
    //          width =  0.;
          else //if (scale_factor < 0.10)
              width =  0.5;

          float height = width;

          if (intersections[i].highlight)
              g->set_color(ezgl::RED);
          else
              g->set_color(ezgl::BLUE);

          //for intersection id, get segs. Check seg's lowest threshold value
          //for that threshold value, enable or disable drawing
            g->fill_rectangle({x-(width/2),y-(height/2)}, {x + (width/2), y + (height/2)});
    
        }
    }
    
}


void clearIntersection_highlights(){
    for (std::vector<int>::iterator it = Highlighted_intersections.begin(); it != Highlighted_intersections.end(); ++it){
        std::cout << *it;
        intersections[*it].highlight = false;
    }
}

void populateFeatureCentroids(){
    
}
//populates Featurepoints_xy vector needed to draw features and feature names
void populateFeaturepoints_xy(){
    //uses StreetsDatabase function
    Featurepoints_xy.resize(getNumFeatures());
    
    int numOfFeaturePoints;
    
    for(int featureId = 0; featureId < getNumFeatures(); ++featureId){

        numOfFeaturePoints = getFeaturePointCount(featureId);

        //iterate through feature points then convert to (x,y) coordinates
        for (unsigned featurePointId = 0; featurePointId < numOfFeaturePoints; featurePointId++){
            //1) retrieves the featurepoint in lat lon
            //2) LatLonToCartesian is called to return an (x,y) pair
            //3)the pair is converted to point2d (needed by ezgl to draw))
            //4) the point2d is inserted into the featurepioints vector of the appropriate Featurepoints_xy element
            //does not matter that feature points will be in reverse order, as long as they are consecutive

            std::pair<double, double> featurePoint_xy = latLonToCartesian(getFeaturePoint(featurePointId, featureId));
            
            ezgl::point2d featurePoint_point2d(featurePoint_xy.first, featurePoint_xy.second);
            
            Featurepoints_xy[featureId].push_back(featurePoint_point2d);
        }
        
        if(FeatureAreaVector[featureId] == 0){
            //insert feature middle point into FeatureCentroids hashtable
            FeatureCentroids.insert({featureId , find_PolyLine_Middle(featureId)});
        }
        else{
            //insertfeature area centre point into FeatureCentroids hashtable       
            FeatureCentroids.insert({featureId , compute2DPolygonCentroid(Featurepoints_xy[featureId], FeatureAreaVector[featureId])});

        }
    } 
}
