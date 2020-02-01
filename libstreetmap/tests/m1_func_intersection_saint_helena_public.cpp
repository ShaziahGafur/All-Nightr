
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

SUITE(intersection_queries_public_saint_helena) {

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

    TEST_FIXTURE(MapFixture, intersection_street_segments) {
        std::vector<StreetSegmentIndex> expected;

        expected = {0, 402, 403};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(0)));

        expected = {51, 255};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(81)));

        expected = {62, 63};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(96)));

        expected = {90, 91};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(112)));

        expected = {122, 123};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(180)));

        expected = {159, 343, 344};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(228)));

        expected = {170, 327, 328};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(239)));

        expected = {209, 210, 211};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(283)));

        expected = {212, 359, 360};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(286)));

        expected = {234, 235, 239};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(313)));

        expected = {250, 251};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(331)));

        expected = {270, 271};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(337)));

        expected = {335, 336, 338};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(379)));

        expected = {364, 453};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(325)));

        expected = {397, 398};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(107)));

    } //intersection_street_segments

    TEST_FIXTURE(MapFixture, intersection_street_names) {
        std::vector<std::string> expected;

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(81)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(107)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(325)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(331)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(337)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(228)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(379)));

        expected = {"<unknown>", "<unknown>", "The Pavement"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(283)));

        expected = {"<unknown>", "Casons", "Casons"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(286)));

        expected = {"<unknown>", "Commonwealth Avenue", "Commonwealth Avenue"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(239)));

        expected = {"<unknown>", "Evergreen Drive", "Evergreen Drive"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(313)));

        expected = {"<unknown>", "Longwood Avenue", "Longwood Avenue"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(0)));

        expected = {"Colt Sheds", "Colt Sheds"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(112)));

        expected = {"Cow Path", "Cow Path"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(180)));

        expected = {"Deadwood", "Deadwood"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(96)));

    } //intersection_street_names

    TEST_FIXTURE(MapFixture, directly_connected) {
        ECE297_CHECK(!are_directly_connected(std::make_pair(116, 293)));

        ECE297_CHECK(!are_directly_connected(std::make_pair(241, 302)));

        ECE297_CHECK(!are_directly_connected(std::make_pair(371, 207)));

        ECE297_CHECK(!are_directly_connected(std::make_pair(377, 374)));

        ECE297_CHECK(!are_directly_connected(std::make_pair(402, 295)));

        ECE297_CHECK(!are_directly_connected(std::make_pair(413, 75)));

        ECE297_CHECK(are_directly_connected(std::make_pair(0, 0)));

        ECE297_CHECK(are_directly_connected(std::make_pair(81, 85)));

        ECE297_CHECK(are_directly_connected(std::make_pair(96, 5)));

        ECE297_CHECK(are_directly_connected(std::make_pair(107, 116)));

        ECE297_CHECK(are_directly_connected(std::make_pair(180, 181)));

        ECE297_CHECK(are_directly_connected(std::make_pair(224, 238)));

        ECE297_CHECK(are_directly_connected(std::make_pair(239, 231)));

        ECE297_CHECK(are_directly_connected(std::make_pair(283, 285)));

        ECE297_CHECK(are_directly_connected(std::make_pair(286, 290)));

    } //directly_connected

    TEST_FIXTURE(MapFixture, adjacent_intersections) {
        std::vector<IntersectionIndex> expected;

        expected = {4, 95, 96, 130};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(5)));

        expected = {8, 10};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(9)));

        expected = {14, 16, 362};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(15)));

        expected = {22, 106, 385};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(23)));

        expected = {103, 105};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(104)));

        expected = {136, 137};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(112)));

        expected = {142};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(143)));

        expected = {162, 231, 371};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(239)));

        expected = {173, 282};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(185)));

        expected = {210, 400};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(401)));

        expected = {213, 215};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(214)));

        expected = {286, 288, 290};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(287)));

        expected = {348, 350};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(349)));

        expected = {349, 355, 393};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(350)));

        expected = {370, 402, 405};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(408)));

    } //adjacent_intersections

} //intersection_queries_public_saint_helena

