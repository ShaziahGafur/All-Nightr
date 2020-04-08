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
#include "drawMap.h"
#include "globals.h"
#include "m3.h"
#include "m3A.h"
#include "m4.h"

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

    //Quick test for find path:
    
//    find_path_between_intersections(108, 115, 5);
//    std::cout<<getIntersectionName(100)+"\t To:"+getIntersectionName(150)+"\n";
//    find_path_with_walk_to_pick_up(100, 150, 1, 2, 1200);
//    std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> path;
//    path = find_path_with_walk_to_pick_up(29171, 87192, 24.69242195727185418, 3.75261393716713698, 41.02582674660070694);   
    
        std::vector<DeliveryInfo> deliveries;
        std::vector<IntersectionIndex> depots;
        std::vector<CourierSubpath> result_path;
        float turn_penalty;
        float truck_capacity;
        
        deliveries = {DeliveryInfo(50955, 114599, 3.64505), DeliveryInfo(13864, 84826, 150.28346), DeliveryInfo(106836, 40478, 132.79056), DeliveryInfo(9037, 47950, 133.22345), DeliveryInfo(56819, 91575, 29.19379), DeliveryInfo(114780, 2526, 13.61199), DeliveryInfo(66440, 3042, 64.46082), DeliveryInfo(43487, 146131, 188.11136), DeliveryInfo(48026, 115825, 122.91425), DeliveryInfo(9088, 13180, 194.16504), DeliveryInfo(135696, 89864, 140.79376), DeliveryInfo(70817, 46762, 55.83173), DeliveryInfo(128368, 75909, 53.42627), DeliveryInfo(7107, 38918, 139.53435), DeliveryInfo(131450, 69208, 62.54547), DeliveryInfo(54341, 50187, 132.25157), DeliveryInfo(55780, 131937, 115.65539), DeliveryInfo(18428, 22635, 1.23825), DeliveryInfo(148729, 29698, 171.64159), DeliveryInfo(26717, 126555, 111.32867), DeliveryInfo(90722, 88823, 146.61469), DeliveryInfo(89818, 53919, 37.15287), DeliveryInfo(56077, 99475, 5.99976), DeliveryInfo(57606, 120679, 14.50743), DeliveryInfo(36647, 74187, 88.43934), DeliveryInfo(36147, 120472, 55.23724), DeliveryInfo(26866, 53224, 157.20303), DeliveryInfo(7832, 78160, 147.67279), DeliveryInfo(35474, 100371, 113.03408), DeliveryInfo(49900, 91799, 68.13067), DeliveryInfo(110047, 105659, 135.47527), DeliveryInfo(65531, 147042, 126.80992), DeliveryInfo(92846, 73209, 41.46676), DeliveryInfo(76847, 96421, 41.83722), DeliveryInfo(147775, 116379, 124.71667), DeliveryInfo(99955, 88995, 198.43752), DeliveryInfo(24143, 51489, 177.14240), DeliveryInfo(115112, 98300, 40.57933), DeliveryInfo(91958, 124878, 4.58260), DeliveryInfo(60411, 58215, 6.86531), DeliveryInfo(147089, 80441, 195.18217), DeliveryInfo(113021, 46252, 39.01222), DeliveryInfo(88253, 56464, 159.00911), DeliveryInfo(110603, 12428, 128.93202), DeliveryInfo(116429, 120125, 77.88078), DeliveryInfo(95180, 105909, 183.02200), DeliveryInfo(145225, 24659, 55.50504), DeliveryInfo(137276, 56686, 83.74704), DeliveryInfo(99914, 68916, 153.42888), DeliveryInfo(28583, 104773, 165.24220)};
        depots = {13, 51601, 61505, 85936, 132265};
        turn_penalty = 15.000000000;
        truck_capacity = 306.382446289;

        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity); 
    

    std::cout << "Closing map\n";
    close_map(); 

    return SUCCESS_EXIT_CODE;
}
