#include <random>
#include <unittest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"

#include "unit_test_util.h"
#include "path_verify.h"

using ece297test::relative_error;
using ece297test::path_is_legal;


SUITE(path_with_walk_to_pickup_perf_medium_public) {
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
    TEST_FIXTURE(MapFixture, find_path_with_walk_to_pick_up_perf_medium) {
        //Verify Functionality
        std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> path;
        path = find_path_with_walk_to_pick_up(43398, 43107, 24.82604173884579524, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(43398, 43107, 24.82604173884579524, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 24.82604173884579524) <= 1970.49068014809199667);

        path = find_path_with_walk_to_pick_up(24099, 46244, 16.65614495382273930, 1.39999999999999991, 65.55530968667447667);
        CHECK(path_is_legal(24099, 46244, 16.65614495382273930, 1.39999999999999991, 65.55530968667447667, path));
        CHECK(compute_path_travel_time(path.second, 16.65614495382273930) <= 6.08230124756841306);

        path = find_path_with_walk_to_pick_up(48237, 47917, 19.89102086808226488, 1.39999999999999991, 81.09569291065832886);
        CHECK(path_is_legal(48237, 47917, 19.89102086808226488, 1.39999999999999991, 81.09569291065832886, path));
        CHECK(compute_path_travel_time(path.second, 19.89102086808226488) <= 18.22219829210879283);

        path = find_path_with_walk_to_pick_up(30929, 129014, 15.93547418887312084, 4.00808120502696852, 37.19129828234493118);
        CHECK(path_is_legal(30929, 129014, 15.93547418887312084, 4.00808120502696852, 37.19129828234493118, path));
        CHECK(compute_path_travel_time(path.second, 15.93547418887312084) <= 1979.31255011201483285);

        path = find_path_with_walk_to_pick_up(87192, 95462, 16.92280555852369517, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(87192, 95462, 16.92280555852369517, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 16.92280555852369517) <= 2074.54440027342616304);

        path = find_path_with_walk_to_pick_up(97731, 88004, 20.17182549212847675, 0.99465713279713786, 36.04427610336737331);
        CHECK(path_is_legal(97731, 88004, 20.17182549212847675, 0.99465713279713786, 36.04427610336737331, path));
        CHECK(compute_path_travel_time(path.second, 20.17182549212847675) <= 2165.49353481595926496);

        path = find_path_with_walk_to_pick_up(106654, 102129, 17.90023057511676186, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(106654, 102129, 17.90023057511676186, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 17.90023057511676186) <= 1660.07752300618153640);

        path = find_path_with_walk_to_pick_up(1708, 108377, 21.69284068351273831, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(1708, 108377, 21.69284068351273831, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 21.69284068351273831) <= 1528.30940514377675754);

        path = find_path_with_walk_to_pick_up(124575, 110738, 20.86010464040528234, 0.95157840000920491, 165.71950729141926217);
        CHECK(path_is_legal(124575, 110738, 20.86010464040528234, 0.95157840000920491, 165.71950729141926217, path));
        CHECK(compute_path_travel_time(path.second, 20.86010464040528234) <= 602.02538035516033688);

        path = find_path_with_walk_to_pick_up(130777, 36345, 24.46383170662661399, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(130777, 36345, 24.46383170662661399, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 24.46383170662661399) <= 565.51688821292134435);

        path = find_path_with_walk_to_pick_up(141763, 106629, 17.91447647939344989, 1.51130898454886142, 59.42036260916577106);
        CHECK(path_is_legal(141763, 106629, 17.91447647939344989, 1.51130898454886142, 59.42036260916577106, path));
        CHECK(compute_path_travel_time(path.second, 17.91447647939344989) <= 1626.50344868598585890);

        path = find_path_with_walk_to_pick_up(143933, 30702, 24.44644791758729951, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(143933, 30702, 24.44644791758729951, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 24.44644791758729951) <= 1735.14600500764254321);

        path = find_path_with_walk_to_pick_up(9375, 140038, 17.03745712321989103, 3.43447944824956064, 196.72098233849041549);
        CHECK(path_is_legal(9375, 140038, 17.03745712321989103, 3.43447944824956064, 196.72098233849041549, path));
        CHECK(compute_path_travel_time(path.second, 17.03745712321989103) <= 766.34979122397726314);

        path = find_path_with_walk_to_pick_up(9621, 57207, 17.99571822308105951, 2.96039398907351270, 262.53690682781547139);
        CHECK(path_is_legal(9621, 57207, 17.99571822308105951, 2.96039398907351270, 262.53690682781547139, path));
        CHECK(compute_path_travel_time(path.second, 17.99571822308105951) <= 2177.50965433936971749);

        path = find_path_with_walk_to_pick_up(20636, 20882, 17.74109712564015950, 3.68781029567370577, 77.00360068941159852);
        CHECK(path_is_legal(20636, 20882, 17.74109712564015950, 3.68781029567370577, 77.00360068941159852, path));
        CHECK(compute_path_travel_time(path.second, 17.74109712564015950) <= 470.80704959561484202);

        path = find_path_with_walk_to_pick_up(29286, 5879, 24.02208313989090271, 1.39999999999999991, 77.43564537261509884);
        CHECK(path_is_legal(29286, 5879, 24.02208313989090271, 1.39999999999999991, 77.43564537261509884, path));
        CHECK(compute_path_travel_time(path.second, 24.02208313989090271) <= 7.27822147634801997);

        path = find_path_with_walk_to_pick_up(70078, 34497, 17.09874884921223170, 1.06272448021076205, 295.42869587712903012);
        CHECK(path_is_legal(70078, 34497, 17.09874884921223170, 1.06272448021076205, 295.42869587712903012, path));
        CHECK(compute_path_travel_time(path.second, 17.09874884921223170) <= 251.66964102483905208);

        path = find_path_with_walk_to_pick_up(71467, 35066, 24.13029619854805219, 3.34338160104507409, 167.15297635290451694);
        CHECK(path_is_legal(71467, 35066, 24.13029619854805219, 3.34338160104507409, 167.15297635290451694, path));
        CHECK(compute_path_travel_time(path.second, 24.13029619854805219) <= 347.79056689641180355);

        path = find_path_with_walk_to_pick_up(53543, 53547, 18.04165087403552903, 1.39999999999999991, 77.98930627243511537);
        CHECK(path_is_legal(53543, 53547, 18.04165087403552903, 1.39999999999999991, 77.98930627243511537, path));
        CHECK(compute_path_travel_time(path.second, 18.04165087403552903) <= 9.30623968958248859);

        path = find_path_with_walk_to_pick_up(77233, 127911, 16.55227837724952522, 3.12015467344443920, 78.99469089954210688);
        CHECK(path_is_legal(77233, 127911, 16.55227837724952522, 3.12015467344443920, 78.99469089954210688, path));
        CHECK(compute_path_travel_time(path.second, 16.55227837724952522) <= 750.54315818540294458);

        path = find_path_with_walk_to_pick_up(78680, 48721, 15.74458254907107602, 4.05253804299871945, 133.94699896999748034);
        CHECK(path_is_legal(78680, 48721, 15.74458254907107602, 4.05253804299871945, 133.94699896999748034, path));
        CHECK(compute_path_travel_time(path.second, 15.74458254907107602) <= 2921.56396461980602908);

        path = find_path_with_walk_to_pick_up(86181, 131510, 19.00171824583044966, 3.90974697206861954, 187.03508151671948667);
        CHECK(path_is_legal(86181, 131510, 19.00171824583044966, 3.90974697206861954, 187.03508151671948667, path));
        CHECK(compute_path_travel_time(path.second, 19.00171824583044966) <= 917.89136410679236633);

        path = find_path_with_walk_to_pick_up(87049, 10517, 19.70216224979132136, 1.39999999999999991, 187.72694175901168023);
        CHECK(path_is_legal(87049, 10517, 19.70216224979132136, 1.39999999999999991, 187.72694175901168023, path));
        CHECK(compute_path_travel_time(path.second, 19.70216224979132136) <= 52.56567467984003628);

        path = find_path_with_walk_to_pick_up(103149, 94120, 19.83329720831089915, 2.77692413206747490, 93.86182123838005964);
        CHECK(path_is_legal(103149, 94120, 19.83329720831089915, 2.77692413206747490, 93.86182123838005964, path));
        CHECK(compute_path_travel_time(path.second, 19.83329720831089915) <= 2432.57454536428713254);

        path = find_path_with_walk_to_pick_up(114941, 86106, 24.30747583831690406, 2.87589492436569127, 275.95300059268697623);
        CHECK(path_is_legal(114941, 86106, 24.30747583831690406, 2.87589492436569127, 275.95300059268697623, path));
        CHECK(compute_path_travel_time(path.second, 24.30747583831690406) <= 1636.07490944073356332);

        path = find_path_with_walk_to_pick_up(121272, 2615, 24.09722874857938990, 4.48910482803189748, 116.59049545560023375);
        CHECK(path_is_legal(121272, 2615, 24.09722874857938990, 4.48910482803189748, 116.59049545560023375, path));
        CHECK(compute_path_travel_time(path.second, 24.09722874857938990) <= 1727.78180137716867648);

        path = find_path_with_walk_to_pick_up(129151, 115497, 16.94758728063605702, 1.67440756558571580, 57.51808304799190807);
        CHECK(path_is_legal(129151, 115497, 16.94758728063605702, 1.67440756558571580, 57.51808304799190807, path));
        CHECK(compute_path_travel_time(path.second, 16.94758728063605702) <= 1102.42932439451283244);

        path = find_path_with_walk_to_pick_up(138772, 138768, 22.03559771910337162, 1.39999999999999991, 34.68574126152051917);
        CHECK(path_is_legal(138772, 138768, 22.03559771910337162, 1.39999999999999991, 34.68574126152051917, path));
        CHECK(compute_path_travel_time(path.second, 22.03559771910337162) <= 3.62030047480073947);

        path = find_path_with_walk_to_pick_up(143031, 147358, 20.19973754572945523, 2.18062209417312935, 223.35113912497985211);
        CHECK(path_is_legal(143031, 147358, 20.19973754572945523, 2.18062209417312935, 223.35113912497985211, path));
        CHECK(compute_path_travel_time(path.second, 20.19973754572945523) <= 1547.89595049092145018);

        path = find_path_with_walk_to_pick_up(49376, 54896, 19.23660809272987393, 2.82422588918281470, 237.49865636288913606);
        CHECK(path_is_legal(49376, 54896, 19.23660809272987393, 2.82422588918281470, 237.49865636288913606, path));
        CHECK(compute_path_travel_time(path.second, 19.23660809272987393) <= 770.46948078890864053);

        //Generate random inputs
        std::vector<IntersectionIndex> intersection_ids1;
        std::vector<IntersectionIndex> intersection_ids2;
        std::vector<double> turn_penalties;
        std::vector<double> walking_speeds;
        std::vector<double> walking_time_limits;
        for(size_t i = 0; i < 40; i++) {
            intersection_ids1.push_back(rand_intersection(rng));
            turn_penalties.push_back(rand_turn_penalty(rng));
            walking_speeds.push_back(rand_walking_speed(rng));
            walking_time_limits.push_back(rand_walking_time_limit(rng));

           double radius = walking_speeds[i]*walking_time_limits[i];

           IntersectionIndex id2 = -1;
           double dist = 0.0;
           do {
               id2 = rand_intersection(rng);
               dist = find_distance_between_two_points(std::make_pair(getIntersectionPosition(intersection_ids1[i]),
                   getIntersectionPosition(id2)));
           } while (dist <= radius);

            intersection_ids2.push_back(id2);
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(148697);
            std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> result;
            for(size_t i = 0; i < 40; i++) {
                result = find_path_with_walk_to_pick_up(intersection_ids1[i], intersection_ids2[i], turn_penalties[i],
                        walking_speeds[i], walking_time_limits[i]);
            }
        }
    } //find_path_with_walk_to_pick_up_perf_medium

} //path_with_walk_to_pickup_perf_medium_public

