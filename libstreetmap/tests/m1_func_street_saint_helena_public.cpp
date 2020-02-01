
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

SUITE(street_queries_public_saint_helena) {

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

    TEST_FIXTURE(MapFixture, street_street_segments) {
        std::vector<StreetSegmentIndex> expected;

        expected = {10, 11, 12, 13};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_street(1)));

        expected = {61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_street(6)));

        expected = {90, 91};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_street(7)));

        expected = {104, 105, 106, 107, 108, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362, 363};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_street(8)));

        expected = {119, 120, 121, 122, 123, 124, 125};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_street(9)));

        expected = {201, 202};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_street(15)));

        expected = {226, 288, 289, 290, 291, 455};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_street(18)));

        expected = {233, 234, 235, 236, 237};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_street(19)));

        expected = {292, 293};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_street(21)));

        expected = {295, 449};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_street(23)));

        expected = {301};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_street(25)));

        expected = {302};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_street(26)));

        expected = {303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323, 324, 325, 326, 327, 328};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_street(27)));

        expected = {389, 390, 391, 392, 445, 446, 447};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_street(30)));

        expected = {458, 459};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_street(33)));

    } //street_street_segments

    TEST_FIXTURE(MapFixture, all_street_intersections) {
        std::vector<IntersectionIndex> expected;

        expected = {5, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_street(6)));

        expected = {13, 14, 15, 16, 17};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_street(1)));

        expected = {13, 44, 46, 262, 263, 360, 412, 414};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_street(30)));

        expected = {16, 358};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_street(26)));

        expected = {17, 356, 357};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_street(21)));

        expected = {50, 51, 54, 162, 164, 168, 175, 184, 208, 224, 238, 239, 244, 259, 277, 306, 307, 316, 320, 342, 347, 366, 367, 368, 369, 370, 371};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_street(27)));

        expected = {53, 265, 277};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_street(15)));

        expected = {55, 304, 351, 352, 353, 354, 355};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_street(18)));

        expected = {112, 136, 137};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_street(7)));

        expected = {149, 150, 153, 154, 155, 156, 157, 158, 159, 162, 212, 286, 289, 290, 291, 292, 392};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_street(8)));

        expected = {167, 174, 259, 311, 312, 313};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_street(19)));

        expected = {176, 177, 178, 179, 180, 181, 182, 183};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_street(9)));

        expected = {359, 360, 364};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_street(23)));

        expected = {362, 365};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_street(25)));

        expected = {371, 398, 417};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_street(33)));

    } //all_street_intersections

    TEST_FIXTURE(MapFixture, intersection_ids_from_street_ids) {
        std::vector<IntersectionIndex> expected;

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_two_streets(std::make_pair(8, 19))));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_two_streets(std::make_pair(9, 18))));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_two_streets(std::make_pair(16, 11))));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_two_streets(std::make_pair(23, 12))));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_two_streets(std::make_pair(27, 18))));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_two_streets(std::make_pair(28, 10))));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_two_streets(std::make_pair(29, 25))));

        expected = {14, 15, 17};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_two_streets(std::make_pair(1, 0))));

        expected = {45};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_two_streets(std::make_pair(3, 0))));

        expected = {46};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_two_streets(std::make_pair(30, 4))));

        expected = {162, 168, 175, 208, 224, 238, 239, 244, 259, 307, 316, 320, 347, 367, 370, 371};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_two_streets(std::make_pair(27, 0))));

        expected = {262, 263};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_two_streets(std::make_pair(30, 0))));

        expected = {282, 300, 301};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_two_streets(std::make_pair(17, 0))));

        expected = {357};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_two_streets(std::make_pair(21, 32))));

        expected = {362};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersections_of_two_streets(std::make_pair(25, 0))));

    } //intersection_ids_from_street_ids

    TEST_FIXTURE(MapFixture, street_ids_from_partial_street_name) {
        std::vector<StreetIndex> expected;

        expected = {1};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Mar")));

        expected = {3};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Castle Terrace")));

        expected = {6};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Deadwood")));

        expected = {8};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Casons")));

        expected = {10};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Wireless Station D")));

        expected = {10};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Wireless Station Drive")));

        expected = {11};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Sapper Way")));

        expected = {11, 12, 18};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Sa")));

        expected = {18};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Sandy Bay Ridge")));

        expected = {19};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Evergreen ")));

        expected = {23};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Seafron")));

        expected = {24};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Constitution Hill")));

        expected = {25};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Shy Road")));

        expected = {27};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Comm")));

        expected = {27};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Commonweal")));

    } //street_ids_from_partial_street_name

} //street_queries_public_saint_helena

