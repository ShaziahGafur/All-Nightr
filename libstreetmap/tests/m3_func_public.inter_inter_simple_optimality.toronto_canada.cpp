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
    TEST(find_path_between_intersections_simple_optimality) {
        std::vector<StreetSegmentIndex> path;
        path = find_path_between_intersections(13, 184, 0.00000000000000000);
        CHECK(path_is_legal(13, 184, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 16.09953137398885303);

        path = find_path_between_intersections(7832, 19097, 0.00000000000000000);
        CHECK(path_is_legal(7832, 19097, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 8.03824362841634255);

        path = find_path_between_intersections(22635, 22611, 0.00000000000000000);
        CHECK(path_is_legal(22635, 22611, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 13.66910295916416196);

        path = find_path_between_intersections(35474, 70532, 0.00000000000000000);
        CHECK(path_is_legal(35474, 70532, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 6.56836343039024584);

        path = find_path_between_intersections(46252, 46253, 0.00000000000000000);
        CHECK(path_is_legal(46252, 46253, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 4.62426647177113903);

        path = find_path_between_intersections(49900, 49899, 0.00000000000000000);
        CHECK(path_is_legal(49900, 49899, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 4.70250415442914527);

        path = find_path_between_intersections(50955, 51854, 0.00000000000000000);
        CHECK(path_is_legal(50955, 51854, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 18.66353443199661299);

        path = find_path_between_intersections(54341, 54340, 0.00000000000000000);
        CHECK(path_is_legal(54341, 54340, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 8.22738406309047043);

        path = find_path_between_intersections(55780, 55781, 0.00000000000000000);
        CHECK(path_is_legal(55780, 55781, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 8.24763350822279229);

        path = find_path_between_intersections(61505, 61526, 0.00000000000000000);
        CHECK(path_is_legal(61505, 61526, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 8.86009493129532721);

        path = find_path_between_intersections(48026, 66479, 0.00000000000000000);
        CHECK(path_is_legal(48026, 66479, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 18.85224925437045229);

        path = find_path_between_intersections(74187, 74186, 0.00000000000000000);
        CHECK(path_is_legal(74187, 74186, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1.29136787115288021);

        path = find_path_between_intersections(76847, 76845, 0.00000000000000000);
        CHECK(path_is_legal(76847, 76845, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 0.70922848457642262);

        path = find_path_between_intersections(88995, 16823, 0.00000000000000000);
        CHECK(path_is_legal(88995, 16823, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 7.27018390513029455);

        path = find_path_between_intersections(91575, 91576, 0.00000000000000000);
        CHECK(path_is_legal(91575, 91576, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 3.09287355890570881);

        path = find_path_between_intersections(91958, 91960, 0.00000000000000000);
        CHECK(path_is_legal(91958, 91960, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 8.32270283546486134);

        path = find_path_between_intersections(99475, 51415, 0.00000000000000000);
        CHECK(path_is_legal(99475, 51415, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 62.22903301067249515);

        path = find_path_between_intersections(105659, 64711, 0.00000000000000000);
        CHECK(path_is_legal(105659, 64711, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 40.87848008232737840);

        path = find_path_between_intersections(106836, 83417, 0.00000000000000000);
        CHECK(path_is_legal(106836, 83417, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 25.61771078977028537);

        path = find_path_between_intersections(110603, 110602, 0.00000000000000000);
        CHECK(path_is_legal(110603, 110602, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 3.72171124250061647);

        path = find_path_between_intersections(116429, 116436, 0.00000000000000000);
        CHECK(path_is_legal(116429, 116436, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 5.31610178162796743);

        path = find_path_between_intersections(98300, 118521, 0.00000000000000000);
        CHECK(path_is_legal(98300, 118521, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1.56084382977039771);

        path = find_path_between_intersections(65531, 65532, 0.00000000000000000);
        CHECK(path_is_legal(65531, 65532, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 3.04784122461097784);

        path = find_path_between_intersections(9088, 125223, 0.00000000000000000);
        CHECK(path_is_legal(9088, 125223, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 11.48506250211068824);

        path = find_path_between_intersections(126555, 126554, 0.00000000000000000);
        CHECK(path_is_legal(126555, 126554, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1.73268825935359794);

        path = find_path_between_intersections(99939, 81512, 0.00000000000000000);
        CHECK(path_is_legal(99939, 81512, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 11.70982868575279845);

        path = find_path_between_intersections(58215, 131990, 0.00000000000000000);
        CHECK(path_is_legal(58215, 131990, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 22.28322520455714439);

        path = find_path_between_intersections(132265, 132262, 0.00000000000000000);
        CHECK(path_is_legal(132265, 132262, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1.39886955267730939);

        path = find_path_between_intersections(137276, 137278, 0.00000000000000000);
        CHECK(path_is_legal(137276, 137278, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 6.44847057435294957);

        path = find_path_between_intersections(24143, 140703, 0.00000000000000000);
        CHECK(path_is_legal(24143, 140703, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 8.36504421038468493);

    } //find_path_between_intersections_simple_optimality

} //inter_inter_simple_path_func_optimality_public

