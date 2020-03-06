#include <random>
#include <unittest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"

#include "unit_test_util.h"
#include "path_verify.h"

using ece297test::relative_error;
using ece297test::path_is_legal;


SUITE(inter_inter_path_perf_medium_public) {
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
    TEST_FIXTURE(MapFixture, find_path_between_intersections_perf_medium) {
        //Verify Functionality
        std::vector<StreetSegmentIndex> path;
        path = find_path_between_intersections(6319, 90906, 0.00000000000000000);
        CHECK(path_is_legal(6319, 90906, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 929.70319301363565501);

        path = find_path_between_intersections(13925, 92061, 15.00000000000000000);
        CHECK(path_is_legal(13925, 92061, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1744.88643600883870022);

        path = find_path_between_intersections(14699, 23812, 0.00000000000000000);
        CHECK(path_is_legal(14699, 23812, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1137.05304661975537783);

        path = find_path_between_intersections(19503, 83781, 0.00000000000000000);
        CHECK(path_is_legal(19503, 83781, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 647.07555080566578454);

        path = find_path_between_intersections(26019, 108771, 15.00000000000000000);
        CHECK(path_is_legal(26019, 108771, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1587.09067727733122410);

        path = find_path_between_intersections(32704, 122317, 0.00000000000000000);
        CHECK(path_is_legal(32704, 122317, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1269.02123621476903281);

        path = find_path_between_intersections(33151, 10292, 0.00000000000000000);
        CHECK(path_is_legal(33151, 10292, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1137.70982310996532760);

        path = find_path_between_intersections(41010, 86066, 15.00000000000000000);
        CHECK(path_is_legal(41010, 86066, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1823.69759077499588784);

        path = find_path_between_intersections(44587, 81264, 15.00000000000000000);
        CHECK(path_is_legal(44587, 81264, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1254.71599018524466373);

        path = find_path_between_intersections(44353, 41850, 15.00000000000000000);
        CHECK(path_is_legal(44353, 41850, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 987.10793969290659788);

        path = find_path_between_intersections(46213, 138966, 0.00000000000000000);
        CHECK(path_is_legal(46213, 138966, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1812.27517321234518022);

        path = find_path_between_intersections(24099, 131786, 0.00000000000000000);
        CHECK(path_is_legal(24099, 131786, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1534.60222326548569072);

        path = find_path_between_intersections(46876, 142950, 15.00000000000000000);
        CHECK(path_is_legal(46876, 142950, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2557.55374337444527555);

        path = find_path_between_intersections(48237, 144974, 15.00000000000000000);
        CHECK(path_is_legal(48237, 144974, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1909.98477647687241188);

        path = find_path_between_intersections(48658, 10059, 0.00000000000000000);
        CHECK(path_is_legal(48658, 10059, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1416.29761480571687571);

        path = find_path_between_intersections(60140, 24681, 0.00000000000000000);
        CHECK(path_is_legal(60140, 24681, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1570.24308321327930571);

        path = find_path_between_intersections(64136, 119431, 0.00000000000000000);
        CHECK(path_is_legal(64136, 119431, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1397.63031230040019182);

        path = find_path_between_intersections(64439, 23377, 15.00000000000000000);
        CHECK(path_is_legal(64439, 23377, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 736.03401754550793612);

        path = find_path_between_intersections(74202, 67789, 15.00000000000000000);
        CHECK(path_is_legal(74202, 67789, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 997.06976133017178654);

        path = find_path_between_intersections(82810, 44651, 0.00000000000000000);
        CHECK(path_is_legal(82810, 44651, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1744.15008882672077561);

        path = find_path_between_intersections(91486, 103421, 15.00000000000000000);
        CHECK(path_is_legal(91486, 103421, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1572.36775967012363253);

        path = find_path_between_intersections(95885, 48270, 0.00000000000000000);
        CHECK(path_is_legal(95885, 48270, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 992.89157126926932051);

        path = find_path_between_intersections(101270, 129399, 0.00000000000000000);
        CHECK(path_is_legal(101270, 129399, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 2183.22296202670304410);

        path = find_path_between_intersections(110721, 117594, 0.00000000000000000);
        CHECK(path_is_legal(110721, 117594, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1362.87255203742006415);

        path = find_path_between_intersections(124388, 32298, 0.00000000000000000);
        CHECK(path_is_legal(124388, 32298, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 561.17826529831415883);

        path = find_path_between_intersections(100474, 97875, 15.00000000000000000);
        CHECK(path_is_legal(100474, 97875, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1280.81743697605452326);

        path = find_path_between_intersections(128701, 88860, 15.00000000000000000);
        CHECK(path_is_legal(128701, 88860, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 715.26612810486813032);

        path = find_path_between_intersections(30910, 1410, 15.00000000000000000);
        CHECK(path_is_legal(30910, 1410, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 645.93965470801890660);

        path = find_path_between_intersections(136124, 84252, 15.00000000000000000);
        CHECK(path_is_legal(136124, 84252, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1694.15152633924071779);

        path = find_path_between_intersections(138577, 133400, 0.00000000000000000);
        CHECK(path_is_legal(138577, 133400, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1772.53707655479502137);

        //Generate random inputs
        std::vector<IntersectionIndex> intersection_ids1;
        std::vector<IntersectionIndex> intersection_ids2;
        std::vector<double> turn_penalties;
        for(size_t i = 0; i < 150; i++) {
            intersection_ids1.push_back(rand_intersection(rng));
            intersection_ids2.push_back(rand_intersection(rng));
            turn_penalties.push_back(rand_turn_penalty(rng));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(21497);
            std::vector<StreetSegmentIndex> result;
            for(size_t i = 0; i < 150; i++) {
                result = find_path_between_intersections(intersection_ids1[i], intersection_ids2[i], turn_penalties[i]);
            }
        }
    } //find_path_between_intersections_perf_medium

} //inter_inter_path_perf_medium_public

