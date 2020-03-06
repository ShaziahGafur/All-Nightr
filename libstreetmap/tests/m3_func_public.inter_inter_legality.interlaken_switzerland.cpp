#include <random>
#include <unittest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"

#include "unit_test_util.h"
#include "path_verify.h"

using ece297test::relative_error;
using ece297test::path_is_legal;


SUITE(inter_inter_path_func_legality_public) {
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
    TEST(find_path_between_intersections_legality) {
        std::vector<StreetSegmentIndex> path;

        path = find_path_between_intersections(0, 624, 0.00000000000000000);
        CHECK(path_is_legal(0, 624, path));

        path = find_path_between_intersections(94, 1787, 0.00000000000000000);
        CHECK(path_is_legal(94, 1787, path));

        path = find_path_between_intersections(109, 1086, 0.00000000000000000);
        CHECK(path_is_legal(109, 1086, path));

        path = find_path_between_intersections(429, 109, 15.00000000000000000);
        CHECK(path_is_legal(429, 109, path));

        path = find_path_between_intersections(525, 1408, 15.00000000000000000);
        CHECK(path_is_legal(525, 1408, path));

        path = find_path_between_intersections(345, 1292, 0.00000000000000000);
        CHECK(path_is_legal(345, 1292, path));

        path = find_path_between_intersections(616, 1331, 15.00000000000000000);
        CHECK(path_is_legal(616, 1331, path));

        path = find_path_between_intersections(657, 443, 0.00000000000000000);
        CHECK(path_is_legal(657, 443, path));

        path = find_path_between_intersections(678, 687, 15.00000000000000000);
        CHECK(path_is_legal(678, 687, path));

        path = find_path_between_intersections(674, 85, 0.00000000000000000);
        CHECK(path_is_legal(674, 85, path));

        path = find_path_between_intersections(730, 696, 15.00000000000000000);
        CHECK(path_is_legal(730, 696, path));

        path = find_path_between_intersections(743, 1039, 0.00000000000000000);
        CHECK(path_is_legal(743, 1039, path));

        path = find_path_between_intersections(803, 1337, 15.00000000000000000);
        CHECK(path_is_legal(803, 1337, path));

        path = find_path_between_intersections(929, 1798, 0.00000000000000000);
        CHECK(path_is_legal(929, 1798, path));

        path = find_path_between_intersections(1067, 292, 15.00000000000000000);
        CHECK(path_is_legal(1067, 292, path));

        path = find_path_between_intersections(1151, 323, 0.00000000000000000);
        CHECK(path_is_legal(1151, 323, path));

        path = find_path_between_intersections(1208, 792, 0.00000000000000000);
        CHECK(path_is_legal(1208, 792, path));

        path = find_path_between_intersections(603, 437, 0.00000000000000000);
        CHECK(path_is_legal(603, 437, path));

        path = find_path_between_intersections(856, 580, 15.00000000000000000);
        CHECK(path_is_legal(856, 580, path));

        path = find_path_between_intersections(1386, 1188, 15.00000000000000000);
        CHECK(path_is_legal(1386, 1188, path));

        path = find_path_between_intersections(1388, 324, 15.00000000000000000);
        CHECK(path_is_legal(1388, 324, path));

        path = find_path_between_intersections(1599, 1392, 0.00000000000000000);
        CHECK(path_is_legal(1599, 1392, path));

        path = find_path_between_intersections(1641, 222, 15.00000000000000000);
        CHECK(path_is_legal(1641, 222, path));

        path = find_path_between_intersections(1660, 1589, 0.00000000000000000);
        CHECK(path_is_legal(1660, 1589, path));

        path = find_path_between_intersections(1166, 1778, 15.00000000000000000);
        CHECK(path_is_legal(1166, 1778, path));

        path = find_path_between_intersections(1756, 470, 15.00000000000000000);
        CHECK(path_is_legal(1756, 470, path));

        path = find_path_between_intersections(1367, 1208, 0.00000000000000000);
        CHECK(path_is_legal(1367, 1208, path));

        path = find_path_between_intersections(1097, 1123, 0.00000000000000000);
        CHECK(path_is_legal(1097, 1123, path));

        path = find_path_between_intersections(1779, 167, 15.00000000000000000);
        CHECK(path_is_legal(1779, 167, path));

        path = find_path_between_intersections(1112, 1552, 0.00000000000000000);
        CHECK(path_is_legal(1112, 1552, path));

    } //find_path_between_intersections_legality

} //inter_inter_path_func_legality_public

