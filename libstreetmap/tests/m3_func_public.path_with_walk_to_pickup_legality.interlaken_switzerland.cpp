#include <random>
#include <unittest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"

#include "unit_test_util.h"
#include "path_verify.h"

using ece297test::relative_error;
using ece297test::path_is_legal;


SUITE(path_with_walk_to_pickup_legality_public) {
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
    TEST(find_path_with_walk_to_pick_up_legality) {
        std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> path;

        path = find_path_with_walk_to_pick_up(109, 1086, 22.58759087598575732, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(109, 1086, 22.58759087598575732, 1.39999999999999991, 0.00000000000000000, path));

        path = find_path_with_walk_to_pick_up(657, 443, 24.74049806129059093, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(657, 443, 24.74049806129059093, 1.39999999999999991, 0.00000000000000000, path));

        path = find_path_with_walk_to_pick_up(672, 671, 17.24833628218366144, 1.39999999999999991, 62.71106131057546662);
        CHECK(path_is_legal(672, 671, 17.24833628218366144, 1.39999999999999991, 62.71106131057546662, path));

        path = find_path_with_walk_to_pick_up(674, 85, 17.38266612778162568, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(674, 85, 17.38266612778162568, 1.39999999999999991, 0.00000000000000000, path));

        path = find_path_with_walk_to_pick_up(743, 1039, 18.40129796266718643, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(743, 1039, 18.40129796266718643, 1.39999999999999991, 0.00000000000000000, path));

        path = find_path_with_walk_to_pick_up(1045, 1535, 17.90710847337793510, 1.35083152302336185, 29.25712996453992432);
        CHECK(path_is_legal(1045, 1535, 17.90710847337793510, 1.35083152302336185, 29.25712996453992432, path));

        path = find_path_with_walk_to_pick_up(1291, 979, 18.12706786144983084, 0.84730791379958814, 48.28180620021811364);
        CHECK(path_is_legal(1291, 979, 18.12706786144983084, 0.84730791379958814, 48.28180620021811364, path));

        path = find_path_with_walk_to_pick_up(1338, 254, 16.89378956086656913, 3.75741376740092914, 21.83751649926404070);
        CHECK(path_is_legal(1338, 254, 16.89378956086656913, 3.75741376740092914, 21.83751649926404070, path));

        path = find_path_with_walk_to_pick_up(1527, 1794, 22.99138211696489620, 3.47174371657243430, 141.96618588894111213);
        CHECK(path_is_legal(1527, 1794, 22.99138211696489620, 3.47174371657243430, 141.96618588894111213, path));

        path = find_path_with_walk_to_pick_up(1571, 1630, 23.92478294037437792, 2.67689528241594932, 0.37200106382929987);
        CHECK(path_is_legal(1571, 1630, 23.92478294037437792, 2.67689528241594932, 0.37200106382929987, path));

        path = find_path_with_walk_to_pick_up(1660, 1589, 24.80338191831117456, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(1660, 1589, 24.80338191831117456, 1.39999999999999991, 0.00000000000000000, path));

        path = find_path_with_walk_to_pick_up(133, 811, 23.58207964209191232, 3.87890825210871171, 8.99964276409176378);
        CHECK(path_is_legal(133, 811, 23.58207964209191232, 3.87890825210871171, 8.99964276409176378, path));

        path = find_path_with_walk_to_pick_up(1097, 1123, 22.04203310234851898, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(1097, 1123, 22.04203310234851898, 1.39999999999999991, 0.00000000000000000, path));

        path = find_path_with_walk_to_pick_up(62, 1790, 21.23583356131032573, 4.51999030988997230, 6.87390550720356419);
        CHECK(path_is_legal(62, 1790, 21.23583356131032573, 4.51999030988997230, 6.87390550720356419, path));

        path = find_path_with_walk_to_pick_up(30, 28, 16.77524564084403735, 1.39999999999999991, 72.01947697334534837);
        CHECK(path_is_legal(30, 28, 16.77524564084403735, 1.39999999999999991, 72.01947697334534837, path));

        path = find_path_with_walk_to_pick_up(125, 995, 15.32586511173930077, 3.98617106755402695, 167.53579668017286508);
        CHECK(path_is_legal(125, 995, 15.32586511173930077, 3.98617106755402695, 167.53579668017286508, path));

        path = find_path_with_walk_to_pick_up(532, 1290, 22.52695215707312926, 2.95114699275022563, 290.77265871815563969);
        CHECK(path_is_legal(532, 1290, 22.52695215707312926, 2.95114699275022563, 290.77265871815563969, path));

        path = find_path_with_walk_to_pick_up(238, 237, 16.21467047900597436, 1.39999999999999991, 177.06495415913934721);
        CHECK(path_is_legal(238, 237, 16.21467047900597436, 1.39999999999999991, 177.06495415913934721, path));

        path = find_path_with_walk_to_pick_up(660, 1062, 20.21819737661307315, 2.87563263631685384, 65.42661053311175579);
        CHECK(path_is_legal(660, 1062, 20.21819737661307315, 2.87563263631685384, 65.42661053311175579, path));

        path = find_path_with_walk_to_pick_up(768, 1407, 22.95045568973362649, 2.43549633248642339, 83.25755882125727680);
        CHECK(path_is_legal(768, 1407, 22.95045568973362649, 2.43549633248642339, 83.25755882125727680, path));

        path = find_path_with_walk_to_pick_up(1163, 380, 22.86015171952646341, 3.17371570451583729, 203.21290661051020265);
        CHECK(path_is_legal(1163, 380, 22.86015171952646341, 3.17371570451583729, 203.21290661051020265, path));

        path = find_path_with_walk_to_pick_up(1235, 706, 24.70388500754370043, 3.39189903624434219, 228.24059861675877414);
        CHECK(path_is_legal(1235, 706, 24.70388500754370043, 3.39189903624434219, 228.24059861675877414, path));

        path = find_path_with_walk_to_pick_up(1291, 238, 21.66117254177195406, 1.39999999999999991, 80.13940665127076102);
        CHECK(path_is_legal(1291, 238, 21.66117254177195406, 1.39999999999999991, 80.13940665127076102, path));

        path = find_path_with_walk_to_pick_up(1337, 589, 23.46132436906174945, 1.39999999999999991, 113.91292612407590923);
        CHECK(path_is_legal(1337, 589, 23.46132436906174945, 1.39999999999999991, 113.91292612407590923, path));

        path = find_path_with_walk_to_pick_up(1408, 1518, 24.96924697045825781, 4.35138221449282003, 238.06695998103199940);
        CHECK(path_is_legal(1408, 1518, 24.96924697045825781, 4.35138221449282003, 238.06695998103199940, path));

        path = find_path_with_walk_to_pick_up(1647, 1287, 21.15430917549949541, 2.16972952256718621, 64.85196960124248733);
        CHECK(path_is_legal(1647, 1287, 21.15430917549949541, 2.16972952256718621, 64.85196960124248733, path));

        path = find_path_with_walk_to_pick_up(1701, 1702, 23.29108811416090674, 1.39999999999999991, 82.35532619884476446);
        CHECK(path_is_legal(1701, 1702, 23.29108811416090674, 1.39999999999999991, 82.35532619884476446, path));

        path = find_path_with_walk_to_pick_up(1736, 1581, 20.80948077349938785, 4.78467092166508046, 254.30763637150070622);
        CHECK(path_is_legal(1736, 1581, 20.80948077349938785, 4.78467092166508046, 254.30763637150070622, path));

        path = find_path_with_walk_to_pick_up(360, 1582, 22.29047040070515351, 3.22579846584059915, 170.95479495904675105);
        CHECK(path_is_legal(360, 1582, 22.29047040070515351, 3.22579846584059915, 170.95479495904675105, path));

        path = find_path_with_walk_to_pick_up(1803, 524, 16.36752755822002214, 3.21385185254518113, 272.54241231542823698);
        CHECK(path_is_legal(1803, 524, 16.36752755822002214, 3.21385185254518113, 272.54241231542823698, path));

    } //find_path_with_walk_to_pick_up_legality

} //path_with_walk_to_pickup_legality_public

