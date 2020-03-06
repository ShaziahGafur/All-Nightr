#include <random>
#include <unittest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"

#include "unit_test_util.h"
#include "path_verify.h"

using ece297test::relative_error;
using ece297test::path_is_legal;


SUITE(inter_inter_path_perf_hard_public) {
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
    TEST_FIXTURE(MapFixture, find_path_between_intersections_perf_hard) {
        //Verify Functionality
        std::vector<StreetSegmentIndex> path;
        path = find_path_between_intersections(5408, 124088, 15.00000000000000000);
        CHECK(path_is_legal(5408, 124088, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1701.92496738252202704);

        path = find_path_between_intersections(5670, 41794, 15.00000000000000000);
        CHECK(path_is_legal(5670, 41794, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1516.16595957698814345);

        path = find_path_between_intersections(4085, 145871, 15.00000000000000000);
        CHECK(path_is_legal(4085, 145871, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1642.69754044479009281);

        path = find_path_between_intersections(19221, 139238, 15.00000000000000000);
        CHECK(path_is_legal(19221, 139238, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1621.04694296971888434);

        path = find_path_between_intersections(23387, 24914, 15.00000000000000000);
        CHECK(path_is_legal(23387, 24914, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1695.51428486307599997);

        path = find_path_between_intersections(24868, 60697, 0.00000000000000000);
        CHECK(path_is_legal(24868, 60697, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1224.71760006781573793);

        path = find_path_between_intersections(29402, 150410, 0.00000000000000000);
        CHECK(path_is_legal(29402, 150410, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1069.70860052728176015);

        path = find_path_between_intersections(23256, 88135, 0.00000000000000000);
        CHECK(path_is_legal(23256, 88135, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 801.98924175128922798);

        path = find_path_between_intersections(44457, 56663, 15.00000000000000000);
        CHECK(path_is_legal(44457, 56663, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2897.24278846080778749);

        path = find_path_between_intersections(44260, 73743, 0.00000000000000000);
        CHECK(path_is_legal(44260, 73743, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1381.84844140395466638);

        path = find_path_between_intersections(47427, 39034, 0.00000000000000000);
        CHECK(path_is_legal(47427, 39034, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 2267.45686146441948949);

        path = find_path_between_intersections(48937, 111224, 15.00000000000000000);
        CHECK(path_is_legal(48937, 111224, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2180.53214895249084293);

        path = find_path_between_intersections(52019, 49906, 15.00000000000000000);
        CHECK(path_is_legal(52019, 49906, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2241.54888764446650384);

        path = find_path_between_intersections(54467, 45095, 0.00000000000000000);
        CHECK(path_is_legal(54467, 45095, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 779.73857241401731244);

        path = find_path_between_intersections(57200, 125884, 0.00000000000000000);
        CHECK(path_is_legal(57200, 125884, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1244.44853245123090346);

        path = find_path_between_intersections(57987, 18591, 0.00000000000000000);
        CHECK(path_is_legal(57987, 18591, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 2317.51266031284603741);

        path = find_path_between_intersections(64910, 6880, 0.00000000000000000);
        CHECK(path_is_legal(64910, 6880, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1431.64539412665385498);

        path = find_path_between_intersections(68308, 140634, 0.00000000000000000);
        CHECK(path_is_legal(68308, 140634, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 2091.00547235182466466);

        path = find_path_between_intersections(93193, 119472, 0.00000000000000000);
        CHECK(path_is_legal(93193, 119472, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1014.07152305684030580);

        path = find_path_between_intersections(99005, 4463, 15.00000000000000000);
        CHECK(path_is_legal(99005, 4463, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1254.38745954220348722);

        path = find_path_between_intersections(103153, 136389, 0.00000000000000000);
        CHECK(path_is_legal(103153, 136389, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1543.22409044086953145);

        path = find_path_between_intersections(100649, 16348, 0.00000000000000000);
        CHECK(path_is_legal(100649, 16348, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 920.39766570684139424);

        path = find_path_between_intersections(114863, 94613, 0.00000000000000000);
        CHECK(path_is_legal(114863, 94613, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 2056.95341201005840048);

        path = find_path_between_intersections(122562, 59231, 15.00000000000000000);
        CHECK(path_is_legal(122562, 59231, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1326.63156380565760628);

        path = find_path_between_intersections(122668, 143113, 0.00000000000000000);
        CHECK(path_is_legal(122668, 143113, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1332.19380725596056436);

        path = find_path_between_intersections(127308, 49214, 15.00000000000000000);
        CHECK(path_is_legal(127308, 49214, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1965.73557552280817617);

        path = find_path_between_intersections(129349, 99110, 0.00000000000000000);
        CHECK(path_is_legal(129349, 99110, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1419.26726748623809726);

        path = find_path_between_intersections(133213, 34805, 15.00000000000000000);
        CHECK(path_is_legal(133213, 34805, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 978.31714100955048252);

        path = find_path_between_intersections(142761, 29400, 15.00000000000000000);
        CHECK(path_is_legal(142761, 29400, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1151.57979318246248113);

        path = find_path_between_intersections(147895, 109171, 15.00000000000000000);
        CHECK(path_is_legal(147895, 109171, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1592.89807153977835696);

        //Generate random inputs
        std::vector<IntersectionIndex> intersection_ids1;
        std::vector<IntersectionIndex> intersection_ids2;
        std::vector<double> turn_penalties;
        for(size_t i = 0; i < 475; i++) {
            intersection_ids1.push_back(rand_intersection(rng));
            intersection_ids2.push_back(rand_intersection(rng));
            turn_penalties.push_back(rand_turn_penalty(rng));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(34801);
            std::vector<StreetSegmentIndex> result;
            for(size_t i = 0; i < 475; i++) {
                result = find_path_between_intersections(intersection_ids1[i], intersection_ids2[i], turn_penalties[i]);
            }
        }
    } //find_path_between_intersections_perf_hard

} //inter_inter_path_perf_hard_public

