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
#include <iostream>
#include <string>
#include "m1.h"
#include "m2.h" //should this be included, since drawMap.cpp must include it too
#include "globals.h"
#include "m3.h"

//Program exit codes
constexpr int SUCCESS_EXIT_CODE = 0;        //Everyting went OK
constexpr int ERROR_EXIT_CODE = 1;          //An error occured
constexpr int BAD_ARGUMENTS_EXIT_CODE = 2;  //Invalid command-line usage

//The default map to load if none is specified
std::string default_map_path = "toronto_canada";
std::string path_directory = "/cad2/ece297s/public/maps/";
std::string file_type = ".streets.bin";

int main(int argc, char** argv) {
    
    std::string map_path;   
    if(argc == 1) {
        //Use a default map
        map_path = default_map_path;
    } else if (argc == 2) {
        //Get the map from the command line
        map_path = argv[1];
    } else {
        //Invalid arguments
        std::cerr << "Usage: " << argv[0] << " [map_file_path]\n";
        std::cerr << "  If no map_file_path is provided a default map is loaded.\n";
        return BAD_ARGUMENTS_EXIT_CODE;
    }

    //Load the map and related data structures
    bool load_success = load_map(path_directory + map_path + file_type);
    if(!load_success) {
        std::cerr << "Failed to load map '" << path_directory<<map_path<<file_type<< "'\n";
        return ERROR_EXIT_CODE;
    }

    std::cout << "Successfully loaded map '" <<path_directory<<map_path<<file_type<< "'\n";

    //You can now do something with the map data
    //Clean-up the map data and related data structures
//    draw_map();
//    std::vector<StreetSegmentIndex> path;
//        double turn_penalty;
//        double expected;
//        double actual;
    //Quick test for find path:
    
//    find_path_between_intersections(108, 115, 5);

    std::cout << "Closing map\n";
    close_map(); 

    return SUCCESS_EXIT_CODE;
}
