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
    TEST(find_path_between_intersections_optimality) {
        std::vector<StreetSegmentIndex> path;
        path = find_path_between_intersections(12655, 238784, 0.00000000000000000);
        CHECK(path_is_legal(12655, 238784, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1564.69113216417508738);

        path = find_path_between_intersections(14685, 145134, 0.00000000000000000);
        CHECK(path_is_legal(14685, 145134, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 2874.03004123251321289);

        path = find_path_between_intersections(22, 83381, 0.00000000000000000);
        CHECK(path_is_legal(22, 83381, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1262.67153962793668143);

        path = find_path_between_intersections(75561, 160740, 15.00000000000000000);
        CHECK(path_is_legal(75561, 160740, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 3911.58976340184108267);

        path = find_path_between_intersections(82337, 177822, 15.00000000000000000);
        CHECK(path_is_legal(82337, 177822, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1824.09868882376554211);

        path = find_path_between_intersections(90134, 11485, 0.00000000000000000);
        CHECK(path_is_legal(90134, 11485, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1749.20643704648705352);

        path = find_path_between_intersections(99384, 138862, 0.00000000000000000);
        CHECK(path_is_legal(99384, 138862, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1610.44942411160991469);

        path = find_path_between_intersections(114431, 77605, 15.00000000000000000);
        CHECK(path_is_legal(114431, 77605, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1776.73606665819465888);

        path = find_path_between_intersections(124175, 240326, 0.00000000000000000);
        CHECK(path_is_legal(124175, 240326, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1465.31233889786085456);

        path = find_path_between_intersections(97616, 93084, 15.00000000000000000);
        CHECK(path_is_legal(97616, 93084, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2720.16975698004034712);

        path = find_path_between_intersections(146595, 150028, 0.00000000000000000);
        CHECK(path_is_legal(146595, 150028, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 557.68939486831823160);

        path = find_path_between_intersections(148592, 207426, 0.00000000000000000);
        CHECK(path_is_legal(148592, 207426, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1921.44528368345731906);

        path = find_path_between_intersections(87809, 59217, 0.00000000000000000);
        CHECK(path_is_legal(87809, 59217, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 785.25064308215166875);

        path = find_path_between_intersections(107359, 178721, 15.00000000000000000);
        CHECK(path_is_legal(107359, 178721, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2057.89722708461658840);

        path = find_path_between_intersections(153799, 43171, 0.00000000000000000);
        CHECK(path_is_legal(153799, 43171, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 659.07752288255744588);

        path = find_path_between_intersections(155804, 237601, 15.00000000000000000);
        CHECK(path_is_legal(155804, 237601, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 3318.18195331856850316);

        path = find_path_between_intersections(161515, 105890, 0.00000000000000000);
        CHECK(path_is_legal(161515, 105890, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1095.12741043151072518);

        path = find_path_between_intersections(142606, 39012, 15.00000000000000000);
        CHECK(path_is_legal(142606, 39012, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1940.19647543368319020);

        path = find_path_between_intersections(182627, 161448, 0.00000000000000000);
        CHECK(path_is_legal(182627, 161448, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 980.38684742570535491);

        path = find_path_between_intersections(185177, 158839, 15.00000000000000000);
        CHECK(path_is_legal(185177, 158839, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 3012.82078286195383043);

        path = find_path_between_intersections(185469, 43413, 15.00000000000000000);
        CHECK(path_is_legal(185469, 43413, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2215.31226919084701876);

        path = find_path_between_intersections(57321, 14603, 15.00000000000000000);
        CHECK(path_is_legal(57321, 14603, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1283.68251598758843102);

        path = find_path_between_intersections(194667, 83200, 15.00000000000000000);
        CHECK(path_is_legal(194667, 83200, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 3028.60930994782984271);

        path = find_path_between_intersections(90613, 91812, 15.00000000000000000);
        CHECK(path_is_legal(90613, 91812, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 1569.55892725398189214);

        path = find_path_between_intersections(46187, 172633, 0.00000000000000000);
        CHECK(path_is_legal(46187, 172633, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 826.50308202594476370);

        path = find_path_between_intersections(213724, 186007, 0.00000000000000000);
        CHECK(path_is_legal(213724, 186007, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1760.77769759369698477);

        path = find_path_between_intersections(219266, 29777, 15.00000000000000000);
        CHECK(path_is_legal(219266, 29777, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2565.68938633498237323);

        path = find_path_between_intersections(221820, 212406, 0.00000000000000000);
        CHECK(path_is_legal(221820, 212406, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 876.68587599887780470);

        path = find_path_between_intersections(80633, 58410, 0.00000000000000000);
        CHECK(path_is_legal(80633, 58410, path));
        CHECK(compute_path_travel_time(path, 0.00000000000000000) <= 1442.35700842045025638);

        path = find_path_between_intersections(234665, 62886, 15.00000000000000000);
        CHECK(path_is_legal(234665, 62886, path));
        CHECK(compute_path_travel_time(path, 15.00000000000000000) <= 2418.43915190852749220);

    } //find_path_between_intersections_optimality

} //inter_inter_path_func_optimality_public

