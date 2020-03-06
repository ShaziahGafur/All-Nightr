#include <random>
#include <unittest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"

#include "unit_test_util.h"
#include "path_verify.h"

using ece297test::relative_error;
using ece297test::path_is_legal;


SUITE(inter_inter_simple_path_func_legality_public) {
struct MapFixture {
    MapFixture() {
        rng = std::minstd_rand(4);
        rand_intersection = std::uniform_int_distribution<IntersectionIndex>(0, getNumIntersections()-1);
        rand_street = std::uniform_int_distribution<StreetIndex>(1, getNumStreets()-1);
        rand_segment = std::uniform_int_distribution<StreetSegmentIndex>(0, getNumStreetSegments()-1);
        rand_poi = std::uniform_int_distribution<POIIndex>(0, getNumPointsOfInterest()-1);
        rand_lat = std::uniform_real_distribution<double>(46.650653839, 46.711387634);
        rand_lon = std::uniform_real_distribution<double>(7.800767422, 7.915423393);
        rand_turn_penalty = std::uniform_real_distribution<double>(0., 30.);
        rand_walking_speed = std::uniform_real_distribution<double>(0.8, 5);
        rand_walking_time_limit = std::uniform_real_distribution<double>(0.0, 300);
    }

    std::minstd_rand rng;
    std::uniform_int_distribution<IntersectionIndex> rand_intersection;
    std::uniform_int_distribution<StreetSegmentIndex> rand_street;
    std::uniform_int_distribution<StreetSegmentIndex> rand_segment;
    std::uniform_int_distribution<POIIndex> rand_poi;
    std::uniform_real_distribution<double> rand_lat;
    std::uniform_real_distribution<double> rand_lon;
    std::uniform_real_distribution<double> rand_turn_penalty;
    std::uniform_real_distribution<double> rand_walking_speed;
    std::uniform_real_distribution<double> rand_walking_time_limit;
};
    TEST(find_path_between_intersections_simple_legality) {
        std::vector<StreetSegmentIndex> path;

        path = find_path_between_intersections(109, 110, 0.00000000000000000);
        CHECK(path_is_legal(109, 110, path));

        path = find_path_between_intersections(167, 188, 0.00000000000000000);
        CHECK(path_is_legal(167, 188, path));

        path = find_path_between_intersections(94, 297, 0.00000000000000000);
        CHECK(path_is_legal(94, 297, path));

        path = find_path_between_intersections(323, 40, 0.00000000000000000);
        CHECK(path_is_legal(323, 40, path));

        path = find_path_between_intersections(622, 620, 0.00000000000000000);
        CHECK(path_is_legal(622, 620, path));

        path = find_path_between_intersections(643, 642, 0.00000000000000000);
        CHECK(path_is_legal(643, 642, path));

        path = find_path_between_intersections(657, 658, 0.00000000000000000);
        CHECK(path_is_legal(657, 658, path));

        path = find_path_between_intersections(743, 678, 0.00000000000000000);
        CHECK(path_is_legal(743, 678, path));

        path = find_path_between_intersections(682, 749, 0.00000000000000000);
        CHECK(path_is_legal(682, 749, path));

        path = find_path_between_intersections(324, 326, 0.00000000000000000);
        CHECK(path_is_legal(324, 326, path));

        path = find_path_between_intersections(850, 848, 0.00000000000000000);
        CHECK(path_is_legal(850, 848, path));

        path = find_path_between_intersections(885, 903, 0.00000000000000000);
        CHECK(path_is_legal(885, 903, path));

        path = find_path_between_intersections(972, 1187, 0.00000000000000000);
        CHECK(path_is_legal(972, 1187, path));

        path = find_path_between_intersections(489, 1035, 0.00000000000000000);
        CHECK(path_is_legal(489, 1035, path));

        path = find_path_between_intersections(1331, 460, 0.00000000000000000);
        CHECK(path_is_legal(1331, 460, path));

        path = find_path_between_intersections(579, 798, 0.00000000000000000);
        CHECK(path_is_legal(579, 798, path));

        path = find_path_between_intersections(1214, 1425, 0.00000000000000000);
        CHECK(path_is_legal(1214, 1425, path));

        path = find_path_between_intersections(1188, 975, 0.00000000000000000);
        CHECK(path_is_legal(1188, 975, path));

        path = find_path_between_intersections(1459, 1458, 0.00000000000000000);
        CHECK(path_is_legal(1459, 1458, path));

        path = find_path_between_intersections(1453, 1732, 0.00000000000000000);
        CHECK(path_is_legal(1453, 1732, path));

        path = find_path_between_intersections(1513, 1503, 0.00000000000000000);
        CHECK(path_is_legal(1513, 1503, path));

        path = find_path_between_intersections(945, 41, 0.00000000000000000);
        CHECK(path_is_legal(945, 41, path));

        path = find_path_between_intersections(1589, 1588, 0.00000000000000000);
        CHECK(path_is_legal(1589, 1588, path));

        path = find_path_between_intersections(1599, 1602, 0.00000000000000000);
        CHECK(path_is_legal(1599, 1602, path));

        path = find_path_between_intersections(1641, 952, 0.00000000000000000);
        CHECK(path_is_legal(1641, 952, path));

        path = find_path_between_intersections(0, 1105, 0.00000000000000000);
        CHECK(path_is_legal(0, 1105, path));

        path = find_path_between_intersections(1292, 284, 0.00000000000000000);
        CHECK(path_is_legal(1292, 284, path));

        path = find_path_between_intersections(1721, 1722, 0.00000000000000000);
        CHECK(path_is_legal(1721, 1722, path));

        path = find_path_between_intersections(150, 295, 0.00000000000000000);
        CHECK(path_is_legal(150, 295, path));

        path = find_path_between_intersections(1798, 1800, 0.00000000000000000);
        CHECK(path_is_legal(1798, 1800, path));

    } //find_path_between_intersections_simple_legality

} //inter_inter_simple_path_func_legality_public

