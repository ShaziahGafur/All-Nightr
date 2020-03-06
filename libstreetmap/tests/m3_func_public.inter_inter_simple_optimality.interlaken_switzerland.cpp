#include <random>
#include <unittest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"

#include "unit_test_util.h"
#include "path_verify.h"

using ece297test::relative_error;
using ece297test::path_is_legal;


SUITE(inter_inter_simple_path_func_optimality_public) {
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
    TEST(find_path_between_intersections_simple_optimality) {
        std::vector<StreetSegmentIndex> path;
        path = find_path_between_intersections(109, 110, 0.00000000000000000);
        CHECK(path_is_legal(109, 110, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 4.97408726599182316);

        path = find_path_between_intersections(167, 188, 0.00000000000000000);
        CHECK(path_is_legal(167, 188, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 10.43367237850913121);

        path = find_path_between_intersections(94, 297, 0.00000000000000000);
        CHECK(path_is_legal(94, 297, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 3.38744006564422007);

        path = find_path_between_intersections(323, 40, 0.00000000000000000);
        CHECK(path_is_legal(323, 40, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 2.76108421684468874);

        path = find_path_between_intersections(622, 620, 0.00000000000000000);
        CHECK(path_is_legal(622, 620, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 9.88288284425505381);

        path = find_path_between_intersections(643, 642, 0.00000000000000000);
        CHECK(path_is_legal(643, 642, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1.01957793171374367);

        path = find_path_between_intersections(657, 658, 0.00000000000000000);
        CHECK(path_is_legal(657, 658, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 14.19555985336321058);

        path = find_path_between_intersections(743, 678, 0.00000000000000000);
        CHECK(path_is_legal(743, 678, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 4.18663290925014930);

        path = find_path_between_intersections(682, 749, 0.00000000000000000);
        CHECK(path_is_legal(682, 749, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 12.45599705321318496);

        path = find_path_between_intersections(324, 326, 0.00000000000000000);
        CHECK(path_is_legal(324, 326, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 28.40853526085309255);

        path = find_path_between_intersections(850, 848, 0.00000000000000000);
        CHECK(path_is_legal(850, 848, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 13.40813182486119892);

        path = find_path_between_intersections(885, 903, 0.00000000000000000);
        CHECK(path_is_legal(885, 903, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 23.32163948485609239);

        path = find_path_between_intersections(972, 1187, 0.00000000000000000);
        CHECK(path_is_legal(972, 1187, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 7.60432645468576673);

        path = find_path_between_intersections(489, 1035, 0.00000000000000000);
        CHECK(path_is_legal(489, 1035, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 2.12316639307352206);

        path = find_path_between_intersections(1331, 460, 0.00000000000000000);
        CHECK(path_is_legal(1331, 460, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 8.55609633208028164);

        path = find_path_between_intersections(579, 798, 0.00000000000000000);
        CHECK(path_is_legal(579, 798, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 16.54405005193099143);

        path = find_path_between_intersections(1214, 1425, 0.00000000000000000);
        CHECK(path_is_legal(1214, 1425, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 3.68447717250071394);

        path = find_path_between_intersections(1188, 975, 0.00000000000000000);
        CHECK(path_is_legal(1188, 975, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 3.14695616595945982);

        path = find_path_between_intersections(1459, 1458, 0.00000000000000000);
        CHECK(path_is_legal(1459, 1458, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 6.49392658218810404);

        path = find_path_between_intersections(1453, 1732, 0.00000000000000000);
        CHECK(path_is_legal(1453, 1732, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 5.92399381612661990);

        path = find_path_between_intersections(1513, 1503, 0.00000000000000000);
        CHECK(path_is_legal(1513, 1503, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 7.23456229488296199);

        path = find_path_between_intersections(945, 41, 0.00000000000000000);
        CHECK(path_is_legal(945, 41, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 4.57169156200308180);

        path = find_path_between_intersections(1589, 1588, 0.00000000000000000);
        CHECK(path_is_legal(1589, 1588, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 16.85501184302877675);

        path = find_path_between_intersections(1599, 1602, 0.00000000000000000);
        CHECK(path_is_legal(1599, 1602, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1.01695245648223032);

        path = find_path_between_intersections(1641, 952, 0.00000000000000000);
        CHECK(path_is_legal(1641, 952, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 13.16099621162177868);

        path = find_path_between_intersections(0, 1105, 0.00000000000000000);
        CHECK(path_is_legal(0, 1105, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 37.38523869222353113);

        path = find_path_between_intersections(1292, 284, 0.00000000000000000);
        CHECK(path_is_legal(1292, 284, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 21.46756776041117831);

        path = find_path_between_intersections(1721, 1722, 0.00000000000000000);
        CHECK(path_is_legal(1721, 1722, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 3.76357520209430030);

        path = find_path_between_intersections(150, 295, 0.00000000000000000);
        CHECK(path_is_legal(150, 295, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 3.54359852039255374);

        path = find_path_between_intersections(1798, 1800, 0.00000000000000000);
        CHECK(path_is_legal(1798, 1800, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 2.55217989062672590);

    } //find_path_between_intersections_simple_optimality

} //inter_inter_simple_path_func_optimality_public

