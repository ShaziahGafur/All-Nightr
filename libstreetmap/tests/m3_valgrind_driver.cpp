#include <random>
#include <iostream>
#include <string>
#include <unittest++/UnitTest++.h>

#include "unit_test_util.h"

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"

extern void set_disable_event_loop (bool new_setting);

std::string map_name = "/cad2/ece297s/public/maps/saint-helena.streets.bin";

int main(int argc, char** argv) {
    //Disable interactive graphics
    set_disable_event_loop(true);

    bool load_success = load_map(map_name);

    if(!load_success) {
        std::cout << "ERROR: Could not load map file: '" << map_name << "'!";
        std::cout << " Subsequent tests will likely fail." << std::endl;
        //Don't abort tests, since we still want to show that all
        //tests fail.
    }

    //Run the unit tests
    int num_failures = UnitTest::RunAllTests();

    close_map();

    return num_failures;
}

struct MapFixture {
    MapFixture() {
        rng = std::minstd_rand(0);
        rand_intersection = std::uniform_int_distribution<IntersectionIndex>(0, getNumIntersections()-1);
        rand_street = std::uniform_int_distribution<StreetIndex>(1, getNumStreets()-1);
        rand_segment = std::uniform_int_distribution<StreetSegmentIndex>(0, getNumStreetSegments()-1);
        rand_poi = std::uniform_int_distribution<POIIndex>(0, getNumPointsOfInterest()-1);
        rand_lat = std::uniform_real_distribution<float>(-16.00203, -7.925433);
        rand_lon = std::uniform_real_distribution<float>(-14.41355, -5.649098);
        rand_turn_penalty = std::uniform_real_distribution<double>(0., 30.);
        rand_walking_speed = std::uniform_real_distribution<double>(0.8, 5.);
        rand_walking_time_limit = std::uniform_real_distribution<double>(0., 300.);
    }

    std::minstd_rand rng;
    std::uniform_int_distribution<IntersectionIndex> rand_intersection;
    std::uniform_int_distribution<StreetIndex> rand_street;
    std::uniform_int_distribution<StreetSegmentIndex> rand_segment;
    std::uniform_int_distribution<POIIndex> rand_poi;
    std::uniform_real_distribution<float> rand_lat;
    std::uniform_real_distribution<float> rand_lon;
    std::uniform_real_distribution<double> rand_turn_penalty;
    std::uniform_real_distribution<double> rand_walking_speed;
    std::uniform_real_distribution<double> rand_walking_time_limit;
};

TEST_FIXTURE(MapFixture, valgrind_exercise) {

    for(size_t i = 0; i < 10; i++) {

        IntersectionIndex source = rand_intersection(rng);
        IntersectionIndex destination = rand_intersection(rng);
        double turn_penalty = rand_turn_penalty(rng);

        std::vector<StreetSegmentIndex> path = find_path_between_intersections(source, destination, turn_penalty);

        double path_cost = compute_path_travel_time(path, turn_penalty);

          
        double walking_speed = rand_walking_speed(rng);
        double walking_time_limit = rand_walking_time_limit(rng); 
        std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> w_path;
        w_path = find_path_with_walk_to_pick_up(source, destination, turn_penalty, walking_speed, walking_time_limit); 
        path_cost = compute_path_travel_time(w_path.second, turn_penalty);
    }

};
