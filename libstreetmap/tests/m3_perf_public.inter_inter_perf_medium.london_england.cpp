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
    TEST_FIXTURE(MapFixture, find_path_between_intersections_perf_medium) {
        //Verify Functionality
        std::vector<StreetSegmentIndex> path;
        path = find_path_between_intersections(33539, 48507, 0.00000000000000000);
        CHECK(path_is_legal(33539, 48507, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 2364.36018379237430054);

        path = find_path_between_intersections(51244, 102666, 0.00000000000000000);
        CHECK(path_is_legal(51244, 102666, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1470.02088658992943238);

        path = find_path_between_intersections(67297, 60432, 15.00000000000000000);
        CHECK(path_is_legal(67297, 60432, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2377.77089271674458359);

        path = find_path_between_intersections(68434, 65486, 15.00000000000000000);
        CHECK(path_is_legal(68434, 65486, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2572.90354773974922864);

        path = find_path_between_intersections(109805, 116735, 15.00000000000000000);
        CHECK(path_is_legal(109805, 116735, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 4720.99824392131176864);

        path = find_path_between_intersections(109842, 199663, 0.00000000000000000);
        CHECK(path_is_legal(109842, 199663, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1366.35517611163481888);

        path = find_path_between_intersections(116826, 224737, 15.00000000000000000);
        CHECK(path_is_legal(116826, 224737, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 3019.71572301532751226);

        path = find_path_between_intersections(162105, 141762, 15.00000000000000000);
        CHECK(path_is_legal(162105, 141762, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 918.17479422277529011);

        path = find_path_between_intersections(163296, 35783, 15.00000000000000000);
        CHECK(path_is_legal(163296, 35783, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1862.29032593204397017);

        path = find_path_between_intersections(199195, 230522, 0.00000000000000000);
        CHECK(path_is_legal(199195, 230522, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 2563.85218040465815648);

        path = find_path_between_intersections(203253, 223434, 0.00000000000000000);
        CHECK(path_is_legal(203253, 223434, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1806.29956156218668184);

        path = find_path_between_intersections(175301, 20991, 0.00000000000000000);
        CHECK(path_is_legal(175301, 20991, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 472.00005057952955667);

        path = find_path_between_intersections(127132, 71129, 0.00000000000000000);
        CHECK(path_is_legal(127132, 71129, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 862.85473438238648214);

        path = find_path_between_intersections(56178, 221441, 15.00000000000000000);
        CHECK(path_is_legal(56178, 221441, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 923.47835916418227953);

        path = find_path_between_intersections(111597, 50071, 0.00000000000000000);
        CHECK(path_is_legal(111597, 50071, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1814.57095864146117492);

        path = find_path_between_intersections(164195, 55117, 0.00000000000000000);
        CHECK(path_is_legal(164195, 55117, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1016.12935217528786325);

        path = find_path_between_intersections(182730, 229273, 0.00000000000000000);
        CHECK(path_is_legal(182730, 229273, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1461.03627621911186907);

        path = find_path_between_intersections(186414, 66324, 0.00000000000000000);
        CHECK(path_is_legal(186414, 66324, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 878.42424578512407152);

        path = find_path_between_intersections(227291, 59664, 0.00000000000000000);
        CHECK(path_is_legal(227291, 59664, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1222.99443886632275280);

        path = find_path_between_intersections(83553, 78426, 0.00000000000000000);
        CHECK(path_is_legal(83553, 78426, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 2939.36498022753130499);

        path = find_path_between_intersections(195225, 34121, 0.00000000000000000);
        CHECK(path_is_legal(195225, 34121, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 2298.41108490748911208);

        path = find_path_between_intersections(228788, 4467, 15.00000000000000000);
        CHECK(path_is_legal(228788, 4467, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 3969.52505227427127465);

        path = find_path_between_intersections(230574, 150660, 15.00000000000000000);
        CHECK(path_is_legal(230574, 150660, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2752.30325226069908240);

        path = find_path_between_intersections(231435, 50911, 0.00000000000000000);
        CHECK(path_is_legal(231435, 50911, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1790.35660922461875089);

        path = find_path_between_intersections(171031, 76664, 15.00000000000000000);
        CHECK(path_is_legal(171031, 76664, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1210.36888363801881496);

        path = find_path_between_intersections(89829, 235147, 0.00000000000000000);
        CHECK(path_is_legal(89829, 235147, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1508.60320716281853493);

        path = find_path_between_intersections(65252, 153333, 15.00000000000000000);
        CHECK(path_is_legal(65252, 153333, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2203.03071061612581616);

        path = find_path_between_intersections(16684, 79011, 15.00000000000000000);
        CHECK(path_is_legal(16684, 79011, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2472.57976514027632220);

        path = find_path_between_intersections(237994, 172575, 15.00000000000000000);
        CHECK(path_is_legal(237994, 172575, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2583.10116528683374781);

        path = find_path_between_intersections(184207, 162765, 15.00000000000000000);
        CHECK(path_is_legal(184207, 162765, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2151.76069467413253733);

        //Generate random inputs
        std::vector<IntersectionIndex> intersection_ids1;
        std::vector<IntersectionIndex> intersection_ids2;
        std::vector<double> turn_penalties;
        for(size_t i = 0; i < 40; i++) {
            intersection_ids1.push_back(rand_intersection(rng));
            intersection_ids2.push_back(rand_intersection(rng));
            turn_penalties.push_back(rand_turn_penalty(rng));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(7690);
            std::vector<StreetSegmentIndex> result;
            for(size_t i = 0; i < 40; i++) {
                result = find_path_between_intersections(intersection_ids1[i], intersection_ids2[i], turn_penalties[i]);
            }
        }
    } //find_path_between_intersections_perf_medium

} //inter_inter_path_perf_medium_public

