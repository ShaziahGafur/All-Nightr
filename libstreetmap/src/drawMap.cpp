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
//corners of the map, value set in draw_map())
double max_lat;
double min_lat;
double max_lon;
double min_lon;

/************  FUNCTION DECLARATIONS  ***********/
void draw_map_blank_canvas ();
void draw_main_canvas (ezgl::renderer *g);

void populateWayRoadType();
void draw_intersections();  

//void find_map_bounds(double& max_lat, double& min_lat, double& max_lon, double& min_lon);

std::pair < double, double > latLonToCartesian (double lat, double lon);

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
            
    //convert latlon points to cartesian points
    //declare pair of min and Max cartesian coordinates and assign them to return pair of latlon to cartesian helper function
    
//    std::pair < double, double > minCartesian = latLonToCartesian (min_lat, min_lon);
//    std::pair < double, double > maxCartesian = latLonToCartesian (max_lat, max_lon);
    //ezgl::rectangle initial_world({minCartesian.first, minCartesian.second},{maxCartesian.first, maxCartesian.second});
    
    min_lon
   
    ezgl::rectangle initial_world({min_lon, min_lat},{max_lon, max_lat}); //keep this initial_world version (refer to tutorial slides)
    //  ezgl::rectangle initial_world({min_lon, min_lat},{max_lon, max_lat});
    
    application.add_canvas("MainCanvas", draw_main_canvas, initial_world);
    application.run(NULL,NULL,NULL,NULL);
}

void draw_main_canvas (ezgl::renderer *g){
//    g->set_line_dash (ezgl::line_dash::asymmetric_5_3);
    //Variables
    float segmentSlope, rotationAngle, xMiddleOfSegment, yMiddleOfSegment, segmentLength;
    std::string streetName;
    //Drawing Backgrounds
    //***********************************************************************************
    g->draw_rectangle({min_lon, min_lat},{max_lon, max_lat});
    g->set_color (225, 230, 234, 255);
    g->fill_rectangle({min_lon,min_lat}, {max_lon, max_lat});
    
    
    //Drawing Streets
     //***********************************************************************************
    
    g->set_line_width (10);   // 3 pixels wide
    g->set_color (255, 255, 255, 255); 
    g->set_line_dash(ezgl::line_dash::none);
    
    for (int streetIdx = 0; streetIdx < StreetVector.size(); streetIdx++ ){ //for each street
        std::vector<int> segments = StreetVector[streetIdx].streetSegments;
        streetName = getStreetName(streetIdx);
        
        for (int i = 0; i < segments.size(); i++ ){
            int segmentID = segments[i];
            struct InfoStreetSegment segmentInfo = getInfoStreetSegment(segmentID);
            int numCurvePoints = segmentInfo.curvePointCount;
            segmentLength = find_street_segment_length(segmentID);
            
            if (numCurvePoints==0){
                int fromIntersection = segmentInfo.from; 
                int toIntersection = segmentInfo.to; 
                
                float xF = intersections[fromIntersection].position.lon();
                float yF = intersections[fromIntersection].position.lat();
                
                float xT = intersections[toIntersection].position.lon();
                float yT = intersections[toIntersection].position.lat();
                g->set_color (255, 255, 255, 255);
                g->draw_line({xF, yF}, {xT, yT});
                
                //find slope of the segment
                segmentSlope = (yT - yF)/(xT - xF);
                rotationAngle = atan(segmentSlope);
                xMiddleOfSegment = 0.5*(xF + xT);
                yMiddleOfSegment = 0.5*(yF + yT);
                        
                //draw text
                g->set_color (0, 0, 0, 255);   
                g->draw_text({ xMiddleOfSegment, yMiddleOfSegment}, streetName, segmentLength, segmentLength);
                g->set_text_rotation(rotationAngle);
            }
            else{
                //first deal with all curves from segment's "from" intersection to the last curve point
                
                //first curve of the segment
                LatLon pointsLeft  = getIntersectionPosition(segmentInfo.from);
                LatLon pointsRight = getStreetSegmentCurvePoint(0, segmentID);
                
                float xL = pointsLeft.lon();
                float yL = pointsLeft.lat();
                
                float xR = pointsRight.lon();
                float yR = pointsRight.lat();
                
                g->set_color (255, 255, 255, 255); 
                g->draw_line({xL, yL}, {xR, yR});
////                
                for (int curvePointIndex = 0; curvePointIndex < numCurvePoints - 1; curvePointIndex++){
                    pointsLeft = pointsRight;
                    pointsRight = getStreetSegmentCurvePoint(curvePointIndex + 1, segmentID);
                    xL = pointsLeft.lon();
                    yL = pointsLeft.lat();

                    xR = pointsRight.lon();
                    yR = pointsRight.lat();
                    
                    g->draw_line({xL, yL}, {xR, yR});
                }
                
                //then, deal with the last curve point to the segment's "to" intersection
                pointsLeft = pointsRight;
                pointsRight = getIntersectionPosition(segmentInfo.to);
                xL = pointsLeft.lon();
                yL = pointsLeft.lat();

                xR = pointsRight.lon();
                yR = pointsRight.lat();                
                
                g->draw_line({xL, yL}, {xR, yR});
                
            }
        }
    }  
    
    //Drawing Intersections
    //***********************************************************************************
    
    //Primitives
    g->set_color (200, 200, 200, 255);
    
    //Drawing
    for(size_t i = 0; i < intersections.size(); ++i){

      float x = intersections[i].position.lon();
      float y = intersections[i].position.lat();

      //must convert lat lon values to cartesian (refer to tutorial slides)
//      x = x_from_lon(x);
//      y = y_from_lat(y);
     
      float width = 0.0001;
      float height = width;

      g->fill_rectangle({x-(width/2),y-(height/2)}, {x + (width/2), y + (height/2)});
    
    }
    
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
                LatLon point = getFeaturePoint(featurePointId, featureId);
            
//                //convert LatLon points into x y coordinates
//                previousPoint_x = x_from_lon (previousPoint.lon());
//                previousPoint_y = y_from_lat (previousPoint.lat());
//
//                point_x = x_from_lon (point.lon());
//                point_y = y_from_lat (point.lat());

                //draw line between feature points
                g->set_line_width(3);
                g->draw_line({previousPoint.lon(), previousPoint.lat()}, {point.lon(), point.lat()});
            }
        }
        else{
            std::vector<ezgl::point2d> points_xy_coordinates;
            
            //iterate through feature points to get (x,y) coordinates  in order to draw the polygon
            for (unsigned featurePointId = 0; featurePointId < numOfFeaturePoints; featurePointId++){

//                //convert LatLon points into (x, y) coordinate pairs
//                point_x = x_from_lon (getFeaturePoint(featurePointId, featureId).lon());
//                point_y = y_from_lat (getFeaturePoint(featurePointId, featureId).lat());

                points_xy_coordinates.push_back(ezgl::point2d(getFeaturePoint(featurePointId, featureId).lon(), getFeaturePoint(featurePointId, featureId).lat()));
            }
            
       //     ezgl function which takes a vector<point2d> (i.e (x,y) coordinates for each point that defines the feature outline)
            g->fill_poly(points_xy_coordinates);
        }
    }
    
}

std::pair < double, double > latLonToCartesian (double lat, double lon){
    //convert LatLon points into x y coordinates
    double y = lat*DEGREE_TO_RADIAN *EARTH_RADIUS_METERS;
    double x = lon*DEGREE_TO_RADIAN *EARTH_RADIUS_METERS*cos(latAvg);
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
    
}