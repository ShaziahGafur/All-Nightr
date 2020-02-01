
#include "m1.h"
#include "unit_test_util.h"

#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"

#include <unittest++/UnitTest++.h>

#include <random>
#include <algorithm>
#include <set>

using ece297test::relative_error;
using ece297test::sorted;

SUITE(spatial_queries_public_saint_helena) {

    struct BaseMapFixture {
        BaseMapFixture() {
            //Load the map
            try {
                load_map("/cad2/ece297s/public/maps/saint-helena.streets.bin");
            } catch (...) {
                std::cout << "!!!! BaseMapFixture test setup: load_map threw an exceptinon !!!!" << std::endl;
                throw; // re-throw exceptinon
            }
        }
    
        ~BaseMapFixture() {
            //Clean-up
            try {
                close_map();
            } catch (const std::exception& e) {
                std::cout << "!!!! BaseMapFixture test teardown: close_map threw an exceptinon. what(): " << e.what() << " !!!!" << std::endl;
                std::terminate(); // we're in a destructor
            } catch (...) {
                std::cout << "!!!! BaseMapFixture test teardown: close_map threw an exceptinon !!!!" << std::endl;
                std::terminate(); // we're in a destructor
            }
        }
    };


    struct MapFixture : BaseMapFixture {};

    TEST_FIXTURE(MapFixture, closest_intersection) {
        std::set<IntersectionIndex> valid_results;
        IntersectionIndex actual;

        ECE297_CHECK_EQUAL(106, find_closest_intersection(LatLon(-15.92557716369628906, -5.67993307113647461)));

        ECE297_CHECK_EQUAL(200, find_closest_intersection(LatLon(-15.96410846710205078, -5.73165512084960938)));

        ECE297_CHECK_EQUAL(248, find_closest_intersection(LatLon(-16.01629066467285156, -5.71199226379394531)));

        ECE297_CHECK_EQUAL(248, find_closest_intersection(LatLon(-15.99796295166015625, -5.70190429687500000)));

        ECE297_CHECK_EQUAL(249, find_closest_intersection(LatLon(-15.96500301361083984, -5.73743915557861328)));

        ECE297_CHECK_EQUAL(249, find_closest_intersection(LatLon(-15.96105766296386719, -5.73459911346435547)));

        valid_results = {250, 251};
        actual = find_closest_intersection(LatLon(-15.95084381103515625, -5.74608373641967773));
        ECE297_CHECK(valid_results.count(actual));

        ECE297_CHECK_EQUAL(251, find_closest_intersection(LatLon(-15.95145702362060547, -5.74287843704223633)));

        valid_results = {275, 276};
        actual = find_closest_intersection(LatLon(-15.92397212982177734, -5.73987007141113281));
        ECE297_CHECK(valid_results.count(actual));

        ECE297_CHECK_EQUAL(290, find_closest_intersection(LatLon(-15.96496868133544922, -5.72022294998168945)));

        ECE297_CHECK_EQUAL(297, find_closest_intersection(LatLon(-15.92969417572021484, -5.64759063720703125)));

        ECE297_CHECK_EQUAL(300, find_closest_intersection(LatLon(-15.96121406555175781, -5.66072654724121094)));

        ECE297_CHECK_EQUAL(304, find_closest_intersection(LatLon(-15.98956108093261719, -5.69358444213867188)));

        ECE297_CHECK_EQUAL(352, find_closest_intersection(LatLon(-15.98100376129150391, -5.70508718490600586)));

        ECE297_CHECK_EQUAL(390, find_closest_intersection(LatLon(-15.99313545227050781, -5.66105508804321289)));

    } //closest_intersection

} //spatial_queries_public_saint_helena

