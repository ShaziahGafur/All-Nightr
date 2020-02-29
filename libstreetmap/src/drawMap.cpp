#include "m1.h"
#include "globals.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "m2.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "ezgl/point.hpp"
#include "intersection_data.h"
#include <vector>
#include <string>
#include <cmath>


/************  GLOBAL VARIABLES  *****************/
//Vector --> key: [intersection ID] value: [intersection_data struct]
std::vector<intersection_data> intersections;

//Hashtable --> key: [OSMway] value: [road type]
std::unordered_map<OSMID, std::string> WayRoadType;

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

void populateWayRoadType();
void draw_intersections();  

void act_on_mouse_click( ezgl:: application* app, GdkEventButton* event, double x_click, double y_click);

//void find_map_bounds(double& max_lat, double& min_lat, double& max_lon, double& min_lon);

std::pair < double, double > latLonToCartesian (LatLon latLonPoint);

double y_from_lat (double lat);
double x_from_lon (double lon);

/************************************************/


void draw_map(){
    populateWayRoadType();
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
   
    ezgl::rectangle initial_world({min_lon, min_lat},{max_lon, max_lat}); //keep this initial_world version (refer to tutorial slides)
    //  ezgl::rectangle initial_world({min_lon, min_lat},{max_lon, max_lat});
    
    application.add_canvas("MainCanvas", draw_main_canvas, initial_world);
    application.run(NULL,act_on_mouse_click,NULL,NULL);
}

void draw_main_canvas (ezgl::renderer *g){
  /*  
    //Variables
    float segmentSlope, rotationAngle, xMiddleOfSegment, yMiddleOfSegment, segmentLength;
    std::string streetName;
    //Drawing Backgrounds
    //***********************************************************************************
    g->draw_rectangle({min_lon, min_lat},{max_lon, max_lat});
    g->set_color (225, 230, 234, 255);
    g->fill_rectangle({min_lon,min_lat}, {max_lon, max_lat});
    
   //Drawing Features
    //***********************************************************************************
    g->set_line_dash(ezgl::line_dash::none);
    
    //variables needed for drawing features
//    double previousPoint_x, previousPoint_y, point_x, point_y;
    
    for(size_t featureId = 0; featureId < getNumFeatures(); ++featureId){
        //get feature type to determine colour
        FeatureType feature_type = getFeatureType(featureId);
        switch(feature_type)
        {
            case     Unknown   : g->set_color (224,224,224, 255);
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
            
            //iterate through feature points to get (x,y) coordinates  in order to draw the polyline)
            for (unsigned featurePointId = 1; featurePointId < numOfFeaturePoints; featurePointId++){
                //Declare and initialize adjacent LatLon points
                LatLon previousPoint = getFeaturePoint(featurePointId-1, featureId);
                LatLon nextPoint = getFeaturePoint(featurePointId, featureId);
            
                //convert LatLon points into x y coordinates
                std::pair<double,double> xyPrevious = latLonToCartesian(previousPoint);
                std::pair<double,double> xyNext = latLonToCartesian(nextPoint);

                //draw line between feature points
                g->set_line_width(3);
                g->draw_line({xyPrevious.first , xyPrevious.second}, {xyNext.first, xyNext.second});
            }
        }
        else{
            std::vector<ezgl::point2d> points_xy_coordinates;
            
            //iterate through feature points to get (x,y) coordinates  in order to draw the polygon
            for (unsigned featurePointId = 0; featurePointId < numOfFeaturePoints; featurePointId++){

                //convert LatLon points into (x, y) coordinate pairs
                std::pair <double,double> featurePoints = latLonToCartesian(getFeaturePoint(featurePointId, featureId));

                points_xy_coordinates.push_back(ezgl::point2d(featurePoints.first, featurePoints.second));
            }
            
       //     ezgl function which takes a vector<point2d> (i.e (x,y) coordinates for each point that defines the feature outline)
            g->fill_poly(points_xy_coordinates);
        }
    }
    
    //Drawing Streets
     //***********************************************************************************
    
    
    for (int streetIdx = 0; streetIdx < StreetVector.size(); streetIdx++ ){ //for each street
        std::vector<int> segments = StreetVector[streetIdx].streetSegments;
        streetName = getStreetName(streetIdx);
        
        for (int i = 0; i < segments.size(); i++ ){
            int segmentID = segments[i];
            struct InfoStreetSegment segmentInfo = getInfoStreetSegment(segmentID);
            int numCurvePoints = segmentInfo.curvePointCount;
            segmentLength = find_street_segment_length(segmentID);
            std::string roadType = WayRoadType.at(segmentInfo.wayOSMID);
                        
            if(roadType=="motorway"){
                g->set_color (232, 144, 160, 255);
                g->set_line_width (20);
                g->set_line_dash(ezgl::line_dash::none);
            }
            else if(roadType=="trunk"){
                g->set_color (250, 178, 154, 255);
                g->set_line_width (18);
                g->set_line_dash(ezgl::line_dash::none);
            }
            else if(roadType=="primary"){
                g->set_color (252, 215, 162, 255);
                g->set_line_width (16);
                g->set_line_dash(ezgl::line_dash::none);
            }
            else if(roadType=="secondary"){
                g->set_color (246, 251, 187, 255);
                g->set_line_width (12);
                g->set_line_dash(ezgl::line_dash::none);
            }
            else if(roadType=="tertiary"){
                g->set_color (255, 255, 255, 255);
                g->set_line_width (10);
                g->set_line_dash(ezgl::line_dash::none);
            }
            else if(roadType=="residential"){
                g->set_color (255, 255, 255, 255);
                g->set_line_width (5);
                g->set_line_dash(ezgl::line_dash::none);
            }
            else if(roadType=="unclassified"){
                g->set_color (255, 255, 255, 255);
                g->set_line_width (5);
                g->set_line_dash(ezgl::line_dash::none);
            }
            else{
                g->set_line_width (5);
                g->set_color (255, 255, 255, 255);
                g->set_line_dash(ezgl::line_dash::none);
            }     
            
            if (numCurvePoints==0){ //if segment is a straight line
                int fromIntersection = segmentInfo.from; 
                int toIntersection = segmentInfo.to; 
                
                std::pair <double, double> xyFrom = latLonToCartesian(intersections[fromIntersection].position);
                std::pair <double, double> xyTo = latLonToCartesian(intersections[toIntersection].position);

                g->draw_line({xyFrom.first, xyFrom.second}, {xyTo.first, xyTo.second});
                
                //find slope of the segment
                segmentSlope = (xyTo.second - xyFrom.second)/(xyTo.first - xyFrom.first);
                rotationAngle = atan(segmentSlope)/DEGREE_TO_RADIAN;
                xMiddleOfSegment = 0.5*(xyFrom.first + xyTo.first);
                yMiddleOfSegment = 0.5*(xyFrom.second + xyTo.second);
                        
                //draw text
                g->set_color (0, 0, 0, 255);   
                g->draw_text({ xMiddleOfSegment, yMiddleOfSegment}, streetName, segmentLength, segmentLength);
                g->set_text_rotation(rotationAngle);
            }
            else{//segment is curved
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
                
                xyLeft = latLonToCartesian(pointsLeft);
                xyRight = latLonToCartesian(pointsRight);
                     
                g->draw_line({xyLeft.first, xyLeft.second}, {xyRight.first, xyRight.second});
            }
        }
    }  
    */
       //Drawing Intersections
    //***********************************************************************************
    
    //Drawing
    for(size_t i = 0; i < intersections.size(); ++i){

      double x = intersections[i].position.lon();
      double y = intersections[i].position.lat();

      //must convert lat lon values to cartesian (refer to tutorial slides)
      x = x_from_lon(x);
      y = y_from_lat(y);
     
      float width = 10;
      float height = width;
      
       if (intersections[i].highlight)
          g->set_color(ezgl::RED);
      else
          g->set_color(ezgl::BLUE);

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

void act_on_mouse_click( ezgl:: application* app, GdkEventButton* event, double x_click, double y_click){
    //x_click and y_click are the world coordinates where the mouse was clicked
    //will convert to latlon then use find_closest_intersection
    
    LatLon lat_lon_click = LatLon(lat_from_y (y_click), lon_from_x (x_click));
    
    std::cout << lat_lon_click;
    
    int closestInt_id = find_closest_intersection(lat_lon_click);
    
    std::cout << "Clicked on intersection id\t" << closestInt_id << getIntersectionName(closestInt_id) << std::endl;
    
    intersections[closestInt_id].highlight = true;
    //ezgl::application->update_message (“my message”)
    
    app->refresh_drawing();
            
}