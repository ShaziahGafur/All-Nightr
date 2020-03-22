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
std::string default_map_path = "london_england";
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
    //draw_map();
    std::vector<StreetSegmentIndex> path;
        double turn_penalty;
        double expected;
        double actual;

        path = {189462, 189466, 74720, 74719, 261581, 261580, 261579, 261578, 261577, 261576, 308932, 74697, 261427, 181922, 181923, 228147, 228148, 262362, 261431, 232466, 10612, 10613, 10614, 228137, 143657, 143658, 143659, 228138, 144825, 155038, 177489, 155040, 155039, 294676, 294675, 76966, 294682, 294681, 246948, 146832, 223459, 223458, 223457, 5607, 261620, 289793, 36531, 36532, 36533, 36534, 36535, 36536, 36537, 66479, 131054, 131053, 88454, 88506, 88507, 277711, 277712, 267540, 267541, 267542, 267543, 267544, 267545, 267546, 267547, 267548, 267549, 267550, 267551, 267552, 267553, 267554, 267555, 267556, 267557, 143966, 143967, 143968, 129380, 129381, 277716, 277715, 83443, 83444, 263799, 164136, 227938, 227946, 131800, 131799, 166921, 166920, 277019, 124665, 124664, 156282, 156281, 156280, 260176, 156279, 163162, 277250, 163163, 156278, 156277, 156276, 122374, 122375, 122376, 122377, 122378, 122379, 122380, 122381, 236321, 121683, 308442, 308443, 308444, 261604, 143812, 143811, 143810, 278404, 278403, 278402, 129422, 129421, 115867, 115866, 115865, 143815, 143814, 143813, 9761, 292814, 292815, 121684, 292818, 292817, 292816, 211960, 83389, 83388, 83385, 293952, 293951, 293950, 86590, 158743, 158744, 158745, 158746, 12210, 12211, 12212, 12213, 12214, 277224, 133631, 78828, 78827, 78826, 293939, 293938, 293937, 293936, 293935, 293934, 293933, 293932, 293931, 293930, 129167, 129166, 158753};
        turn_penalty = 2.68072293181142607;
        expected = 1054.65932540458447875;
        actual = compute_path_travel_time(path, turn_penalty);
        std:: cout << actual;
    std::cout << "Closing map\n";
    close_map(); 

    return SUCCESS_EXIT_CODE;
}
