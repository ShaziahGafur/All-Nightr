#include "m1.h"
#include "globals.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "m2.h"
#include "m3.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "ezgl/point.hpp"
#include "intersection_data.h"
#include "poiStruct.h"
#include "poiStruct.cpp"
#include <vector>
#include <set>
#include <unordered_set>
#include <string>
#include <cmath>
#include <iostream>
#include <sstream>
#include <gtk/gtk.h>
#include <time.h>
#include <regex>
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

enum Mode {
    base = 0,
    directions,
    help,
    find,
    load
};

//nightmode street color scheme
//streets

ezgl::color Colour_driving_highlight(252, 248, 3, 200); //bright yellow
ezgl::color Colour_walking_highlight(3, 215, 252, 200); //bright blue 

ezgl::color Colour_unclassified(114, 111, 85, 255); //yellow (1 = least opaque)
ezgl::color Colour_motorway(100, 38, 7); // orange (3 = lightest)
ezgl::color Colour_trunk(129, 68, 6, 255); // orange (2)
ezgl::color Colour_primary(174, 133, 40, 255); // orange (1 = darkest)
ezgl::color Colour_secondary(154, 92, 0, 255); //yellow (3 = lightest)
ezgl::color Colour_tertiary(152, 122, 0, 255); // yellow (2 )
ezgl::color Colour_residential(114, 111, 85, 255); // yellow (1 = darkest)


#define default_turn_penalty 0.25 // (15s converted to minutes)

//features
//poi

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
/**************   DATA STRUCTURES ****************/
//Vector --> key: [intersection ID] value: [intersection_data struct]
std::vector<intersection_data> intersections;

//Hashtable --> key: [OSMway] value: [Road Type (e.g. 0 = Unknown, 1 = motorway...)]
std::unordered_map<OSMID, RoadType> WaybyRoadType;

//Hashtable --> key: [feature id] value: [centroid (x,y)]
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

//when displaying the area around an intersection, these two values depict comfortable dimensions of Lat and Lon
const double intersectionViewLon = 0.004; //Only one dimension is required as ezgl takes care of the other dimension for us
ezgl::rectangle new_world;
bool navigateScreen; 

//variable that keeps track of what state the GUI is currently in
Mode CurrentMode = base;

//current weekday. Used to determine hours of operation
std::string Weekday;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/************  FUNCTION DECLARATIONS  ***********/
void draw_map_blank_canvas ();
void draw_main_canvas (ezgl::renderer *g);

//  CONVERSIONS //
double lon_from_x (double x);
double lat_from_y (double x);
double y_from_lat (double lat);
double x_from_lon (double lon);
std::pair < double, double > latLonToCartesian (LatLon latLonPoint);
double getRotationAngleForText(std::pair <double, double> xyFrom, std::pair <double, double> xyTo);
double getRotationAngle(std::pair <double, double> xyFrom, std::pair <double, double> xyTo);

//------------------------------------------------------------------------------
//  DRAWING //
//also populates intersections vector
void find_map_bounds();
void drawFeature_byType(int feature_type, ezgl::renderer *g);
void drawFeatures(ezgl::renderer *g);
void draw_feature_names(ezgl::renderer *g);
void draw_streets(ezgl::renderer *g);
void draw_street_name(ezgl::renderer *g, std::pair<double, double> & xyFrom, std::pair<double, double> & xyTo, double& segmentLength, std::string& streetName, bool oneWay);
void draw_intersections(ezgl::renderer *g);  
void draw_points_of_interest(ezgl::renderer *g);
void clearIntersection_highlights();

//------------------------------------------------------------------------------
// POPULATING GLOBAL VARIABLES //
void populatePointsOfInterest();
void populateWaybyRoadType();
void populateFeatureIds_byType();
void populateFeaturepoints_xy();
void populateFeatureCentroids();

//------------------------------------------------------------------------------
// MISCELLANEOUS
ezgl::point2d compute2DPolygonCentroid(int& featureId, std::vector<ezgl::point2d> &vertices, double& area);
ezgl::point2d find_PolyLine_Middle(int featureId);
std::string get_operationHours(const OSMNode* poi_OSMentity);
bool extract_streets_from_text(const char* text, std::string& street1, std::string& street2);
//returns a vector with all of the possible intersections given a set of street_ids
std::vector< std::vector<int> >  get_intersection_and_suggestions(std::vector<int>& street_ids_1, std::vector<int>& street_ids_2, std::string& suggested_streets);
int get_intersection(std::vector<int>& street_ids_1, std::vector<int>& street_ids_2);

//------------------------------------------------------------------------------
// APPLICATION
void act_on_mouse_click( ezgl:: application* app, GdkEventButton* event, double x_click, double y_click);
void find_button(GtkWidget *widget, ezgl::application *application);
void load_map_button(GtkWidget* widget, ezgl::application *application);
void initial_setup(ezgl::application *application, bool /*new_window*/);
void directions_button(GtkWidget* widget, ezgl::application *application);
void done_button(GtkWidget* widget, ezgl::application *application);
void help_button(GtkWidget* widget, ezgl::application *application);
void go_button(GtkWidget* widget, ezgl::application *application);
void on_dialog_response(GtkDialog *dialog, gint response_id, gpointer user_data);
void hide_direction_entries(ezgl::application *application);
void show_direction_entries(ezgl::application *application);

/************************************************/


/**
 * This function sets up fundamental variables for drawing the map, 
 * such as max & min lat and lons of the map for developing a scale for the map and 
 * populating global variables.
 * After, this function calls draw_map_blank_canvas(), which manages setting up the application & environment for the map to function
 */

void find_map_bounds(){
    //find the maximum and minimum lat & lons of the map based on intersections 
    max_lat = getIntersectionPosition(0).lat(); 
    min_lat = max_lat;
    max_lon = getIntersectionPosition(0).lon();
    min_lon = max_lon;
    
    //initialize global intersections vector
    intersections.clear();
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
}

void draw_map(){

    find_map_bounds();
    
    //get the current weekday
    std::time_t now = std::time(0);
    std::stringstream date(ctime(&now));
    date >> Weekday;
    
    //populate all global variables
    populateWaybyRoadType();
    populateFeatureIds_byType();
    populatePointsOfInterest();
    populateFeaturepoints_xy();
    populateFeatureCentroids();
    
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
    
    if (navigateScreen){
        g->set_visible_world(new_world); 
    }
    navigateScreen = false; //resetting for the next iteration
    
    //Use these for creating thresholds for zooming
//    std::cout<<"\nscale_factor: "<<scale_factor;
//    std::cout<<"\nzoom: "<<zoom;
    
    //Drawing Background
    //***********************************************************************************
    g->set_color (32, 32, 32, 255);
    g->fill_rectangle({min_lon,min_lat}, {max_lon, max_lat});
    
//  Draw all types of features  
    drawFeatures(g);
    
//   Drawing Streets
    draw_streets(g);

    //Draw Feature Names
    draw_feature_names(g);
     
    //Draw POIs
    draw_points_of_interest(g);
    
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

double getRotationAngleForText(std::pair <double, double> xyFrom, std::pair <double, double> xyTo){
    
    double rotationAngle = atan2(xyFrom.second - xyTo.second, xyFrom.first - xyTo.first) / DEGREE_TO_RADIAN;
//    std::cout<<"rotation angle value: "<<rotationAngle<<"\n";
    if (rotationAngle > 90) {
        rotationAngle = rotationAngle - 180;
    }
    if (rotationAngle < -90) {
        rotationAngle = rotationAngle + 180;
    }
    
    return rotationAngle;
}

double getRotationAngle(std::pair <double, double> xyFrom, std::pair <double, double> xyTo){
    
    double rotationAngle = atan2(xyFrom.second - xyTo.second, xyFrom.first - xyTo.first) / DEGREE_TO_RADIAN;

//    if (rotationAngle > 90) {
//        rotationAngle = rotationAngle - 180;
//    }
//    if (rotationAngle < -90) {
//        rotationAngle = rotationAngle + 180;
//    }
    
    return rotationAngle;
}

void populatePointsOfInterest(){
    //clear any remnant data
    PointsOfInterest.clear();
    
    //store value of POI
    std::string value;
    
    poiStruct poiData;
    LatLon latlon;
    std::pair<double, double> xy;
    double x, y;
    std::vector<poiStruct> library, cafes, fast_food;
    
    OSMID poi_OSMID;
    const OSMNode* poi_OSMentity;
    
    //iterate through points of interest using layer 1
    for (unsigned poiIterator = 0; poiIterator < getNumPointsOfInterest(); poiIterator++){
        
        value = getPointOfInterestType(poiIterator);
    
        if (value == "library"){
            poiData.Name = getPointOfInterestName(poiIterator);
            poi_OSMID = getPointOfInterestOSMNodeID(poiIterator);
            poi_OSMentity = getNodeByIndex(OSMID_to_node.at(poi_OSMID));
            
            latlon = getPointOfInterestPosition(poiIterator);
            
            //conversion to cartesian
            x = x_from_lon (latlon.lon());
            y = y_from_lat (latlon.lat());
            xy = std::make_pair(x,y);
            poiData.xyCoordinates = xy;
            
            poiData.hours = get_operationHours(poi_OSMentity);
            
            library.push_back(poiData);
        }
        else if (value == "cafe"){
            poiData.Name = getPointOfInterestName(poiIterator);
            poi_OSMID = getPointOfInterestOSMNodeID(poiIterator);
            poi_OSMentity = getNodeByIndex(OSMID_to_node.at(poi_OSMID));
            
            latlon = getPointOfInterestPosition(poiIterator);
            
            x = x_from_lon (latlon.lon());
            y = y_from_lat (latlon.lat());
            xy = std::make_pair(x,y);
            poiData.xyCoordinates = xy;
            
            poiData.hours = get_operationHours(poi_OSMentity);
            
            cafes.push_back(poiData);
        }
        else if (value == "fast_food"){
            poiData.Name = getPointOfInterestName(poiIterator);
            poi_OSMID = getPointOfInterestOSMNodeID(poiIterator);
            poi_OSMentity = getNodeByIndex(OSMID_to_node.at(poi_OSMID));
            
            latlon = getPointOfInterestPosition(poiIterator);
            
            x = x_from_lon (latlon.lon());
            y = y_from_lat (latlon.lat());
            xy = std::make_pair(x,y);
            poiData.xyCoordinates = xy;
            
            poiData.hours = get_operationHours(poi_OSMentity);
            
            fast_food.push_back(poiData);
        }
    }
    
    PointsOfInterest[0] = library;
    PointsOfInterest[1] = cafes;
    PointsOfInterest[2] = fast_food;
}

/*
 * Searches through all OSM Entities and their tags to match OSMIDs with RoadType (e.g. motorway, primary roads, residential, etc.)
 * Populates global variable WaybyRoadType
 * The "highway" OSM key is used to access the road type
 */
void populateWaybyRoadType(){
    //clear any remnant data that is there now
    WaybyRoadType.clear();
    
    std::string key,value;
    RoadType road_type;
    
    
    //Searches through all OSM Ways and their keys to determine road types
    for (unsigned i = 0; i < getNumberOfWays(); i++){
        //creates a pointer that enables accessing the way's OSMID
        const OSMWay* wayPtr = getWayByIndex(i);
        key="N/A"; //in case the way has no keys
        for(int j = 0; j<getTagCount(wayPtr); ++j){
            std::tie(key,value) = getTagPair(wayPtr,j);
            
            if (key == "highway"){
                
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

                WaybyRoadType.insert({wayPtr->id(), road_type}); //take the value of key and store it in global variable with OSMID
                break;
            }
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
            
            if (key == "highway"){
                
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

                WaybyRoadType.insert({relationPtr->id(), road_type}); //take the value of key and store it in global variable with OSMID
                break;
            }
        }
        
    }
    
    //Searches through all OSM Nodes and their keys to determine road types
    //also looks for POI tags
    //Repeats same process as above, for node 
    for (unsigned i = 0; i < getNumberOfNodes(); i++){
        
        //creates a pointer that enables accessing the node's OSMID
        const OSMNode* nodePtr = getNodeByIndex(i);
        
        key="N/A"; //in case the way has no keys
        for(int j=0;j<getTagCount(nodePtr); ++j){
            
            std::tie(key,value) = getTagPair(nodePtr,j);
            
            if (key == "highway"){
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

                WaybyRoadType.insert({nodePtr->id(), road_type}); //take the value of key and store it in global variable with OSMID
                //breaks out of the inner tag loop, increments to next node
                break;
            }

        }
    }
}

void populateFeatureIds_byType(){
    //clear any remnant data
    FeatureIds_byType.clear();
    
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

    //prepare full window title string
    std::string fullTitle = "All Nightr - " + MapName;
    //convert to char* type (to pass into gtk function)
    const char* windowTitle = fullTitle.c_str();
    GtkWindow* mainWindowPointer = (GtkWindow*) application->get_object("MainWindow"); 
    gtk_window_set_title(mainWindowPointer, windowTitle);
    
    navigateScreen = false;
    
  // link it with find_button callback function
    GtkButton* findButton = (GtkButton*) application->get_object("find");
    g_signal_connect(findButton,"clicked",G_CALLBACK(find_button),application);
    GtkWidget *find_image = gtk_image_new_from_file("libstreetmap/resources/find_icon.png");
    gtk_button_set_image(GTK_BUTTON(findButton), find_image);
    gtk_button_set_label(GTK_BUTTON(findButton), NULL);
   
//    GtkButton* directions_goButton = (GtkButton*) application->get_object("directions_go");   
//    g_signal_connect(go_button, "clicked", G_CALLBACK(go_button), application);
    
    //link load button to load_map call-back function
    GtkButton* loadButton = (GtkButton*) application->get_object("load_map");   
    g_signal_connect(loadButton, "clicked", G_CALLBACK(load_map_button), application);

   //link Get direction button to directions_button call-back function
    GtkButton* directionsButton = (GtkButton*) application->get_object("directions");   
    g_signal_connect(directionsButton, "clicked", G_CALLBACK(directions_button), application);
    GtkWidget *directions_image = gtk_image_new_from_file("libstreetmap/resources/direction_icon.png");
    gtk_button_set_image(GTK_BUTTON(directionsButton), directions_image);
    gtk_button_set_label(GTK_BUTTON(directionsButton), NULL);
    
    //link help button to help call-back function
    GtkButton* helpButton = (GtkButton*) application->get_object("help");   
    g_signal_connect(helpButton, "clicked", G_CALLBACK(help_button), application);
    GtkWidget *help_image = gtk_image_new_from_file("libstreetmap/resources/help_icon.png");
    gtk_button_set_image(GTK_BUTTON(helpButton), help_image);
    gtk_button_set_label(GTK_BUTTON(helpButton), NULL);
    
    //link Done button to done_button call-back function
    GtkButton* Done = (GtkButton*) application->get_object("done");
    g_signal_connect(Done, "clicked", G_CALLBACK(done_button), application);
    
    //link Go button to done_button call-back function
    GtkButton* directions_goButton = (GtkButton*) application->get_object("directions_go");
    g_signal_connect(directions_goButton, "clicked", G_CALLBACK(go_button), application);
    
    //*hide all unnecessary widgets
    //hide the Done button
    GtkWidget* done_widgetPtr = (GtkWidget*)application->get_object("done");
    gtk_widget_hide(done_widgetPtr);
    
    //to start with, extra directions text input is hidden
    hide_direction_entries(application);
}

void find_button(GtkWidget* widget, ezgl::application *application){
    //update global variable
    CurrentMode = find;
    
    //show the Done button
    GtkWidget* done_widgetPtr = (GtkWidget*)application->get_object("done");
    gtk_widget_show(done_widgetPtr);
    //show street suggestions
    GtkWidget* searchResults_widgetPtr = (GtkWidget*)application->get_object("SearchStreetsResults");
    gtk_widget_show(searchResults_widgetPtr);
    
    //regardless of what mode it is, reset the segment highlights
  while(!(segmentsHighlighted.empty())){
        int segUnhighlightID = segmentsHighlighted.back();
        segmentHighlight[segUnhighlightID].walking = false;
        segmentHighlight[segUnhighlightID].driving = false;
        segmentsHighlighted.pop_back();
    }

    //two string variables needed to interpret input
    std::string street1, street2, suggested_streets;
    
    // Get the GtkEntry object
    GtkEntry* text_entry = (GtkEntry *) application->get_object("TextInput");
    
    // Get the text written in the widget
    const char* text = gtk_entry_get_text(text_entry);
    
    if (extract_streets_from_text(text, street1, street2) == false){
        application->update_message ("Please enter two street names (e.g. Main street and Danforth)"); 
        return;
    }
    //obtains all of the possible streetIds that match the entered street names
    std::vector<int> street_ids_1 = find_street_ids_from_partial_street_name(street1);
    std::vector<int> street_ids_2 = find_street_ids_from_partial_street_name(street2);
    
    //a vector with all of the possible intersections given a set of street_ids
    std::vector< std::vector<int> > streetIntersections;
    
    streetIntersections = get_intersection_and_suggestions(street_ids_1, street_ids_2, suggested_streets);
    
    //sting which holds the primary intersection names
    std::string intersectionNames = "";
    
    if (streetIntersections.empty())
        intersectionNames = "No results found";
        
    else{
        //clear other highlighted intersections, 
        clearIntersection_highlights();
        
        //add the intersecting street names to string
        //note: two streets may have more than one intersection
        for(int i = 0; i < streetIntersections[0].size(); i++){
            intersectionNames += getIntersectionName(streetIntersections[0][i]);
            //if there are more intersections left
            if (i+1 != streetIntersections[0].size())
                intersectionNames+=", ";

            Highlighted_intersections.push_back(streetIntersections[0][i]);
            intersections[streetIntersections[0][i]].highlight = true;
        }
        
        //update global variables to navigate screen to the intersection
        LatLon intersectionFound = getIntersectionPosition(streetIntersections[0][0]);
        ezgl::rectangle intersectionView({x_from_lon(intersectionFound.lon()-(intersectionViewLon/2)), y_from_lat(intersectionFound.lat())},{x_from_lon(intersectionFound.lon()+(intersectionViewLon/2)), y_from_lat(intersectionFound.lat())}); 
        new_world = intersectionView;
        navigateScreen = true;
    }

    application->update_message (intersectionNames); 
    
    // Redraw the graphics
    application->refresh_drawing(); 
    
    GtkWidget* view = (GtkWidget *)application->get_object("SearchStreetsResults");
    GtkTextView * textViewPtr = GTK_TEXT_VIEW(view);
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(textViewPtr);
    gtk_text_buffer_set_text(buffer, "  ", -1); 
    
    //if there are alternative intersections
    if (suggested_streets.empty() == false){
        //append message
        suggested_streets = "\nDid you mean?\n\n" + suggested_streets;
        gtk_text_buffer_set_text(buffer, suggested_streets.c_str(), -1);

    }
    

     //INCLUDE CODE BELOW ONCE MODE 2 IS SET UP

    //else if mode 2 (Direction mode)
    //update global variables to navigate screen to directions
    
//    int startID = 1, destID = 9;
//    LatLon start = IntersectionCoordinates[startID];
//    LatLon dest = IntersectionCoordinates[destID];
//    std::pair <double, double> xyStart = latLonToCartesian(start);
//    std::pair <double, double> xyDest = latLonToCartesian(dest);
//    double xDiff = abs(xyStart.first - xyDest.first);
//    double yDiff = abs(xyStart.second - xyDest.second);
//    double xAvg = (xyStart.first + xyDest.first)/2;
//    double yAvg = (xyStart.second + xyDest.second)/2;
//    
//    if (xDiff > yDiff){ //if greater dominance in x direction
//        ezgl::rectangle directionsView({(xAvg-(xDiff*2/3)), yAvg},{(xAvg+(xDiff*2/3)), yAvg}); 
//        new_world = directionsView;
//    }
//    else{
//        ezgl::rectangle directionsView({xAvg, yAvg-(yDiff*2/3)},{xAvg, yAvg+(yDiff*2/3)}); 
//        new_world = directionsView;
//    }
//        
//    navigateScreen = true;
    
}

//vertices are the feature points in xy coordinates, vertexCount = number of feature points
ezgl::point2d compute2DPolygonCentroid(int& featureId, std::vector< ezgl::point2d > & vertices, double& area)
{
    if(area == 0)
        return find_PolyLine_Middle(featureId);
    
    else{
        ezgl::point2d centroid(0.0, 0.0);
        double x0 = 0.0; // Current vertex X
        double y0 = 0.0; // Current vertex Y
        double x1 = 0.0; // Next vertex X
        double y1 = 0.0; // Next vertex Y
        double a = 0.0;  //multiplication factor (xi*yi+1 - xi+1yi))
        double AR = 0.0;
        
        // For all vertices
        for (int i = 0; (i+1) < vertices.size(); ++i){
            x0 = vertices[i].x;
            x1 = vertices[i+1].x;

            y0 = vertices[i].y;
            y1 = vertices[i+1].y;

            a = (x0*y1) - (x1*y0);
            AR += a;
            centroid.x += (x0 + x1)*a;
            centroid.y += (y0 + y1)*a;
        }
        AR *= 0.5; 
        centroid.x /= (6.0 * AR);
        centroid.y /= (6.0 * AR);  
        
        return centroid;
    }
}

void draw_feature_names(ezgl::renderer *g){
    
    int feature_type;

    g->set_text_rotation(0);

    //helpful for debugging
    //std::cout << "max_width " << max_width << "\n";
    //float width = max_width;
    //g->fill_rectangle({feature_centroid.x-(width/2),feature_centroid.y-(width/2)}, {feature_centroid.x + (width/2), feature_centroid.y + (width/2)});

    //loops through FeatureCentroids hashtable, consisting of feature centroid locations with unique names
    for( std::unordered_map< int, ezgl::point2d >::iterator it = FeatureCentroids.begin();  it != FeatureCentroids.end(); it++){
        
        feature_type = getFeatureType((*it).first);
        
        if( (scale_factor < 0.05) && (feature_type != Unknown) && (feature_type != Building) && (getFeatureName((*it).first) != "<noname>") ){
               
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
            g->draw_text((*it).second, getFeatureName((*it).first));
            
//            g->fill_rectangle({(*it).second.x-50,(*it).second.y-50}, {(*it).second.x + 50, (*it).second.y + 50});
        }
    

       // g->fill_rectangle({FeatureCentroids.at(FeatureIds_uniqueNames[i]).x-(10),FeatureCentroids.at(FeatureIds_uniqueNames[i]).y-(10)}, {FeatureCentroids.at(FeatureIds_uniqueNames[i]).x + (10), FeatureCentroids.at(FeatureIds_uniqueNames[i]).y + (10)});
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

//function draws points of interest names on map
void draw_points_of_interest(ezgl::renderer *g){
    //use two different enables because there are many more restaurants than libraries
    bool enable_poi_eat = true;
    bool enable_poi_lib = true;
    //if too zoomed out, don't draw any names
    if (scale_factor > 0.2){
        enable_poi_eat = false;
        enable_poi_lib = false;
    }
    else if (scale_factor > 0.02)
        enable_poi_eat = false;
    
    //Extract vectors from PointsOfInterest vector to make it easier to parse through each type separately
    std::vector<poiStruct> library = PointsOfInterest[0];
    std::vector<poiStruct> cafes = PointsOfInterest[1];
    std::vector<poiStruct> fast_food = PointsOfInterest[2];
    
    //Declare iterator to go through each vector
    std::vector<poiStruct>::iterator it = library.begin();
    
    //Variables to extract data from poi struct
    std::pair<double,double> xyCoordinates;
    std::string poiName;
    
    //loop through library vector, extract data and draw names of libraries on map
    while(it != library.end()){
        xyCoordinates = (*it).xyCoordinates;
        poiName = (*it).Name;
        
        if (enable_poi_lib){
            g->set_color (178,141,196, 255);
            //if 24/7, draw text in bold
            if((*it).hours == "24/7")
                g->format_font("cairo", ezgl::font_slant::normal, ezgl::font_weight::bold);
            else
                g->format_font("cairo", ezgl::font_slant::normal, ezgl::font_weight::normal);

            g->draw_text({xyCoordinates.first, xyCoordinates.second}, poiName);
        }
        it++;
    }
    
    //loop through cafes vector, extract data and draw names of cafes on map
    it = cafes.begin();
    while(it != cafes.end()){
        xyCoordinates = (*it).xyCoordinates;
        poiName = (*it).Name;
        
        if (enable_poi_eat){
            g->set_color (189,164, 143, 255);
            //if 24/7, draw text in bold
            if((*it).hours == "24/7")
                g->format_font("cairo", ezgl::font_slant::normal, ezgl::font_weight::bold);
            else
                g->format_font("cairo", ezgl::font_slant::normal, ezgl::font_weight::normal);
        
            g->draw_text({xyCoordinates.first, xyCoordinates.second}, poiName);
        }
        it++;
    }
    
    //loop through fast_food vector, extract data and draw names of fast_foods on map
    it = fast_food.begin();
    while(it != fast_food.end()){
        xyCoordinates = (*it).xyCoordinates;
        poiName = (*it).Name;
        
        if (enable_poi_eat){
            g->set_color (233,128,96,255);
            //if 24/7, draw text in bold
            if((*it).hours == "24/7")
                g->format_font("cairo", ezgl::font_slant::normal, ezgl::font_weight::bold);
            else
                g->format_font("cairo", ezgl::font_slant::normal, ezgl::font_weight::normal);
            
            g->draw_text({xyCoordinates.first, xyCoordinates.second}, poiName);
        }
        
        it++;
    }
    //set font back to default
    g->format_font("cairo", ezgl::font_slant::normal, ezgl::font_weight::normal);
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
                
                if (segmentHighlight[i].driving){
//                    g->set_line_width (2);
                    g->set_color (Colour_driving_highlight);
                }
                else if (segmentHighlight[i].walking){
//                    g->set_line_width (2);
                    g->set_color (Colour_walking_highlight);
                }
                
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

                            g->set_text_rotation(getRotationAngleForText(xyLeftMax, xyRightMax));
                            g->set_color(ezgl::WHITE);
                            g->draw_text({xMiddleOfSegment, yMiddleOfSegment}, streetSegName, segmentLength, segmentLength);
                        }
                    }
                }
            }
        }
    }
}


void draw_street_name(ezgl::renderer* g, std::pair<double, double>& xyFrom, std::pair<double, double>& xyTo, double& segmentLength, std::string& streetName, bool oneWay)
{
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

    //Variables for drawing text of street segments
    //uses find_street_segment_length from m1
    double xMiddleOfSegment, yMiddleOfSegment;

    //Greater screen_ratio = more reprinting of street name and arrows: < (for oneway streets)
    double screen_ratio = segmentLength / scale_factor; //screen_ratio is the available length of the street segment on screen.
    g ->set_color(224, 224, 224, 255);

        xMiddleOfSegment = 0.5 * (xyFrom.first + xyTo.first);
        yMiddleOfSegment = 0.5 * (xyFrom.second + xyTo.second);
        
        //set rotation for text to draw
        g->set_text_rotation(getRotationAngleForText(xyFrom, xyTo));

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
              g->set_color(ezgl::LIME_GREEN);
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
        intersections[*it].highlight = false;
    }
}

//populates Featurepoints_xy vector needed to draw features and feature names
void populateFeaturepoints_xy(){
    //clear any data that is there
    Featurepoints_xy.clear();
    
    //uses StreetsDatabase function
    Featurepoints_xy.resize(getNumFeatures());
    
    int numOfFeaturePoints;
    
    for(int featureId = 0; featureId < getNumFeatures(); ++featureId){

        
        if(FeatureAreaVector[featureId] == 0){
            //it is a polyline
            numOfFeaturePoints = getFeaturePointCount(featureId);
        }
        else{
            //it is a polygon and the first and last points are identical
            numOfFeaturePoints = getFeaturePointCount(featureId) - 1;
        }

        //iterate through feature points then convert to (x,y) coordinates
        //must stop at (numOfFeaturePoints-1) because
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
    } 
}

//populates the FeatureCentroids unordered_map
//first have to define all of the unique Names 
//then can associate a featureid with that name (largest area)
//then, populate the FeatureCentroids vector using that featureid)
void populateFeatureCentroids(){
    //erase any remnant data
    FeatureCentroids.clear();
    
    std::unordered_map<std::string, int> uniqueNames;
    std::unordered_map<std::string, int>::const_iterator found_ptr;
    
    //insert the unique Feature names into the uniqueNames unordered_map
    for( int featureid = 0;  featureid < getNumFeatures(); featureid++){
        
        found_ptr = uniqueNames.find(getFeatureName(featureid));
        
        //if feature with same name already exists in the uniqueNames unordered map, insert only if area is larger
        if(found_ptr != uniqueNames.end()){
            
            //if feature name with larger area found, replace uniqueNames featureid
             if(FeatureAreaVector[featureid] > uniqueNames[getFeatureName(featureid)])
                 uniqueNames.insert({getFeatureName(featureid), featureid});
             continue;
        }
        //otherwise, insert the first/only instance of that feature name       
        uniqueNames.insert({getFeatureName(featureid), featureid});
    }
    
    //for each unique feature name, find it's centroid and insert it into the FeatureCentroids vector (used to draw Feature names)
    for(std::unordered_map<std::string, int>::iterator it = uniqueNames.begin(); it != uniqueNames.end(); it++){
        
        FeatureCentroids.insert( {(*it).second, compute2DPolygonCentroid((*it).second, Featurepoints_xy[(*it).second], FeatureAreaVector[(*it).second] ) });
    }
}

//Funciton looks though tags to get hours of operation
//If open from 10-11pm for current weekday, returns true
std::string get_operationHours(const OSMNode* poi_OSMentity){
    std::string key, value;
    
    for(int j = 0; j < getTagCount(poi_OSMentity); j++){
        std::tie(key, value) = getTagPair(poi_OSMentity, j);
        if(key == "opening_hours"){
            //std::cout << value << "\n";
            return value;
        }
    }
    return "not available";
}

void load_map_button(GtkWidget* widget, ezgl::application *application)
{   
    CurrentMode = load;
    // string variables needed to interpret input
    std::string city_input, country_input, new_map;
    std::string path_directory = "/cad2/ece297s/public/maps/";
    std::string file_type = ".streets.bin";
    
    // Get the GtkEntry object
    GtkEntry* text_entry = (GtkEntry *) application->get_object("TextInput");
    
    // Get the text written in the widget
    const char* text = gtk_entry_get_text(text_entry);
    
    //convert into a string
    std::string map_input(text);
    
    //check if nothing was entered
    if(map_input.empty()){
        std::cout << "No map entered";
        return;
    }
     //otherwise parse the input
    
    //helps to parse entered text
    std::size_t pos = map_input.find(",");
    
   //extract city (get all characters up to, but not including ",")
    city_input = map_input.substr(0, pos);

    //extract country (all characters after "," and a [space])
    country_input = map_input.substr(pos + 2);
    
    //the new map is city_country
    new_map = city_input + "_" + country_input;
    
    //1) check if they are loading the same map (waste of time!)
    if((city_input + " " + country_input) == MapName){
        application->update_message ("Map already loaded!"); 
    }
    
    else{
        //also empties all global variables from m1
        close_map();
        
        bool load_success = load_map( path_directory + new_map + file_type); 
        
        if(!load_success){
            application -> update_message ( "Failed to load map '" + path_directory + new_map + file_type );
        }
        
        else{
            Highlighted_intersections.clear();
            
            //updates min_lon, min_lat, max_lon, max_lat Global Variables (needed to populate global variables)
            //also populates intersections vector
            find_map_bounds();
            
            //re-populate all global variables
            populateWaybyRoadType();
            populateFeatureIds_byType();
            populatePointsOfInterest();
            populateFeaturepoints_xy();
            populateFeatureCentroids();

            ezgl::rectangle new_initial_world({min_lon, min_lat}, {max_lon, max_lat}); 

            application->change_canvas_world_coordinates("MainCanvas", new_initial_world);

            application->refresh_drawing();
            
            application->update_message ("Map loaded"); 

            //prepare full window title string
            std::string fullTitle = "All Nightr - " + MapName;
            //convert to char* type (to pass into gtk function)
            const char* windowTitle = fullTitle.c_str();
            GtkWindow* mainWindowPointer = (GtkWindow*) application->get_object("MainWindow"); 
            gtk_window_set_title(mainWindowPointer, windowTitle);
        }

    }

    return;
}

void directions_button(GtkWidget* widget, ezgl::application *application){
    //update global variable
    CurrentMode = directions;

    //**Adjust Layout to Direction Mode
    //show additional buttons/ text entry
    show_direction_entries(application);
    //show the Done button
    GtkWidget* done_widgetPtr = (GtkWidget*)application->get_object("done");
    gtk_widget_show(done_widgetPtr);
    
    //Update search bar text
    GtkEntry* TextInput_entry = (GtkEntry*) application->get_object("TextInput");
    gtk_entry_set_placeholder_text(TextInput_entry, "Enter destination Intersection");
    
    
    //application->refresh_drawing(); 

    
    /*
    //Get Intersections from Entries
    // two string variables needed to interpret input
    std::string street1, street2, suggested_streets;
    
    //Text prompt for directions mode 
    std::string directions_prompt = "";
    
    int destinationId, sourceId;
    
    // Get the text written in the widget
    const char* text = gtk_entry_get_text(entry1);
    
    if (extract_streets_from_text(text, street1, street2) == false)
        //insert some kind of error message
        return;
    
    //obtains all of the possible streetIds that match the entered street names
    std::vector<int> street_ids_1 = find_street_ids_from_partial_street_name(street1);
    std::vector<int> street_ids_2 = find_street_ids_from_partial_street_name(street2);
    
    //a vector with all of the possible intersections given a set of street_ids
    std::vector< std::vector<int> > streetIntersections;
    
    streetIntersections = get_intersection_and_suggestions(street_ids_1, street_ids_2, suggested_streets);
    
    //string which holds the primary intersection names
    std::string intersectionNames = "";
    
    if (streetIntersections.empty())
        directions_prompt = "No results found";
        
    else{
        //clear other highlighted intersections, 
        clearIntersection_highlights();
        
        destinationId = streetIntersections[0][0];
        
        directions_prompt = getIntersectionName(streetIntersections[0][0]);

        Highlighted_intersections.push_back(streetIntersections[0][0]);
        intersections[streetIntersections[0][0]].highlight = true;
        
//        
//        //update global variables to navigate screen to the intersection
//        LatLon intersectionFound = getIntersectionPosition(streetIntersections[0][0]);
//        ezgl::rectangle intersectionView({x_from_lon(intersectionFound.lon()-(intersectionViewLon/2)), y_from_lat(intersectionFound.lat())},{x_from_lon(intersectionFound.lon()+(intersectionViewLon/2)), y_from_lat(intersectionFound.lat())}); 
//        new_world = intersectionView;
//        navigateScreen = true;
//    }

    application->update_message ("Directions Mode"); 
    
    directions_prompt = "Destination: \n" + directions_prompt + "\n\n" + "Please enter starting location in search bar: ";
    }
    
    GtkWidget* view = (GtkWidget *)application->get_object("SearchStreetsResults");
    GtkTextView * textViewPtr = GTK_TEXT_VIEW(view);
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(textViewPtr);
    
    gtk_text_buffer_set_text(buffer, directions_prompt.c_str(), -1);
 */   
    
}

void help_button(GtkWidget* widget, ezgl::application *application){
    GObject *window; // the parent window over which to add the dialog
    GtkWidget *content_area; // the content area of the dialog
    GtkWidget *label; // the label we will create to display a message in the content area
    GtkWidget *dialog; // the dialog box we will create
    
    // get a pointer to the main application window
    window = application->get_object(application->get_main_window_id().c_str());
    
    // Create the dialog window.
    // Modal windows prevent interaction with other windows in the same application
    dialog = gtk_dialog_new_with_buttons(
        "Help Window",
        (GtkWindow*) window,
        GTK_DIALOG_MODAL,
        ("OK"),
        GTK_RESPONSE_DELETE_EVENT,
        NULL);
    
    // Create a label and attach it to the content area of the dialog
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    label = gtk_label_new("Welcome to All-Nightr! \n Using this app, you can locate intersections, find directions, and load different cities! \n\n Locating Intersections \n 1.\tType in the intersection into the search bar.\n2.\t"
            "Hit the search icon to find your intersection!\n3.\tPress Done to clear the screen\n\n"
            "Finding Directions\n1.\tPress the directions icon to enter into directions mode.\n2.\tType in your destination and starting point into the search bars.\n3.\tView your path.\n4.\tPress Done to clear the screen\n\n"
            "Loading Cities\n1.\tType in the city and country into the search bar (e.g. city, country)\n2.\tPress the load map button to load your new map! ");
    gtk_container_add(GTK_CONTAINER(content_area), label);
    
    // The main purpose of this is to show dialog’s child widget, label
    gtk_widget_show_all(dialog);
    
    // Connecting the "response" signal from the user to the associated callback function
    g_signal_connect(GTK_DIALOG(dialog), "response", G_CALLBACK(on_dialog_response), NULL);
    
}
bool extract_streets_from_text(const char* text, std::string& street1, std::string& street2){
    
    //convert into a string
    std::string text_input(text);
    
    //if text is empty or only white spaces
    if (text_input.empty() || (text_input.find_first_not_of(' ') == std::string::npos)){ 
        std::cout<<"Empty Find parameters\n";
        return false;
    }
       
    std::smatch m; //typedef std::match_results<string>
    
    //get street names to be used find function
    //format (street1)(and or &)(street2)
    std::regex ex("([a-zA-Z]+[\\s\\-[a-zA-Z]+]*)+\\s(and|&)\\s([a-zA-Z]+[\\s\\-[a-zA-Z]+]*)");
    
    bool found = std::regex_search(text_input, m, ex);
     
    if(found){
        street1 = m[1].str();
        street2 = m[3].str();
        return true;
    }
    
    else return false;

}

//returns a vector with all of the possible intersections given a set of street_ids
std::vector< std::vector<int> >  get_intersection_and_suggestions(std::vector<int>& street_ids_1, std::vector<int>& street_ids_2, std::string& suggested_streets){
    
    std::pair<int, int> twoStreets;
    //a vector which holds all of the intersection possibilities
    std::vector< std::vector<int> > streetIntersections;
    std::vector<int> streetIntersections_temp;
    
    //nested for-loop which finds intersections between all streetId's returned by partial_street_name function
    //stops once a single intersection is found. (Other matches go to suggested streets)
    for(int str1_idx = 0; str1_idx < street_ids_1.size(); str1_idx++){
        
        twoStreets.first = street_ids_1[str1_idx];
        
        for(int str2_idx = 0; str2_idx < street_ids_2.size(); str2_idx++){
            
            twoStreets.second = street_ids_2[str2_idx];

            streetIntersections_temp = find_intersections_of_two_streets(twoStreets);
            
            //if an intersection is found...
            if(!streetIntersections_temp.empty()){
                
                //after first match found, all other names go into suggested_streets string
                if(streetIntersections.empty() == false)
                    suggested_streets += getStreetName(twoStreets.first)+" & "+getStreetName(twoStreets.second)+"\n";
                    
                streetIntersections.push_back(streetIntersections_temp);
            }  
        }
    }
    return streetIntersections;
}

//button returns to base mode, depending on which mode is active at the moment
void done_button(GtkWidget* widget, ezgl::application *application){
    
    if(CurrentMode == directions){
        
        hide_direction_entries(application);
                
    }
    else if (CurrentMode == find){
        clearIntersection_highlights();
        //hide street name suggestions
        GtkWidget* searchResults_widgetPtr = (GtkWidget*)application->get_object("SearchStreetsResults");
        gtk_widget_hide(searchResults_widgetPtr);
    }
    else if (CurrentMode == load){
        //do nothing
    }
    
    // clear search bar
    GtkEntry* TextInput_entry = (GtkEntry*) application->get_object("TextInput");
    gtk_entry_set_placeholder_text(TextInput_entry, "Enter an intersection name or Load a new map (e.g. tokyo, japan)...");
    
    //clear text dialog
    GtkWidget* view = (GtkWidget *)application->get_object("SearchStreetsResults");
    GtkTextView * textViewPtr = GTK_TEXT_VIEW(view);
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(textViewPtr);
    
    gtk_text_buffer_set_text(buffer, "", -1);
    
    GtkWidget* done_widgetPtr = (GtkWidget*)application->get_object("done");
    gtk_widget_hide(done_widgetPtr);
}

void on_dialog_response(GtkDialog *dialog, gint response_id, gpointer user_data)
{   
    switch(response_id) {

        case GTK_RESPONSE_DELETE_EVENT:
                                    gtk_widget_destroy(GTK_WIDGET (dialog));
                                    break;

        default:                    std::cout << "UNKNOWN ";
                                    break;
    }       
}

void hide_direction_entries(ezgl::application *application){
    //to start with, extra directions text input is hidden
    GtkWidget* directionsEntry_widgetPtr = (GtkWidget*)application->get_object("directions_entry");
    gtk_widget_hide(directionsEntry_widgetPtr);
    
    GtkWidget* speedEntry_widgetPtr = (GtkWidget*)application->get_object("time_limit");
    gtk_widget_hide(speedEntry_widgetPtr);
    
    GtkWidget* timeEntry_widgetPtr = (GtkWidget*)application->get_object("walking_speed");
    gtk_widget_hide(timeEntry_widgetPtr);
    
    GtkWidget* speedLabel_widgetPtr = (GtkWidget*)application->get_object("walking_speed_label");
    gtk_widget_hide(speedLabel_widgetPtr);
    
    GtkWidget* timeLabel_widgetPtr = (GtkWidget*)application->get_object("time_limit_label");
    gtk_widget_hide(timeLabel_widgetPtr);
    
    GtkWidget* uberLabel_widgetPtr = (GtkWidget*)application->get_object("uber_label");
    gtk_widget_hide(uberLabel_widgetPtr);
    
    GtkWidget* uberToggle_widgetPtr = (GtkWidget*)application->get_object("uber");
    gtk_widget_hide(uberToggle_widgetPtr);
    
    GtkWidget* directionsGo_widgetPtr = (GtkWidget*)application->get_object("directions_go");
    gtk_widget_hide(directionsGo_widgetPtr);
    
    GtkWidget* searchResults_widgetPtr = (GtkWidget*)application->get_object("SearchStreetsResults");
    gtk_widget_hide(searchResults_widgetPtr);
}

void show_direction_entries(ezgl::application *application){
        //to start with, extra directions text input is hidden
    GtkWidget* directionsEntry_widgetPtr = (GtkWidget*)application->get_object("directions_entry");
    gtk_widget_show(directionsEntry_widgetPtr);
    
    GtkWidget* speedEntry_widgetPtr = (GtkWidget*)application->get_object("time_limit");
    gtk_widget_show(speedEntry_widgetPtr);
    
    GtkWidget* timeEntry_widgetPtr = (GtkWidget*)application->get_object("walking_speed");
    gtk_widget_show(timeEntry_widgetPtr);
    
    GtkWidget* speedLabel_widgetPtr = (GtkWidget*)application->get_object("walking_speed_label");
    gtk_widget_show(speedLabel_widgetPtr);
    
    GtkWidget* timeLabel_widgetPtr = (GtkWidget*)application->get_object("time_limit_label");
    gtk_widget_show(timeLabel_widgetPtr);
    
    GtkWidget* uberLabel_widgetPtr = (GtkWidget*)application->get_object("uber_label");
    gtk_widget_show(uberLabel_widgetPtr);
    
    GtkWidget* uberToggle_widgetPtr = (GtkWidget*)application->get_object("uber");
    gtk_widget_show(uberToggle_widgetPtr);
    
    GtkWidget* directionsGo_widgetPtr = (GtkWidget*)application->get_object("directions_go");
    gtk_widget_show(directionsGo_widgetPtr);
    
    GtkWidget* searchResults_widgetPtr = (GtkWidget*)application->get_object("SearchStreetsResults");
    gtk_widget_show(searchResults_widgetPtr);
}

void go_button(GtkWidget* widget, ezgl::application *application){
    //reset segmnent highlights
    while(!(segmentsHighlighted.empty())){
        int segUnhighlightID = segmentsHighlighted.back();
        segmentHighlight[segUnhighlightID].walking = false;
        segmentHighlight[segUnhighlightID].driving = false;
        segmentsHighlighted.pop_back();
    }
    
    std::pair<std::string, std::string> intersectionA;
    std::pair<std::string, std::string> intersectionB;
    
    // Get the GtkEntry object
    GtkEntry* text_entry = (GtkEntry *) application->get_object("TextInput");
    
    // Get the text written in the widget
    const char* textA = gtk_entry_get_text(text_entry);
    
    // Get the GtkEntry object
    text_entry = (GtkEntry *) application->get_object("directions_entry");
    
    // Get the text written in the widget
    const char* textB = gtk_entry_get_text(text_entry);
    
    if(extract_streets_from_text(textA, intersectionA.first, intersectionA.second) == false){
        application->update_message ("Please enter two street names (e.g. Main street and Danforth)"); 
        return;
    }
    
    if(extract_streets_from_text(textB, intersectionB.first, intersectionB.second) == false){
        application->update_message ("Please enter two street names (e.g. Main street and Danforth)"); 
        return;
    }
    
    //obtains all of the possible streetIds that match the entered street names
    std::vector<int> Astreet_ids_1 = find_street_ids_from_partial_street_name(intersectionA.first);
    std::vector<int> Astreet_ids_2 = find_street_ids_from_partial_street_name(intersectionA.second);
    
    //obtains all of the possible streetIds that match the entered street names
    std::vector<int> Bstreet_ids_1 = find_street_ids_from_partial_street_name(intersectionB.first);
    std::vector<int> Bstreet_ids_2 = find_street_ids_from_partial_street_name(intersectionB.second);
    
    //a vector with all of the possible intersections given a set of street_ids
    std::pair<int, int>intersectionIds{0,0};
    
    intersectionIds.first = get_intersection(Astreet_ids_1, Astreet_ids_1);
    intersectionIds.second = get_intersection(Astreet_ids_1, Astreet_ids_1);

    //sting which holds the primary intersection names
    std::string intersectionNames = "";
    
    GtkWidget* view = (GtkWidget *)application->get_object("SearchStreetsResults");
    GtkTextView * textViewPtr = GTK_TEXT_VIEW(view);
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(textViewPtr);
    gtk_text_buffer_set_text(buffer, "  ", -1); 
    
    if (intersectionIds.first == 0 && intersectionIds.second == 0){
        intersectionNames = "No results found";
        gtk_text_buffer_set_text(buffer, intersectionNames.c_str(), -1); 
    }
    
    else {
        intersectionNames = getIntersectionName(intersectionIds.first) + " to " +getIntersectionName(intersectionIds.second);
        
        //update global variables to navigate screen to directions
    
        int startID = intersectionIds.first, destID = intersectionIds.second;

        LatLon start = IntersectionCoordinates[startID];
        LatLon dest = IntersectionCoordinates[destID];
        std::pair <double, double> xyStart = latLonToCartesian(start);
        std::pair <double, double> xyDest = latLonToCartesian(dest);
        double xDiff = abs(xyStart.first - xyDest.first);
        double yDiff = abs(xyStart.second - xyDest.second);
        double xAvg = (xyStart.first + xyDest.first)/2;
        double yAvg = (xyStart.second + xyDest.second)/2;
    }
        
        ///............
}

//returns a vector with all of the possible intersections given a set of street_ids
int get_intersection(std::vector<int>& street_ids_1, std::vector<int>& street_ids_2){
    
    std::pair<int, int> twoStreets;
    //a vector which holds all of the intersection possibilities
    int streetIntersections = -1;
    
    //nested for-loop which finds intersections between all streetId's returned by partial_street_name function
    //stops once a single intersection is found. (Other matches go to suggested streets)
    for(int str1_idx = 0; (str1_idx < street_ids_1.size()) && (streetIntersections == -1); str1_idx++){
        
        twoStreets.first = street_ids_1[str1_idx];
        
        for(int str2_idx = 0; str2_idx < street_ids_2.size(); str2_idx++){
            
            twoStreets.second = street_ids_2[str2_idx];

            streetIntersections = find_intersections_of_two_streets(twoStreets)[0];
             
        }
    }
    return streetIntersections;
}