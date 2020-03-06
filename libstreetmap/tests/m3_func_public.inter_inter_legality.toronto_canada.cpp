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
        rand_lat = std::uniform_real_distribution<double>(43.479999542, 43.919982910);
        rand_lon = std::uniform_real_distribution<double>(-79.789985657, -79.000000000);
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

        path = find_path_between_intersections(13, 51601, 0.00000000000000000);
        CHECK(path_is_legal(13, 51601, path));

        path = find_path_between_intersections(7832, 147775, 0.00000000000000000);
        CHECK(path_is_legal(7832, 147775, path));

        path = find_path_between_intersections(9088, 89818, 0.00000000000000000);
        CHECK(path_is_legal(9088, 89818, path));

        path = find_path_between_intersections(28583, 106836, 0.00000000000000000);
        CHECK(path_is_legal(28583, 106836, path));

        path = find_path_between_intersections(35474, 9037, 15.00000000000000000);
        CHECK(path_is_legal(35474, 9037, path));

        path = find_path_between_intersections(43487, 116429, 15.00000000000000000);
        CHECK(path_is_legal(43487, 116429, path));

        path = find_path_between_intersections(50955, 110047, 15.00000000000000000);
        CHECK(path_is_legal(50955, 110047, path));

        path = find_path_between_intersections(49900, 36147, 0.00000000000000000);
        CHECK(path_is_legal(49900, 36147, path));

        path = find_path_between_intersections(55780, 7107, 0.00000000000000000);
        CHECK(path_is_legal(55780, 7107, path));

        path = find_path_between_intersections(56077, 56819, 15.00000000000000000);
        CHECK(path_is_legal(56077, 56819, path));

        path = find_path_between_intersections(60411, 57606, 15.00000000000000000);
        CHECK(path_is_legal(60411, 57606, path));

        path = find_path_between_intersections(61505, 85936, 0.00000000000000000);
        CHECK(path_is_legal(61505, 85936, path));

        path = find_path_between_intersections(66440, 110603, 15.00000000000000000);
        CHECK(path_is_legal(66440, 110603, path));

        path = find_path_between_intersections(70817, 48026, 15.00000000000000000);
        CHECK(path_is_legal(70817, 48026, path));

        path = find_path_between_intersections(76847, 148729, 0.00000000000000000);
        CHECK(path_is_legal(76847, 148729, path));

        path = find_path_between_intersections(88253, 24143, 15.00000000000000000);
        CHECK(path_is_legal(88253, 24143, path));

        path = find_path_between_intersections(90722, 92846, 0.00000000000000000);
        CHECK(path_is_legal(90722, 92846, path));

        path = find_path_between_intersections(91958, 128368, 0.00000000000000000);
        CHECK(path_is_legal(91958, 128368, path));

        path = find_path_between_intersections(95180, 26717, 0.00000000000000000);
        CHECK(path_is_legal(95180, 26717, path));

        path = find_path_between_intersections(96421, 147042, 15.00000000000000000);
        CHECK(path_is_legal(96421, 147042, path));

        path = find_path_between_intersections(99955, 65531, 0.00000000000000000);
        CHECK(path_is_legal(99955, 65531, path));

        path = find_path_between_intersections(113021, 99914, 0.00000000000000000);
        CHECK(path_is_legal(113021, 99914, path));

        path = find_path_between_intersections(114780, 26866, 15.00000000000000000);
        CHECK(path_is_legal(114780, 26866, path));

        path = find_path_between_intersections(114599, 98300, 15.00000000000000000);
        CHECK(path_is_legal(114599, 98300, path));

        path = find_path_between_intersections(132265, 115112, 0.00000000000000000);
        CHECK(path_is_legal(132265, 115112, path));

        path = find_path_between_intersections(135696, 18428, 15.00000000000000000);
        CHECK(path_is_legal(135696, 18428, path));

        path = find_path_between_intersections(54341, 36647, 0.00000000000000000);
        CHECK(path_is_legal(54341, 36647, path));

        path = find_path_between_intersections(137276, 131450, 0.00000000000000000);
        CHECK(path_is_legal(137276, 131450, path));

        path = find_path_between_intersections(145225, 38918, 15.00000000000000000);
        CHECK(path_is_legal(145225, 38918, path));

        path = find_path_between_intersections(147089, 13864, 15.00000000000000000);
        CHECK(path_is_legal(147089, 13864, path));

    } //find_path_between_intersections_legality

} //inter_inter_path_func_legality_public

