#include "m1.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "m2.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "intersection_data.h"
#include <vector>
#include <string>

/************  GLOBAL VARIABLES  *****************/

std::vector<intersection_data> intersections;
float latAvg; //average latitude of map, value set in draw_map_blank_canvas

/************  FUNCTION DECLARATIONS  ***********/
void draw_map_blank_canvas ();
void draw_main_canvas (ezgl::renderer *g);

void draw_intersections();

void find_map_bounds(double& max_lat, double& min_lat, double& max_lon, double& min_lon);

float x_from_lon(float lon);
float y_from_lat(float lat);
float lon_from_x(float x);
float lat_from_y(float y);

/************************************************/


void draw_map(){
    draw_map_blank_canvas();
}
void draw_map_blank_canvas (){       
    ezgl::application::settings settings;
    settings.main_ui_resource = "libstreetmap/resources/main.ui";
    settings.window_identifier = "MainWindow";
    settings.canvas_identifier = "MainCanvas";

    ezgl::application application(settings);

    
    int numIntersections = getNumIntersections();
        
    double max_lat = getIntersectionPosition(0).lat(); 
    double min_lat = max_lat;
    double max_lon = getIntersectionPosition(0).lon();
    double min_lon = max_lon;
    intersections.resize(numIntersections);
    
    for(int i = 0; i < numIntersections; i++){
        
        //get position from streetsdatabaseAPI function
        intersections[i].position = getIntersectionPosition(i);
        
        //get name from streetsdatabaseAPI function
        intersections[i].name = getIntersectionName(i);
        
        max_lat = std::max(max_lat, intersections[i].position.lat());
        min_lat = std::min(min_lat, intersections[i].position.lat());
        max_lon = std::max(max_lon, intersections[i].position.lon());
        min_lon = std::min(min_lon, intersections[i].position.lon());
    }
    
    latAvg = (max_lat + min_lat ) * 0.5; //sets value of global variable
    
    ezgl::rectangle initial_world({min_lon, min_lat},{max_lon, max_lat});
    application.add_canvas("MainCanvas", draw_main_canvas, initial_world);
    application.run(NULL,NULL,NULL,NULL);
}

void draw_main_canvas (ezgl::renderer *g){
    g->draw_rectangle({0, 0}, {1000, 1000});
//    g->set_color (ezgl::BLACK);
//    g->set_color (0, 0, 0, 255);  // 8-bit r, g, b, alpha. alpha of 255 is opaque, 0 is transparent
//    g->set_line_width (3);   // 3 pixels wide
//    g->set_line_dash (ezgl::line_dash::asymmetric_5_3);
    
    for(size_t i = 0; i < intersections.size(); ++i){

      float x = intersections[i].position.lon();
      float y = intersections[i].position.lat();
//      x = x_from_lon(x);
//      y = y_from_lat(y);
            
      float width = 0.001;
      float height = width;

      g->fill_rectangle({x,y}, {x + width, y + height});
    
    }
}

/**
 * Finds the maximum and minimum lat and lons of the map based on intersections
 */
void find_map_bounds(double& max_lat, double& min_lat, double& max_lon, double& min_lon){
    //Initializes maximum and minimum values to the values of the first intersection
    max_lat = getIntersectionPosition(0).lat(); 
    min_lat = max_lat;
    max_lon = getIntersectionPosition(0).lon();
    min_lon = max_lon;
        
//    
//    //loops through all intersections, searches for the max & min lat & lons in the entire map and assigning them to the double pointers
    for (int intersectionId = 0; intersectionId < getNumIntersections(); intersectionId++){
        
        max_lat = std::max(max_lat, intersections[intersectionId].position.lat());
        min_lat = std::min(min_lat, intersections[intersectionId].position.lat());
        max_lon = std::max(max_lon, intersections[intersectionId].position.lon());
        min_lon = std::min(min_lon, intersections[intersectionId].position.lon());
    }
}

float x_from_lon(float lon){
    return lon*DEGREE_TO_RADIAN *cos(latAvg); 
}

float y_from_lat(float lat){
    return lat*DEGREE_TO_RADIAN;
}

float lon_from_x(float x){
    return 0;
}

float lat_from_y(float y){
    return 0;
}


//void populateIntersections(){
//    
//    int numIntersections = getNumIntersections();
//        
//    intersections.resize(numIntersections);
//    
//    for(int i = 0; i < numIntersections; i++){
//        
//        //get position from streetsdatabaseAPI function
//        intersections[i].position = getIntersectionPosition(i);
//        
//        //get name from streetsdatabaseAPI function
//        intersections[i].name = getIntersectionName(i);
//        std::cout<<"\nIntersection # "<<i<<intersections[i].position.lat();
//    }
//}

/*void draw_intersections(ezgl::renderer *g){
    for(size_t i = 0; i < intersections.size(); ++i){
        
        float x = intersections[i].position.lon();
        float y = intersections[i].position.lat();
        
        float width = 1;
        
        float height = width;
        
        g.fill_rectangle({x,y}, {x + width, y + height});
    
    }
}*/
