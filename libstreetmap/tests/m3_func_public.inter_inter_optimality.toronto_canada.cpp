#include <random>
#include <unittest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"

#include "unit_test_util.h"
#include "path_verify.h"

using ece297test::relative_error;
using ece297test::path_is_legal;


SUITE(inter_inter_path_func_optimality_public) {
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
    TEST(find_path_between_intersections_optimality) {
        std::vector<StreetSegmentIndex> path;
        path = find_path_between_intersections(13, 51601, 0.00000000000000000);
        CHECK(path_is_legal(13, 51601, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 763.54146517002004657);

        path = find_path_between_intersections(7832, 147775, 0.00000000000000000);
        CHECK(path_is_legal(7832, 147775, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1295.83720692716588019);

        path = find_path_between_intersections(9088, 89818, 0.00000000000000000);
        CHECK(path_is_legal(9088, 89818, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1516.35026187155085609);

        path = find_path_between_intersections(28583, 106836, 0.00000000000000000);
        CHECK(path_is_legal(28583, 106836, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1185.57448977604599349);

        path = find_path_between_intersections(35474, 9037, 15.00000000000000000);
        CHECK(path_is_legal(35474, 9037, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2039.13456574832889601);

        path = find_path_between_intersections(43487, 116429, 15.00000000000000000);
        CHECK(path_is_legal(43487, 116429, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1375.04864604952126683);

        path = find_path_between_intersections(50955, 110047, 15.00000000000000000);
        CHECK(path_is_legal(50955, 110047, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1707.54749848174833460);

        path = find_path_between_intersections(49900, 36147, 0.00000000000000000);
        CHECK(path_is_legal(49900, 36147, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 2261.89408344267258144);

        path = find_path_between_intersections(55780, 7107, 0.00000000000000000);
        CHECK(path_is_legal(55780, 7107, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 687.67922185370525767);

        path = find_path_between_intersections(56077, 56819, 15.00000000000000000);
        CHECK(path_is_legal(56077, 56819, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 989.62419976121555010);

        path = find_path_between_intersections(60411, 57606, 15.00000000000000000);
        CHECK(path_is_legal(60411, 57606, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 945.73136270416239313);

        path = find_path_between_intersections(61505, 85936, 0.00000000000000000);
        CHECK(path_is_legal(61505, 85936, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1309.10200595154128678);

        path = find_path_between_intersections(66440, 110603, 15.00000000000000000);
        CHECK(path_is_legal(66440, 110603, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2260.66525715181433043);

        path = find_path_between_intersections(70817, 48026, 15.00000000000000000);
        CHECK(path_is_legal(70817, 48026, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 3060.59426565269041021);

        path = find_path_between_intersections(76847, 148729, 0.00000000000000000);
        CHECK(path_is_legal(76847, 148729, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 2046.61461955350159769);

        path = find_path_between_intersections(88253, 24143, 15.00000000000000000);
        CHECK(path_is_legal(88253, 24143, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1924.37865474357545281);

        path = find_path_between_intersections(90722, 92846, 0.00000000000000000);
        CHECK(path_is_legal(90722, 92846, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1633.46749773043461573);

        path = find_path_between_intersections(91958, 128368, 0.00000000000000000);
        CHECK(path_is_legal(91958, 128368, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1095.19447568713280816);

        path = find_path_between_intersections(95180, 26717, 0.00000000000000000);
        CHECK(path_is_legal(95180, 26717, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 847.16015990513915312);

        path = find_path_between_intersections(96421, 147042, 15.00000000000000000);
        CHECK(path_is_legal(96421, 147042, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2617.36258178405159924);

        path = find_path_between_intersections(99955, 65531, 0.00000000000000000);
        CHECK(path_is_legal(99955, 65531, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1374.42214669694840268);

        path = find_path_between_intersections(113021, 99914, 0.00000000000000000);
        CHECK(path_is_legal(113021, 99914, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1367.42135770379627502);

        path = find_path_between_intersections(114780, 26866, 15.00000000000000000);
        CHECK(path_is_legal(114780, 26866, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1433.45696938779974516);

        path = find_path_between_intersections(114599, 98300, 15.00000000000000000);
        CHECK(path_is_legal(114599, 98300, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 789.38105571986216091);

        path = find_path_between_intersections(132265, 115112, 0.00000000000000000);
        CHECK(path_is_legal(132265, 115112, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 591.25314635021516096);

        path = find_path_between_intersections(135696, 18428, 15.00000000000000000);
        CHECK(path_is_legal(135696, 18428, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1855.92815311566278069);

        path = find_path_between_intersections(54341, 36647, 0.00000000000000000);
        CHECK(path_is_legal(54341, 36647, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1024.42787825354093911);

        path = find_path_between_intersections(137276, 131450, 0.00000000000000000);
        CHECK(path_is_legal(137276, 131450, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1799.26932467175765851);

        path = find_path_between_intersections(145225, 38918, 15.00000000000000000);
        CHECK(path_is_legal(145225, 38918, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1548.67794030258573912);

        path = find_path_between_intersections(147089, 13864, 15.00000000000000000);
        CHECK(path_is_legal(147089, 13864, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1444.58374402627418931);

    } //find_path_between_intersections_optimality

} //inter_inter_path_func_optimality_public

