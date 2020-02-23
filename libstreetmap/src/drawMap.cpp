#include "m1.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "m2.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"

void draw_map_blank_canvas ();
void draw_main_canvas (ezgl::renderer *g);
void find_map_bounds(double* max_lat, double* min_lat, double* max_lon, double* min_lon);

void draw_map (){
    draw_map_blank_canvas();
}
void draw_map_blank_canvas (){       
ezgl::application::settings settings;
settings.main_ui_resource="libstreetmap/resources/main.ui";
settings.window_identifier="MainWindow";
settings.canvas_identifier="MainCanvas";

ezgl::application application(settings);

//4 pointers declared for minimum and maximum lat and lons
double* max_lat;
double* min_lat;
double* max_lon;
double* min_lon;
find_map_bounds(max_lat, min_lat, max_lon, min_lon); //determines the maximum and min lat & lons to format the map

ezgl::rectangle initial_world({*min_lon, *min_lat},{*max_lon, *max_lat});
//ezgl::rectangle initial_world({0, 0},{1000, 1000});
application.add_canvas("MainCanvas", draw_main_canvas, initial_world);


application.run(NULL,NULL,NULL,NULL);            
}

void draw_main_canvas (ezgl::renderer *g){
    g->draw_rectangle({0, 0}, {1000, 1000});
    g->set_color (ezgl::BLACK);
    g->set_color (0, 0, 0, 255);  // 8-bit r, g, b, alpha. alpha of 255 is opaque, 0 is transparent
    g->set_line_width (3);   // 3 pixels wide
    g->set_line_dash (ezgl::line_dash::asymmetric_5_3);
}

/**
 * Finds the maximum and minimum lat and lons of the map based on intersections
 */
void find_map_bounds(double* max_lat, double* min_lat, double* max_lon, double* min_lon){
    //Initializes maximum and minimum values to the values of the first intersection
    *max_lat = getIntersectionPosition(0).lat(); 
    *min_lat = *max_lat;
    *max_lon = getIntersectionPosition(0).lon();
    *min_lon = *max_lon;
    
    //loops through all intersections, searches for the max & min lat & lons in the entire map and assigning them to the double pointers
    for (int intersectionId = 0; intersectionId < getNumIntersections(); intersectionId++){
        
//        *max_lat = std::max(*max_lat, IntersectionCoordinates[intersectionId].lat());
//        *min_lat = std::min(*min_lat, IntersectionCoordinates[intersectionId].lat());
//        *max_lon = std::max(*max_lon, IntersectionCoordinates[intersectionId].lon());
//        *min_lon = std::min(*min_lon, IntersectionCoordinates[intersectionId].lon());
    }
}