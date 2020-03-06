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
        rand_lat = std::uniform_real_distribution<double>(51.280006409, 51.699996948);
        rand_lon = std::uniform_real_distribution<double>(-0.599989712, 0.419978589);
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

        path = find_path_between_intersections(4082, 4078, 0.00000000000000000);
        CHECK(path_is_legal(4082, 4078, path));

        path = find_path_between_intersections(12655, 12656, 0.00000000000000000);
        CHECK(path_is_legal(12655, 12656, path));

        path = find_path_between_intersections(14603, 206371, 0.00000000000000000);
        CHECK(path_is_legal(14603, 206371, path));

        path = find_path_between_intersections(14685, 14686, 0.00000000000000000);
        CHECK(path_is_legal(14685, 14686, path));

        path = find_path_between_intersections(22, 18039, 0.00000000000000000);
        CHECK(path_is_legal(22, 18039, path));

        path = find_path_between_intersections(47989, 47990, 0.00000000000000000);
        CHECK(path_is_legal(47989, 47990, path));

        path = find_path_between_intersections(80633, 80632, 0.00000000000000000);
        CHECK(path_is_legal(80633, 80632, path));

        path = find_path_between_intersections(87126, 87125, 0.00000000000000000);
        CHECK(path_is_legal(87126, 87125, path));

        path = find_path_between_intersections(90134, 90133, 0.00000000000000000);
        CHECK(path_is_legal(90134, 90133, path));

        path = find_path_between_intersections(99384, 99392, 0.00000000000000000);
        CHECK(path_is_legal(99384, 99392, path));

        path = find_path_between_intersections(105890, 43579, 0.00000000000000000);
        CHECK(path_is_legal(105890, 43579, path));

        path = find_path_between_intersections(124175, 124174, 0.00000000000000000);
        CHECK(path_is_legal(124175, 124174, path));

        path = find_path_between_intersections(126296, 126295, 0.00000000000000000);
        CHECK(path_is_legal(126296, 126295, path));

        path = find_path_between_intersections(82337, 82332, 0.00000000000000000);
        CHECK(path_is_legal(82337, 82332, path));

        path = find_path_between_intersections(70270, 52570, 0.00000000000000000);
        CHECK(path_is_legal(70270, 52570, path));

        path = find_path_between_intersections(148592, 148470, 0.00000000000000000);
        CHECK(path_is_legal(148592, 148470, path));

        path = find_path_between_intersections(87809, 124938, 0.00000000000000000);
        CHECK(path_is_legal(87809, 124938, path));

        path = find_path_between_intersections(107359, 150944, 0.00000000000000000);
        CHECK(path_is_legal(107359, 150944, path));

        path = find_path_between_intersections(49002, 49004, 0.00000000000000000);
        CHECK(path_is_legal(49002, 49004, path));

        path = find_path_between_intersections(83200, 120947, 0.00000000000000000);
        CHECK(path_is_legal(83200, 120947, path));

        path = find_path_between_intersections(91598, 94027, 0.00000000000000000);
        CHECK(path_is_legal(91598, 94027, path));

        path = find_path_between_intersections(185177, 185184, 0.00000000000000000);
        CHECK(path_is_legal(185177, 185184, path));

        path = find_path_between_intersections(187157, 187160, 0.00000000000000000);
        CHECK(path_is_legal(187157, 187160, path));

        path = find_path_between_intersections(194107, 149996, 0.00000000000000000);
        CHECK(path_is_legal(194107, 149996, path));

        path = find_path_between_intersections(194157, 194164, 0.00000000000000000);
        CHECK(path_is_legal(194157, 194164, path));

        path = find_path_between_intersections(202178, 202176, 0.00000000000000000);
        CHECK(path_is_legal(202178, 202176, path));

        path = find_path_between_intersections(111831, 163037, 0.00000000000000000);
        CHECK(path_is_legal(111831, 163037, path));

        path = find_path_between_intersections(213724, 213720, 0.00000000000000000);
        CHECK(path_is_legal(213724, 213720, path));

        path = find_path_between_intersections(221820, 30038, 0.00000000000000000);
        CHECK(path_is_legal(221820, 30038, path));

        path = find_path_between_intersections(111359, 111360, 0.00000000000000000);
        CHECK(path_is_legal(111359, 111360, path));

    } //find_path_between_intersections_simple_legality

} //inter_inter_simple_path_func_legality_public

