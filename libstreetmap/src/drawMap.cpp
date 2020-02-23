#include "m2.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"

void draw_map();
void draw_map_blank_canvas ();
void draw_main_canvas (ezgl::renderer *g);

void draw_map(){
    draw_map_blank_canvas();
}
void draw_map_blank_canvas (){       
ezgl::application::settings settings;
settings.main_ui_resource="libstreetmap/resources/main.ui";
settings.window_identifier="MainWindow";
settings.canvas_identifier="MainCanvas";

ezgl::application application(settings);

ezgl:: rectangle initial_world({0,0},{1000,1000});
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
