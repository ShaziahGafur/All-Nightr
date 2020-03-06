#include <random>
#include <unittest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"

#include "unit_test_util.h"
#include "path_verify.h"

using ece297test::relative_error;
using ece297test::path_is_legal;


SUITE(inter_inter_path_perf_very_hard_public) {
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
    TEST_FIXTURE(MapFixture, find_path_between_intersections_perf_very_hard) {
        //Verify Functionality
        std::vector<StreetSegmentIndex> path;
        path = find_path_between_intersections(20306, 114839, 15.00000000000000000);
        CHECK(path_is_legal(20306, 114839, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1814.02382353014627370);

        path = find_path_between_intersections(26426, 122785, 15.00000000000000000);
        CHECK(path_is_legal(26426, 122785, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 842.19453482018275281);

        path = find_path_between_intersections(30548, 28959, 0.00000000000000000);
        CHECK(path_is_legal(30548, 28959, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1693.94376720332888908);

        path = find_path_between_intersections(77792, 230094, 0.00000000000000000);
        CHECK(path_is_legal(77792, 230094, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 951.98372906014594719);

        path = find_path_between_intersections(70888, 126927, 0.00000000000000000);
        CHECK(path_is_legal(70888, 126927, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 2401.92467675399120708);

        path = find_path_between_intersections(82912, 229014, 0.00000000000000000);
        CHECK(path_is_legal(82912, 229014, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1211.33147027561585674);

        path = find_path_between_intersections(108056, 1977, 0.00000000000000000);
        CHECK(path_is_legal(108056, 1977, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 4109.59797943571356882);

        path = find_path_between_intersections(108797, 3920, 15.00000000000000000);
        CHECK(path_is_legal(108797, 3920, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1124.56848532828325915);

        path = find_path_between_intersections(117830, 131066, 15.00000000000000000);
        CHECK(path_is_legal(117830, 131066, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2966.59181355467489993);

        path = find_path_between_intersections(133477, 131055, 0.00000000000000000);
        CHECK(path_is_legal(133477, 131055, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 2057.28651891193749179);

        path = find_path_between_intersections(57228, 176324, 15.00000000000000000);
        CHECK(path_is_legal(57228, 176324, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2584.71574122354059000);

        path = find_path_between_intersections(16239, 169807, 15.00000000000000000);
        CHECK(path_is_legal(16239, 169807, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1672.30656895898891889);

        path = find_path_between_intersections(143780, 100677, 15.00000000000000000);
        CHECK(path_is_legal(143780, 100677, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1775.64766426424580459);

        path = find_path_between_intersections(165638, 94902, 0.00000000000000000);
        CHECK(path_is_legal(165638, 94902, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1995.79999951549098114);

        path = find_path_between_intersections(167379, 14080, 0.00000000000000000);
        CHECK(path_is_legal(167379, 14080, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 2289.91843969650381041);

        path = find_path_between_intersections(158520, 197068, 0.00000000000000000);
        CHECK(path_is_legal(158520, 197068, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 2739.74122106306003843);

        path = find_path_between_intersections(174342, 68393, 0.00000000000000000);
        CHECK(path_is_legal(174342, 68393, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 3014.01177936926296752);

        path = find_path_between_intersections(176379, 74277, 0.00000000000000000);
        CHECK(path_is_legal(176379, 74277, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 2241.34314697152149165);

        path = find_path_between_intersections(182034, 177776, 15.00000000000000000);
        CHECK(path_is_legal(182034, 177776, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1307.33045029746199361);

        path = find_path_between_intersections(195669, 152715, 0.00000000000000000);
        CHECK(path_is_legal(195669, 152715, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1909.98628154097946208);

        path = find_path_between_intersections(200350, 80452, 15.00000000000000000);
        CHECK(path_is_legal(200350, 80452, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2152.67639158704287183);

        path = find_path_between_intersections(202323, 225770, 15.00000000000000000);
        CHECK(path_is_legal(202323, 225770, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2688.35763165227672289);

        path = find_path_between_intersections(290, 28372, 15.00000000000000000);
        CHECK(path_is_legal(290, 28372, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2725.07672924051666996);

        path = find_path_between_intersections(162901, 112777, 0.00000000000000000);
        CHECK(path_is_legal(162901, 112777, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 2629.48887324674296906);

        path = find_path_between_intersections(215898, 200250, 0.00000000000000000);
        CHECK(path_is_legal(215898, 200250, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1746.32137706643879937);

        path = find_path_between_intersections(40554, 104111, 0.00000000000000000);
        CHECK(path_is_legal(40554, 104111, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 3390.81949468097400313);

        path = find_path_between_intersections(113997, 204647, 0.00000000000000000);
        CHECK(path_is_legal(113997, 204647, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 843.98568431539013091);

        path = find_path_between_intersections(146641, 157326, 15.00000000000000000);
        CHECK(path_is_legal(146641, 157326, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1606.35241840114645129);

        path = find_path_between_intersections(150789, 105922, 15.00000000000000000);
        CHECK(path_is_legal(150789, 105922, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2698.61415682820233997);

        path = find_path_between_intersections(92594, 104714, 15.00000000000000000);
        CHECK(path_is_legal(92594, 104714, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1543.95034154624363509);

        //Generate random inputs
        std::vector<IntersectionIndex> intersection_ids1;
        std::vector<IntersectionIndex> intersection_ids2;
        std::vector<double> turn_penalties;
        for(size_t i = 0; i < 100; i++) {
            intersection_ids1.push_back(rand_intersection(rng));
            intersection_ids2.push_back(rand_intersection(rng));
            turn_penalties.push_back(rand_turn_penalty(rng));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(9523);
            std::vector<StreetSegmentIndex> result;
            for(size_t i = 0; i < 100; i++) {
                result = find_path_between_intersections(intersection_ids1[i], intersection_ids2[i], turn_penalties[i]);
            }
        }
    } //find_path_between_intersections_perf_very_hard

} //inter_inter_path_perf_very_hard_public

