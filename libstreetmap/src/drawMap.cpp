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
//Vector --> key: [intersection ID] value: [intersection_data struct]
std::vector<intersection_data> intersections;

//Hashtable --> key: [OSMway] value: [road type]
std::unordered_map<OSMID, std::string> WayRoadType;

//Hashtable --> key: [feature id (POLYGONS ONLY)] value: [centroid (x,y)]
std::unordered_map< int, ezgl::point2d > FeatureCentroids;

//Vector --> key: [type], value = vector: [point of interest structs]
std::vector<std::vector<poiStruct>> PointsOfInterest (3);

//Vector --> key: Feature Type (e.g. 0 = Unknown, 1 = Park...) value: vector containing feature IDs
std::vector<std::vector<int>> FeatureTypes;

double scale_factor = 1;


//average latitude of map, value set in draw_map_blank_canvas
float latAvg; 
//corners of the map, value set in draw_map
double max_lat;
double min_lat;
double max_lon;
double min_lon;

/************  FUNCTION DECLARATIONS  ***********/
void draw_map_blank_canvas ();
void draw_main_canvas (ezgl::renderer *g);
double lon_from_x (double x);
double lat_from_y (double x);

void populatePointsOfInterest();
void populateWayRoadType();
void populateFeatureTypes();
void populateFeatureCentoids();
void drawFeatures(int feature_type, ezgl::renderer *g);
void draw_intersections();  
int intersectionThresrhold(int interIndex);

void draw_feature_names(ezgl::renderer *g);
ezgl::point2d compute2DPolygonCentroid(std::vector<ezgl::point2d> &vertices, int vertexCount, double area);
ezgl::point2d find_PolyLine_Middle(int featureId);
double feature_max_width (int numPoints, int featureId);


void act_on_mouse_click( ezgl:: application* app, GdkEventButton* event, double x_click, double y_click);
void find_button(GtkWidget *widget, ezgl::application *application);
void initial_setup(ezgl::application *application, bool /*new_window*/);

//void find_map_bounds(double& max_lat, double& min_lat, double& max_lon, double& min_lon);

std::pair < double, double > latLonToCartesian (LatLon latLonPoint);

double y_from_lat (double lat);
double x_from_lon (double lon);

/************************************************/


void draw_map(){

    populateWayRoadType();
    populateFeatureTypes();
    populatePointsOfInterest();
    draw_map_blank_canvas();
}
void draw_map_blank_canvas (){       
    ezgl::application::settings settings;
    settings.main_ui_resource = "libstreetmap/resources/main.ui";
    settings.window_identifier = "MainWindow";
    settings.canvas_identifier = "MainCanvas";

    ezgl::application application(settings);
    
    
   //find the maximum and minimum intersections of the map
    max_lat = getIntersectionPosition(0).lat(); 
    min_lat = max_lat;
    max_lon = getIntersectionPosition(0).lon();
    min_lon = max_lon;
    
    //populate the intersections vector
    int numIntersections = getNumIntersections();
    intersections.resize(numIntersections);
    
    //variable used in (long,lat) -> (x,y) conversion
    double sumLat = 0;
    
    //for-loop which populates the intersections vector and keeps track of the min and max lat/lon positions
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
            
    min_lon = x_from_lon(min_lon);
    min_lat = y_from_lat(min_lat);
    max_lon = x_from_lon(max_lon);
    max_lat = y_from_lat(max_lat);
   
    ezgl::rectangle initial_world({min_lon, min_lat},{max_lon, max_lat}); 
    
    application.add_canvas("MainCanvas", draw_main_canvas, initial_world);
    application.run(initial_setup,act_on_mouse_click,NULL,NULL);
}

void draw_main_canvas (ezgl::renderer *g){
    
    //Determine the amount that screen is zoomed in 
    ezgl::rectangle zoom_rect = g->get_visible_world();
    double zoom = zoom_rect.width(); //width of zoom rectangle, converted into lat lon coordinates
    scale_factor = zoom/(max_lon - min_lon);//percentage of the full map shown in the window
    //Use these for creating thresholds for zooming
//    std::cout<<"\nscale_factor: "<<scale_factor;
//    std::cout<<"\nzoom: "<<zoom;

    //Variables
    float rotationAngle, xMiddleOfSegment, yMiddleOfSegment, segmentLength;
    std::string streetName;
    
    //Drawing Backgrounds
    //***********************************************************************************
//    g->draw_rectangle({min_lon, min_lat},{max_lon, max_lat});
//    g->set_color (225, 230, 234, 255);
//    g->fill_rectangle({min_lon,min_lat}, {max_lon, max_lat});
//    
   //Drawing Features
    //***********************************************************************************
    
    //Draws features based on increasing order importance of feature types
    
    //  Lake (Least important, drawn first)
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
    drawFeatures(Lake, g);
    drawFeatures(Island, g);
    drawFeatures(Park, g);
    drawFeatures(Greenspace, g);
    drawFeatures(Beach, g);
    drawFeatures(Golfcourse, g);
    drawFeatures(Building, g);
    drawFeatures(River, g);
    drawFeatures(Stream, g);
    drawFeatures(Unknown, g);
    
    //Drawing Streets
     //***********************************************************************************
    
    for (int streetIdx = 0; streetIdx < StreetVector.size(); streetIdx++ ){ //for each street
        std::vector<int> segments = StreetVector[streetIdx].streetSegments;
        streetName = getStreetName(streetIdx);
        
        for (int i = 0; i < segments.size(); i++ ){
            bool enableDraw = true;
            int segmentID = segments[i];
            struct InfoStreetSegment segmentInfo = getInfoStreetSegment(segmentID);
            int numCurvePoints = segmentInfo.curvePointCount;
            segmentLength = find_street_segment_length(segmentID);
            std::string roadType = WayRoadType.at(segmentInfo.wayOSMID);
            
            //scale_factor used to set a variety of line widths and displays of roads    
            
            if(roadType=="motorway"){
                g->set_line_width (10);
                if (scale_factor >  0.3)
                    g->set_line_width (8);
                g->set_color (232, 144, 160, 255);
                g->set_line_dash(ezgl::line_dash::none);
            }
            else if(roadType=="trunk"){
                g->set_color (250, 178, 154, 255);
                g->set_line_width (16);
                if (scale_factor >  0.18)
                    g->set_line_width (10);
                if (scale_factor >  0.3)
                    g->set_line_width (7);
                g->set_line_dash(ezgl::line_dash::none);
            }
            else if(roadType=="primary"){
                g->set_color (252, 215, 162, 255);
                g->set_line_width (16);
                if (scale_factor >  0.18)
                    g->set_line_width (10);
                if (scale_factor >  0.3)
                    g->set_line_width (8);
                        
                g->set_line_dash(ezgl::line_dash::none);
            }
            else if(roadType=="secondary"){
                g->set_color (246, 251, 187, 255);
                g->set_line_width (12);
                if (scale_factor >  0.18)
                    g->set_line_width (8);
                if (scale_factor >  0.3)
                    g->set_line_width (4);
                g->set_line_dash(ezgl::line_dash::none);
            }
            else if(roadType=="tertiary"){
                if (scale_factor > 0.30)
                    enableDraw = false;
                g->set_color (255, 255, 255, 255);
                g->set_line_width (8);
                if (scale_factor >  0.18)
                    g->set_line_width (4);
                g->set_line_dash(ezgl::line_dash::none);
            }
            else if(roadType=="residential"){
                if (scale_factor > 0.05)
                    enableDraw = false;
                g->set_color (255, 255, 255, 255);
                g->set_line_width (5);
                g->set_line_dash(ezgl::line_dash::none);
            }
            else if(roadType=="unclassified"){
                if (scale_factor > 0.05)
                    enableDraw = false;
                g->set_color (255, 255, 255, 255);
                g->set_line_width (5);
                g->set_line_dash(ezgl::line_dash::none);
            }
            else{
                if (scale_factor > 0.30)
                    enableDraw = false;
                g->set_line_width (8);
                if (scale_factor >  0.18)
                    g->set_line_width (4);
                g->set_color (255, 255, 255, 255);
                g->set_line_dash(ezgl::line_dash::none);
            }     
            
            if (numCurvePoints==0){ //if segment is a straight line
                int fromIntersection = segmentInfo.from; 
                int toIntersection = segmentInfo.to; 
                
                std::pair <double, double> xyFrom = latLonToCartesian(intersections[fromIntersection].position);
                std::pair <double, double> xyTo = latLonToCartesian(intersections[toIntersection].position);
                
                if (enableDraw){ //if the line should be drawn
                    g->draw_line({xyFrom.first, xyFrom.second}, {xyTo.first, xyTo.second});
                    
                    if(streetName!="<unknown>"){ //awkward "<unknown>" street name not drawn
                    
                        rotationAngle = atan2(xyFrom.second - xyTo.second, xyFrom.first - xyTo.first)/DEGREE_TO_RADIAN;
                        xMiddleOfSegment = 0.5*(xyFrom.first + xyTo.first);
                        yMiddleOfSegment = 0.5*(xyFrom.second + xyTo.second);

                        if (rotationAngle > 90 ){
                            rotationAngle = rotationAngle - 180;
                        }
                        if (rotationAngle < -90){
                            rotationAngle = rotationAngle + 180;
                        }
                        //draw text
                        g->set_color (0, 0, 0, 255);   
                        g->set_text_rotation(rotationAngle);

                        std::string direction_symbol = ">"; //symbol for one way street
                        if (segmentInfo.oneWay){
                            if (xyFrom.first > xyTo.first){
                                direction_symbol = "<"; //reverse direction
                            }
                        }
                        double segment_length = SegmentLengths[segmentID]; 
                        double screen_ratio = segment_length/scale_factor; //screen_ratio is the available length of the street segment on screen. 
                        //Greater screen_ratio = more reprinting of street name and arrows: < (for oneway streets)
                        
                        //only enough space to draw direction
                        if (screen_ratio < 5000){
                            if (segmentInfo.oneWay){
                                g->draw_text({ xMiddleOfSegment, yMiddleOfSegment}, direction_symbol, segmentLength, segmentLength);            
                            }
                        }
                        
                        //only enough space to draw name
                        else if (screen_ratio < 20000){
                            g->draw_text({ xMiddleOfSegment, yMiddleOfSegment}, streetName, segmentLength, segmentLength);            
                        }
                        
                        //draw:  [direction] [name] [direction]
                        else if (screen_ratio < 30000){
                            g->draw_text({ xMiddleOfSegment, yMiddleOfSegment}, streetName, segmentLength, segmentLength);            
                            if (segmentInfo.oneWay){
                                g->draw_text({(xMiddleOfSegment+xyFrom.first)/2, (yMiddleOfSegment+xyFrom.second)/2}, direction_symbol, segmentLength, segmentLength);
                                g->draw_text({(xMiddleOfSegment+xyTo.first)/2, (yMiddleOfSegment+xyTo.second)/2}, direction_symbol, segmentLength, segmentLength);
                            }
                        }
                        
                        //draw:  [direction] [name] [direction] [name] [direction] 
                        else if (screen_ratio < 50000){
                            g->draw_text({ (2*xMiddleOfSegment+xyFrom.first)/3, (2*yMiddleOfSegment+xyFrom.second)/3}, streetName, segmentLength, segmentLength); 
                            g->draw_text({ (2*xMiddleOfSegment+xyTo.first)/3, (2*yMiddleOfSegment+xyTo.second)/3}, streetName, segmentLength, segmentLength); 

                            if (segmentInfo.oneWay){
                                g->draw_text({ xMiddleOfSegment, yMiddleOfSegment}, direction_symbol, segmentLength, segmentLength);         
                                g->draw_text({ (((2*xMiddleOfSegment+xyFrom.first)/3)+xyFrom.first)/2, (((2*yMiddleOfSegment+xyFrom.second)/3)+xyFrom.second)/2}, direction_symbol, segmentLength, segmentLength); 
                                g->draw_text({ (((2*xMiddleOfSegment+xyTo.first)/3)+xyTo.first)/2, (((2*yMiddleOfSegment+xyTo.second)/3)+xyTo.second)/2}, direction_symbol, segmentLength, segmentLength); 

                            }
                        }
                        
                         //draw:  [direction] [name] [direction] [name] [direction] [name] [direction] 
                        else{
                            g->draw_text({ xMiddleOfSegment, yMiddleOfSegment}, streetName, segmentLength, segmentLength);            
                            g->draw_text({(xMiddleOfSegment+xyFrom.first)/2, (yMiddleOfSegment+xyFrom.second)/2}, streetName, segmentLength, segmentLength);
                            g->draw_text({(xMiddleOfSegment+xyTo.first)/2, (yMiddleOfSegment+xyTo.second)/2}, streetName, segmentLength, segmentLength);

                            if (segmentInfo.oneWay){
                              g->draw_text({(((xMiddleOfSegment+xyFrom.first)/2)+xyFrom.first)/2, (((yMiddleOfSegment+xyFrom.second)/2)+xyFrom.second)/2}, direction_symbol, segmentLength, segmentLength);         
                              g->draw_text({(((xMiddleOfSegment+xyFrom.first)/2)+xMiddleOfSegment)/2, (((yMiddleOfSegment+xyFrom.second)/2)+yMiddleOfSegment)/2}, direction_symbol, segmentLength, segmentLength);         
                              g->draw_text({(((xMiddleOfSegment+xyTo.first)/2)+xMiddleOfSegment)/2, (((yMiddleOfSegment+xyTo.second)/2)+yMiddleOfSegment)/2}, direction_symbol, segmentLength, segmentLength);         
                              g->draw_text({(((xMiddleOfSegment+xyTo.first)/2)+xyTo.first)/2, (((yMiddleOfSegment+xyTo.second)/2)+xyTo.second)/2}, direction_symbol, segmentLength, segmentLength);         

                            }
                        }
                    }
                }
                g->set_text_rotation(0);
            }
            else{//segment is curved
                if(enableDraw){
                    
                    //first deal with all curves from segment's "from" intersection to the last curve point

                    //first curve of the segment
                    LatLon pointsLeft  = getIntersectionPosition(segmentInfo.from);
                    LatLon pointsRight = getStreetSegmentCurvePoint(0, segmentID);

                    std::pair <double, double> xyLeft = latLonToCartesian(pointsLeft);
                    std::pair <double, double> xyRight = latLonToCartesian(pointsRight);

                    g->draw_line({xyLeft.first, xyLeft.second}, {xyRight.first, xyRight.second});

                    for (int curvePointIndex = 0; curvePointIndex < numCurvePoints - 1; curvePointIndex++){
                        pointsLeft = pointsRight;
                        pointsRight = getStreetSegmentCurvePoint(curvePointIndex + 1, segmentID);

                        xyLeft = latLonToCartesian(pointsLeft);
                        xyRight = latLonToCartesian(pointsRight);

                        g->draw_line({xyLeft.first, xyLeft.second}, {xyRight.first, xyRight.second});
                    }

                    //then, deal with the last curve point to the segment's "to" intersection
                    pointsLeft = pointsRight;
                    pointsRight = getIntersectionPosition(segmentInfo.to);

                    //update segment length to length between curve points
                    std::pair <LatLon, LatLon> points (pointsLeft, pointsRight);
                    segmentLength = find_distance_between_two_points(points);

                    xyLeft = latLonToCartesian(pointsLeft);
                    xyRight = latLonToCartesian(pointsRight);                
                        g->draw_line({xyLeft.first, xyLeft.second}, {xyRight.first, xyRight.second});
                }
            }
        }
    }     

    draw_feature_names(g);
     
    //Draw POIs
    //***********************************************************************************
    
    bool enable_poi = true;
    if (scale_factor > 0.5)
        enable_poi = false;
    
    //Extract vectors from PointsOfInterest vector to make it easier to parse through each type separately
    std::vector<poiStruct> police = PointsOfInterest[0];
    std::vector<poiStruct> hospitals = PointsOfInterest[1];
    std::vector<poiStruct> fire_station = PointsOfInterest[2];
    
    //Declare iterator to go through each vector
    std::vector<poiStruct>::iterator it = police.begin();
    
    //Variables to extract data from poi struct
    poiStruct poiData;
    std::pair<double,double> xyCoordinates;
    std::string poiName;
    
    //loop through police vector, extract data and draw names of police stations on map
    while(it != police.end()){
        poiData = *it;
        xyCoordinates = poiData.xyCoordinates;
        poiName = poiData.Name;
        
        g->set_color (ezgl::BLACK);
        if (enable_poi)
            g->draw_text({xyCoordinates.first, xyCoordinates.second}, poiName);
        
        it++;
    }
    
    //loop through hospital vector, extract data and draw names of police stations on map
    it = hospitals.begin();
    while(it != hospitals.end()){
        poiData = *it;
        xyCoordinates = poiData.xyCoordinates;
        poiName = poiData.Name;
        
        g->set_color (ezgl::BLACK);
        if (enable_poi)
            g->draw_text({xyCoordinates.first, xyCoordinates.second}, poiName, 10, 10);
        
        it++;
    }
    
    //loop through fire_station vector, extract data and draw names of police stations on map
    it = fire_station.begin();
    while(it != fire_station.end()){
        poiData = *it;
        xyCoordinates = poiData.xyCoordinates;
        poiName = poiData.Name;
        
        g->set_color (ezgl::BLACK);
        if (enable_poi)
            g->draw_text({xyCoordinates.first, xyCoordinates.second}, poiName, 10, 10);
        
        it++;
    }
    
     std::cout<<scale_factor<<"<-s f\n";
    //Drawing Intersections
    //***********************************************************************************
    
    for(size_t i = 0; i < intersections.size(); ++i){
      bool enableDraw = false;

      double x = intersections[i].position.lon();
      double y = intersections[i].position.lat();

      //must convert lat lon values to cartesian (refer to tutorial slides)
      x = x_from_lon(x);
      y = y_from_lat(y);
     
      int threshold = intersectionThresrhold(i);
      if (threshold==0||(threshold==1&&scale_factor <= 0.30)||(scale_factor <=0.05) )
          enableDraw = true;
      else 
          enableDraw = false;
      
      float width;
      if (scale_factor > 0.30)
          width =  10;
      else if (scale_factor > 0.20)
          width =  6;
      else if (scale_factor > 0.10)
          width =  4;
      else //if (scale_factor < 0.10)
          width =  2;
      
      float height = width;
      
      if (intersections[i].highlight)
          g->set_color(ezgl::RED);
      else
          g->set_color(ezgl::BLUE);
      
      //for intersection id, get segs. Check seg's lowest threshold value
      //for that threshold value, enable or disable drawing
      if (enableDraw)
        g->fill_rectangle({x-(width/2),y-(height/2)}, {x + (width/2), y + (height/2)});
    
    }

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
            x = 2449241 + x_from_lon (latlon.lon());
            y = y_from_lat (latlon.lat());
            xy = std::make_pair(x,y);
            poiData.addName(name);
            poiData.addXYCoordinates(xy);
            
            police.push_back(poiData);
        }
        else if (value == "hospital"){
            name = getPointOfInterestName(poiIterator);
            latlon = getPointOfInterestPosition(poiIterator);
            
            x = 2449241 + x_from_lon (latlon.lon());
            y = y_from_lat (latlon.lat());
            xy = std::make_pair(x,y);
            
            poiData.addName(name);
            poiData.addXYCoordinates(xy);
            
            hospital.push_back(poiData);
        }
        else if (value == "fire_station"){
            name = getPointOfInterestName(poiIterator);
            latlon = getPointOfInterestPosition(poiIterator);
            
            x = 2449241 + x_from_lon (latlon.lon());
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

void populateWayRoadType(){
            
    //Retrieves OSMNodes and calculate total distance, for each way
    for (unsigned i = 0; i < getNumberOfWays(); i++){
        //creates a pointer that enables accessing the node's OSMID
        const OSMWay* wayPtr = getWayByIndex(i);
        std::string key,value;
        key="N/A"; //in case the way has no keys
        for(int j=0;j<getTagCount(wayPtr); ++j){
            std::tie(key,value) = getTagPair(wayPtr,j);
            if (key=="highway")
                break;
        }
        if (key=="highway"){ //if highway key exists
            WayRoadType.insert({wayPtr->id(),value});
        }
        
    }
    
    for (unsigned i = 0; i < getNumberOfRelations(); i++){
        //creates a pointer that enables accessing the node's OSMID
        const OSMRelation* relationPtr = getRelationByIndex(i);
        std::string key,value;
        key="N/A"; //in case the way has no keys
        for(int j=0;j<getTagCount(relationPtr); ++j){
            std::tie(key,value) = getTagPair(relationPtr,j);
            if (key=="highway")
                break;
        }
        if (key=="highway"){ //if highway key exists
            WayRoadType.insert({relationPtr->id(),value});
        }
        
    }
    
    for (unsigned i = 0; i < getNumberOfNodes(); i++){
        //creates a pointer that enables accessing the node's OSMID
        const OSMNode* nodePtr = getNodeByIndex(i);
        std::string key,value;
        key="N/A"; //in case the way has no keys
        for(int j=0;j<getTagCount(nodePtr); ++j){
            std::tie(key,value) = getTagPair(nodePtr,j);
            if (key=="highway")
                break;
        }
        if (key=="highway"){ //if highway key exists
            WayRoadType.insert({nodePtr->id(),value});
        }
        
    }  
}

void populateFeatureTypes(){
    FeatureTypes.resize(10);
        for(size_t featureId = 0; featureId < getNumFeatures(); ++featureId){

            FeatureType feature_type = getFeatureType(featureId);
            if (feature_type>9||feature_type<0)
                std::cout<<"Error: Invalid Feature Type detected\n";
            else{
                FeatureTypes[feature_type].push_back(featureId);
            }
        }  
}

    //Draws features of a specifc type (e.g. all Beaches)
void drawFeatures(int feature_type, ezgl::renderer *g){        
    for(size_t idx = 0; idx < FeatureTypes[feature_type].size(); ++idx){
        int featureId = FeatureTypes[feature_type][idx];
        switch(feature_type){
            case Unknown: g->set_color (224,224,224, 255);
                                 break;
            case     Park      : g->set_color (204,255,204, 255);
                                 break;
            case     Beach     : g->set_color (245,230,194, 255);
                                 break;       
            case     Lake      : g->set_color (204,255,255, 255);
                                 break;
            case     River     : g->set_color (177,229,229, 255);
                                 break;
            case     Island    : g->set_color (128,243,151, 255);
                                 break;
            case     Building  : g->set_color (220,220,220, 255);
                                 break;
            case     Greenspace: g->set_color (128,243,151, 255);
                                 break;
            case     Golfcourse: g->set_color (0,255,128, 255);
                                 break;
            case     Stream    : g->set_color (177,229,229, 255);
                                 break;
            
            default: g->set_color (224,224,224, 255);
        }
        
        //get all of the points for that particular feature
        int numOfFeaturePoints = getFeaturePointCount(featureId);

        // If the first point and the last point (getFeaturePointCount-1) are NOT the same location, the feature is a polyline
        if (FeatureAreaVector[featureId] == 0) {
//            
//            //iterate through feature points to get (x,y) coordinates  in order to draw the polyline)
//            for (unsigned featurePointId = 1; featurePointId < numOfFeaturePoints; featurePointId++){
//                //Declare and initialize adjacent LatLon points
//                LatLon previousPoint = getFeaturePoint(featurePointId-1, featureId);
//                LatLon nextPoint = getFeaturePoint(featurePointId, featureId);
//            
//                //convert LatLon points into x y coordinates
//                std::pair<double,double> xyPrevious = latLonToCartesian(previousPoint);
//                std::pair<double,double> xyNext = latLonToCartesian(nextPoint);
//
//                //draw line between feature points
//                g->set_line_width(3);
//                g->draw_line({xyPrevious.first , xyPrevious.second}, {xyNext.first, xyNext.second});
//            }
                  
              //insert feature centre point into FeatureCentroids hashtable
              FeatureCentroids.insert({featureId , find_PolyLine_Middle(featureId)});
            
        }
        //otherwise, it is a polygon area
        else{
            std::vector<ezgl::point2d> points_xy_coordinates;
            
            //iterate through feature points to get (x,y) coordinates  in order to draw the polygon
            for (unsigned featurePointId = 0; featurePointId < numOfFeaturePoints; featurePointId++){

                //convert LatLon points into (x, y) coordinate pairs
                std::pair <double,double> featurePoints = latLonToCartesian(getFeaturePoint(featurePointId, featureId));

                points_xy_coordinates.push_back(ezgl::point2d(featurePoints.first, featurePoints.second));
                
                //insert feature centre point into FeatureCentroids hashtable       
                FeatureCentroids.insert({featureId , compute2DPolygonCentroid(points_xy_coordinates, numOfFeaturePoints, FeatureAreaVector[featureId])});
                
            }
            
            //ezgl function which takes a vector<point2d> (i.e (x,y) coordinates for each point that defines the feature outline)
            g->fill_poly(points_xy_coordinates);
            //draw feature name in centre of polygon
            //g->set_color (0, 0, 0, 255);   
//            g->set_color (ezgl:: BLACK);   
//            
//            g->set_text_rotation(0);

//            
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
    
}

void act_on_mouse_click( ezgl:: application* app, GdkEventButton* event, double x_click, double y_click){
    //x_click and y_click are the world coordinates where the mouse was clicked
    //will convert to latlon then use find_closest_intersection
    std::cout << "x: " << x_click << "y: " << y_click << std::endl;
    LatLon lat_lon_click = LatLon(lat_from_y (y_click), lon_from_x (x_click));
    
    int closestInt_id = find_closest_intersection(lat_lon_click);
    
    intersections[closestInt_id].highlight = true;
    
    app->update_message (getIntersectionName(closestInt_id));
    
    app->refresh_drawing();
            
}

void initial_setup(ezgl::application *application, bool new_window)
{

  //Create a Find button and link it with find_button callback function.
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
    //std::cout << street1 << "\n";
    std::getline(iss, street2);
    std::cout<<"street2 is: "<<street2;
    street2 = street2.substr(street2.find("and") + 4); 
//    if (street2.find(" & ")< street2.length()){ //found an & in the string
//        iss >> street2;  
//        std::getline(iss, street2);
//    }
//    else{
//         street2 = street2.substr(street2.find("and") + 4); //
//    }
    //std::cout << street2 << "\n";

    
    //obtains all of the possible streetIds that match the entered street names
    std::vector<int> street_ids_1 = find_street_ids_from_partial_street_name(street1);
    std::vector<int> street_ids_2 = find_street_ids_from_partial_street_name(street2);
    
    int street1_remaining_id = 0;
    int street2_remaining_id = 0;
    //nested for-loop which finds intersections between all streetId's returned by partial_street_name function
    //outer for-loop looks through all matches in 

    for(int i = 0; i < street_ids_1.size() && (streetIntersections.empty() == true); i++){
        for(int j = 0; j < street_ids_2.size() && (streetIntersections.empty() == true) ; j++){
            
            //load twoStreets pair
            twoStreets.first = street_ids_1[i];

            twoStreets.second = street_ids_2[j];

            streetIntersections = find_intersections_of_two_streets(twoStreets);
            street2_remaining_id++;
        }
        
        street1_remaining_id++; //holds record of the index
    }
    //an un empty streetIntersections vector indicates that common intersections were found for the predicted streets
    
    //std::pair<int, int> twoStreets(street_ids_1[0], street_ids_2[0]);
    
    //std::vector<int> streetIntersections = find_intersections_of_two_streets(twoStreets);
    
    std::string intersectionNames = "";
        
    for(int i = 0; i < streetIntersections.size(); i++){
        intersections[streetIntersections[i]].highlight = true;
        intersectionNames+=getIntersectionName(streetIntersections[i]);
        if (i+1!=streetIntersections.size())
            intersectionNames+=", ";
    //    std::cout << intersections[streetIntersections[i]].name << "\n";
    }
    
    //Suggested Street names'  
    std::string suggested_streets = "";
    
    for(int i = street1_remaining_id; i < street_ids_1.size(); i++){
        for(int j = street2_remaining_id; j < street_ids_2.size(); j++){

            //load twoStreets pair
            twoStreets.first = street_ids_1[i];

            twoStreets.second = street_ids_2[j];
            streetIntersections = find_intersections_of_two_streets(twoStreets);
            if (streetIntersections.empty()==false){
                
                for(int k = 0; k < streetIntersections.size(); k++){
                    suggested_streets+=getStreetName(twoStreets.first)+" & "+getStreetName(twoStreets.second)+"\n";
                }
            }
        }
    }
    
    if (suggested_streets!=""){
        std::cout<<"\nDid you mean?\n\n";
        std::cout<<suggested_streets;
    }
    
    application->update_message (intersectionNames); 
    // Redraw the graphics
    application->refresh_drawing();
}

//Based on the segments touching the street, returns the highest zoom threshold value (0 = highest threshold, 1 = medium threshold, 2 = lowest threshold) 
int intersectionThresrhold(int interIndex){
    int threshold=2;
    for (int i = 0; i < IntersectionStreetSegments[interIndex].size(); i++){
        InfoStreetSegment segInfo = getInfoStreetSegment(IntersectionStreetSegments[interIndex][i]);
        std::string roadType = WayRoadType.at(segInfo.wayOSMID);
        if (roadType =="motorway"||roadType =="trunk"||roadType =="primary"||roadType =="secondary") //significant roads always show on map
            return 1;
        else if (roadType!="residential"||roadType!="unclassified"){ //either tertiary or unknown 
            if (threshold==2)
                threshold = 1; //most significant is tertiary/unknown
        }   
    }
    return threshold;
}
//vertices are the feature points in xy coordinates, vertexCount = number of feature points
ezgl::point2d compute2DPolygonCentroid(std::vector< ezgl::point2d > & vertices, int vertexCount, double area)
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
  
    //draw feature name in centre of polygon
    //g->set_color (0, 0, 0, 255);   
    g->set_color (ezgl:: BLACK);   

    g->set_text_rotation(0);

    //std::cout << "max_width " << max_width << "\n";

    //std::cout << feature_centroid.x << feature_centroid.y << "\n";

    for( int featureidx = 0;  featureidx < getNumFeatures(); featureidx++){
                
        if( getFeatureName(featureidx) != "<noname>" && (scale_factor < 0.10))
            g->draw_text(FeatureCentroids.at(featureidx), getFeatureName(featureidx));

        //float width = max_width;

        //g->fill_rectangle({feature_centroid.x-(width/2),feature_centroid.y-(width/2)}, {feature_centroid.x + (width/2), feature_centroid.y + (width/2)});
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
