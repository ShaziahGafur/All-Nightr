
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

SUITE(spatial_queries_public_toronto_canada) {

    struct BaseMapFixture {
        BaseMapFixture() {
            //Load the map
            try {
                load_map("/cad2/ece297s/public/maps/toronto_canada.streets.bin");
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

        ECE297_CHECK_EQUAL(1950, find_closest_intersection(LatLon(43.71936416625976562, -79.52052307128906250)));

        ECE297_CHECK_EQUAL(3963, find_closest_intersection(LatLon(43.77373886108398438, -79.47676086425781250)));

        valid_results = {6027, 6028, 6035, 6044, 6045, 107400};
        actual = find_closest_intersection(LatLon(43.48199844360351562, -79.40149688720703125));
        ECE297_CHECK(valid_results.count(actual));

        ECE297_CHECK_EQUAL(6043, find_closest_intersection(LatLon(43.56536865234375000, -79.34043121337890625)));

        ECE297_CHECK_EQUAL(7057, find_closest_intersection(LatLon(43.73253250122070312, -79.09116363525390625)));

        ECE297_CHECK_EQUAL(10200, find_closest_intersection(LatLon(43.82280731201171875, -79.18561553955078125)));

        ECE297_CHECK_EQUAL(10455, find_closest_intersection(LatLon(43.60358810424804688, -79.29006958007812500)));

        ECE297_CHECK_EQUAL(21861, find_closest_intersection(LatLon(43.71036148071289062, -79.60935211181640625)));

        ECE297_CHECK_EQUAL(28593, find_closest_intersection(LatLon(43.71545028686523438, -79.45131683349609375)));

        ECE297_CHECK_EQUAL(43835, find_closest_intersection(LatLon(43.90982818603515625, -79.60358428955078125)));

        ECE297_CHECK_EQUAL(43989, find_closest_intersection(LatLon(43.77970123291015625, -79.60786437988281250)));

        ECE297_CHECK_EQUAL(44286, find_closest_intersection(LatLon(43.91736984252929688, -79.71572113037109375)));

        ECE297_CHECK_EQUAL(44398, find_closest_intersection(LatLon(43.81575775146484375, -79.77580261230468750)));

        ECE297_CHECK_EQUAL(45040, find_closest_intersection(LatLon(43.77691650390625000, -79.58846282958984375)));

        valid_results = {47233, 106419};
        actual = find_closest_intersection(LatLon(43.74923324584960938, -79.02179718017578125));
        ECE297_CHECK(valid_results.count(actual));

        ECE297_CHECK_EQUAL(60274, find_closest_intersection(LatLon(43.73324203491210938, -79.53834533691406250)));

        ECE297_CHECK_EQUAL(68924, find_closest_intersection(LatLon(43.56804275512695312, -79.77233886718750000)));

        ECE297_CHECK_EQUAL(69945, find_closest_intersection(LatLon(43.57031250000000000, -79.68901824951171875)));

        ECE297_CHECK_EQUAL(75530, find_closest_intersection(LatLon(43.58732604980468750, -79.09315490722656250)));

        ECE297_CHECK_EQUAL(75834, find_closest_intersection(LatLon(43.71529388427734375, -79.55553436279296875)));

        ECE297_CHECK_EQUAL(79633, find_closest_intersection(LatLon(43.69831848144531250, -79.65620422363281250)));

        ECE297_CHECK_EQUAL(80287, find_closest_intersection(LatLon(43.83801651000976562, -79.22557830810546875)));

        ECE297_CHECK_EQUAL(88790, find_closest_intersection(LatLon(43.89463806152343750, -79.20742797851562500)));

        ECE297_CHECK_EQUAL(99994, find_closest_intersection(LatLon(43.87495803833007812, -79.56263732910156250)));

        ECE297_CHECK_EQUAL(114993, find_closest_intersection(LatLon(43.65697860717773438, -79.70158386230468750)));

        ECE297_CHECK_EQUAL(119886, find_closest_intersection(LatLon(43.64250946044921875, -79.35969543457031250)));

        ECE297_CHECK_EQUAL(127808, find_closest_intersection(LatLon(43.90194320678710938, -79.57025146484375000)));

        ECE297_CHECK_EQUAL(132962, find_closest_intersection(LatLon(43.83702087402343750, -79.14601898193359375)));

        ECE297_CHECK_EQUAL(133921, find_closest_intersection(LatLon(43.87591171264648438, -79.01165008544921875)));

        ECE297_CHECK_EQUAL(145146, find_closest_intersection(LatLon(43.85248565673828125, -79.43147277832031250)));

    } //closest_intersection

} //spatial_queries_public_toronto_canada

