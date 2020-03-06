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
    TEST(find_path_with_walk_to_pick_up_legality) {
        std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> path;

        path = find_path_with_walk_to_pick_up(3476, 5207, 17.09186127700705882, 4.96718775348431674, 60.86898706269057868);
        CHECK(path_is_legal(3476, 5207, 17.09186127700705882, 4.96718775348431674, 60.86898706269057868, path));

        path = find_path_with_walk_to_pick_up(6100, 6135, 18.13752153377841125, 1.39999999999999991, 54.71238909427748354);
        CHECK(path_is_legal(6100, 6135, 18.13752153377841125, 1.39999999999999991, 54.71238909427748354, path));

        path = find_path_with_walk_to_pick_up(9088, 89818, 22.58759087598575732, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(9088, 89818, 22.58759087598575732, 1.39999999999999991, 0.00000000000000000, path));

        path = find_path_with_walk_to_pick_up(54341, 36647, 24.74049806129059093, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(54341, 36647, 24.74049806129059093, 1.39999999999999991, 0.00000000000000000, path));

        path = find_path_with_walk_to_pick_up(55780, 7107, 17.38266612778162568, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(55780, 7107, 17.38266612778162568, 1.39999999999999991, 0.00000000000000000, path));

        path = find_path_with_walk_to_pick_up(61505, 85936, 18.40129796266718643, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(61505, 85936, 18.40129796266718643, 1.39999999999999991, 0.00000000000000000, path));

        path = find_path_with_walk_to_pick_up(67977, 47586, 17.14055432418023628, 1.39999999999999991, 41.04450625666842001);
        CHECK(path_is_legal(67977, 47586, 17.14055432418023628, 1.39999999999999991, 41.04450625666842001, path));

        path = find_path_with_walk_to_pick_up(90722, 92846, 22.04203310234851898, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(90722, 92846, 22.04203310234851898, 1.39999999999999991, 0.00000000000000000, path));

        path = find_path_with_walk_to_pick_up(48897, 142693, 22.51417288799332539, 3.59769246754422145, 20.41798773437117376);
        CHECK(path_is_legal(48897, 142693, 22.51417288799332539, 3.59769246754422145, 20.41798773437117376, path));

        path = find_path_with_walk_to_pick_up(137276, 131450, 24.80338191831117456, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(137276, 131450, 24.80338191831117456, 1.39999999999999991, 0.00000000000000000, path));

        path = find_path_with_walk_to_pick_up(188, 129899, 23.55376778903119117, 4.24280161924381360, 100.86802366925402907);
        CHECK(path_is_legal(188, 129899, 23.55376778903119117, 4.24280161924381360, 100.86802366925402907, path));

        path = find_path_with_walk_to_pick_up(11043, 110628, 15.92647539527768785, 2.94874266921210904, 251.42470305510485673);
        CHECK(path_is_legal(11043, 110628, 15.92647539527768785, 2.94874266921210904, 251.42470305510485673, path));

        path = find_path_with_walk_to_pick_up(32796, 136251, 21.81291364015829970, 2.75642041261198489, 129.55739160857751813);
        CHECK(path_is_legal(32796, 136251, 21.81291364015829970, 2.75642041261198489, 129.55739160857751813, path));

        path = find_path_with_walk_to_pick_up(71793, 126304, 20.11897394771401792, 1.64256120121176474, 181.49700581740253824);
        CHECK(path_is_legal(71793, 126304, 20.11897394771401792, 1.64256120121176474, 181.49700581740253824, path));

        path = find_path_with_walk_to_pick_up(72243, 141588, 17.29162505536838879, 1.91794300974800835, 134.37556412834419461);
        CHECK(path_is_legal(72243, 141588, 17.29162505536838879, 1.91794300974800835, 134.37556412834419461, path));

        path = find_path_with_walk_to_pick_up(47444, 100320, 24.70825215594946300, 1.97246632450334825, 209.30152302549333854);
        CHECK(path_is_legal(47444, 100320, 24.70825215594946300, 1.97246632450334825, 209.30152302549333854, path));

        path = find_path_with_walk_to_pick_up(86452, 29788, 22.16138655906405575, 4.28457719647844915, 279.83615334748458281);
        CHECK(path_is_legal(86452, 29788, 22.16138655906405575, 4.28457719647844915, 279.83615334748458281, path));

        path = find_path_with_walk_to_pick_up(26717, 108459, 23.46132436906174945, 1.39999999999999991, 197.57349445421218093);
        CHECK(path_is_legal(26717, 108459, 23.46132436906174945, 1.39999999999999991, 197.57349445421218093, path));

        path = find_path_with_walk_to_pick_up(102766, 96192, 17.76186240368787850, 3.90112849879242507, 102.19600199439707922);
        CHECK(path_is_legal(102766, 96192, 17.76186240368787850, 3.90112849879242507, 102.19600199439707922, path));

        path = find_path_with_walk_to_pick_up(84723, 10335, 23.26211029393396501, 4.93063301304268187, 290.25227446485695282);
        CHECK(path_is_legal(84723, 10335, 23.26211029393396501, 4.93063301304268187, 290.25227446485695282, path));

        path = find_path_with_walk_to_pick_up(115422, 102117, 21.36468488332522853, 3.41788634905191380, 282.73970384645042486);
        CHECK(path_is_legal(115422, 102117, 21.36468488332522853, 3.41788634905191380, 282.73970384645042486, path));

        path = find_path_with_walk_to_pick_up(116429, 116427, 19.37938493793083694, 1.39999999999999991, 36.44011437017926625);
        CHECK(path_is_legal(116429, 116427, 19.37938493793083694, 1.39999999999999991, 36.44011437017926625, path));

        path = find_path_with_walk_to_pick_up(120392, 116425, 16.99415734476158235, 3.11859608116758746, 112.20273183569108255);
        CHECK(path_is_legal(120392, 116425, 16.99415734476158235, 3.11859608116758746, 112.20273183569108255, path));

        path = find_path_with_walk_to_pick_up(128605, 143577, 17.84137078958718092, 4.76750812538666580, 60.71252529790830010);
        CHECK(path_is_legal(128605, 143577, 17.84137078958718092, 4.76750812538666580, 60.71252529790830010, path));

        path = find_path_with_walk_to_pick_up(42103, 63527, 16.95061114321478257, 3.50757257023562552, 274.53301691724317379);
        CHECK(path_is_legal(42103, 63527, 16.95061114321478257, 3.50757257023562552, 274.53301691724317379, path));

        path = find_path_with_walk_to_pick_up(4551, 11004, 15.06191257662446858, 3.13790223835049087, 55.72929914639360049);
        CHECK(path_is_legal(4551, 11004, 15.06191257662446858, 3.13790223835049087, 55.72929914639360049, path));

        path = find_path_with_walk_to_pick_up(91799, 46931, 17.56525768651765773, 1.39999999999999991, 238.63497263933146542);
        CHECK(path_is_legal(91799, 46931, 17.56525768651765773, 1.39999999999999991, 238.63497263933146542, path));

        path = find_path_with_walk_to_pick_up(137826, 149072, 21.73181346174965611, 1.60757833457995147, 188.77115750217822665);
        CHECK(path_is_legal(137826, 149072, 21.73181346174965611, 1.60757833457995147, 188.77115750217822665, path));

        path = find_path_with_walk_to_pick_up(147045, 43999, 16.53896805106891676, 2.27318993094735777, 101.17857039720308876);
        CHECK(path_is_legal(147045, 43999, 16.53896805106891676, 2.27318993094735777, 101.17857039720308876, path));

        path = find_path_with_walk_to_pick_up(148090, 148076, 22.92007129296922940, 1.39999999999999991, 49.62417147907945747);
        CHECK(path_is_legal(148090, 148076, 22.92007129296922940, 1.39999999999999991, 49.62417147907945747, path));

    } //find_path_with_walk_to_pick_up_legality

} //path_with_walk_to_pickup_legality_public

