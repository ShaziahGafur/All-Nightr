#ifndef DRAWMAP_H
#define DRAWMAP_H

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

#define default_turn_penalty 0.25 // (15s converted to minutes)

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

#endif /* DRAWMAP_H */

