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
    TEST(find_path_between_intersections_optimality) {
        std::vector<StreetSegmentIndex> path;
        path = find_path_between_intersections(0, 624, 0.00000000000000000);
        CHECK(path_is_legal(0, 624, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 184.53413527789362547);

        path = find_path_between_intersections(94, 1787, 0.00000000000000000);
        CHECK(path_is_legal(94, 1787, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 364.03578286108222528);

        path = find_path_between_intersections(109, 1086, 0.00000000000000000);
        CHECK(path_is_legal(109, 1086, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 75.46210225151276063);

        path = find_path_between_intersections(429, 109, 15.00000000000000000);
        CHECK(path_is_legal(429, 109, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 73.95200282031989047);

        path = find_path_between_intersections(525, 1408, 15.00000000000000000);
        CHECK(path_is_legal(525, 1408, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 402.96520339743312888);

        path = find_path_between_intersections(345, 1292, 0.00000000000000000);
        CHECK(path_is_legal(345, 1292, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 93.57280537799879028);

        path = find_path_between_intersections(616, 1331, 15.00000000000000000);
        CHECK(path_is_legal(616, 1331, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 117.43896335042867918);

        path = find_path_between_intersections(657, 443, 0.00000000000000000);
        CHECK(path_is_legal(657, 443, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 304.37446729292275904);

        path = find_path_between_intersections(678, 687, 15.00000000000000000);
        CHECK(path_is_legal(678, 687, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 127.21951958615464662);

        path = find_path_between_intersections(674, 85, 0.00000000000000000);
        CHECK(path_is_legal(674, 85, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 216.63095832769008098);

        path = find_path_between_intersections(730, 696, 15.00000000000000000);
        CHECK(path_is_legal(730, 696, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 263.75714931122001872);

        path = find_path_between_intersections(743, 1039, 0.00000000000000000);
        CHECK(path_is_legal(743, 1039, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 213.25522176598875035);

        path = find_path_between_intersections(803, 1337, 15.00000000000000000);
        CHECK(path_is_legal(803, 1337, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 471.57487266586429087);

        path = find_path_between_intersections(929, 1798, 0.00000000000000000);
        CHECK(path_is_legal(929, 1798, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 552.84199454841586885);

        path = find_path_between_intersections(1067, 292, 15.00000000000000000);
        CHECK(path_is_legal(1067, 292, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 399.73781593447438354);

        path = find_path_between_intersections(1151, 323, 0.00000000000000000);
        CHECK(path_is_legal(1151, 323, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 295.93536490380353143);

        path = find_path_between_intersections(1208, 792, 0.00000000000000000);
        CHECK(path_is_legal(1208, 792, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 284.24641135413583015);

        path = find_path_between_intersections(603, 437, 0.00000000000000000);
        CHECK(path_is_legal(603, 437, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 147.82363768756860622);

        path = find_path_between_intersections(856, 580, 15.00000000000000000);
        CHECK(path_is_legal(856, 580, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 322.83944142651807852);

        path = find_path_between_intersections(1386, 1188, 15.00000000000000000);
        CHECK(path_is_legal(1386, 1188, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 310.62645775750098665);

        path = find_path_between_intersections(1388, 324, 15.00000000000000000);
        CHECK(path_is_legal(1388, 324, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 418.40542506731833328);

        path = find_path_between_intersections(1599, 1392, 0.00000000000000000);
        CHECK(path_is_legal(1599, 1392, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 437.92775207592012521);

        path = find_path_between_intersections(1641, 222, 15.00000000000000000);
        CHECK(path_is_legal(1641, 222, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 559.67035234838601809);

        path = find_path_between_intersections(1660, 1589, 0.00000000000000000);
        CHECK(path_is_legal(1660, 1589, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 217.28392908023306518);

        path = find_path_between_intersections(1166, 1778, 15.00000000000000000);
        CHECK(path_is_legal(1166, 1778, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 395.16990298761891154);

        path = find_path_between_intersections(1756, 470, 15.00000000000000000);
        CHECK(path_is_legal(1756, 470, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 358.27172384203981892);

        path = find_path_between_intersections(1367, 1208, 0.00000000000000000);
        CHECK(path_is_legal(1367, 1208, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 106.19894556814341513);

        path = find_path_between_intersections(1097, 1123, 0.00000000000000000);
        CHECK(path_is_legal(1097, 1123, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 429.69166884645227356);

        path = find_path_between_intersections(1779, 167, 15.00000000000000000);
        CHECK(path_is_legal(1779, 167, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 596.02669650609357177);

        path = find_path_between_intersections(1112, 1552, 0.00000000000000000);
        CHECK(path_is_legal(1112, 1552, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 145.36442738960181487);

    } //find_path_between_intersections_optimality

} //inter_inter_path_func_optimality_public

