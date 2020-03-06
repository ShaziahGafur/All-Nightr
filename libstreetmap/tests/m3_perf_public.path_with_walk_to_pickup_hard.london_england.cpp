#include <random>
#include <unittest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"

#include "unit_test_util.h"
#include "path_verify.h"

using ece297test::relative_error;
using ece297test::path_is_legal;


SUITE(path_with_walk_to_pickup_perf_hard_public) {
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
    TEST_FIXTURE(MapFixture, find_path_with_walk_to_pick_up_perf_hard) {
        //Verify Functionality
        std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> path;
        path = find_path_with_walk_to_pick_up(68434, 65486, 22.03968825460766823, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(68434, 65486, 22.03968825460766823, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 22.03968825460766823) <= 2798.39885243821890981);

        path = find_path_with_walk_to_pick_up(136459, 179710, 23.58207964209191232, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(136459, 179710, 23.58207964209191232, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 23.58207964209191232) <= 4132.20573820808294840);

        path = find_path_with_walk_to_pick_up(145196, 96131, 22.42551950461261612, 1.39999999999999991, 111.84617772556984505);
        CHECK(path_is_legal(145196, 96131, 22.42551950461261612, 1.39999999999999991, 111.84617772556984505, path));
        CHECK(compute_path_travel_time(path.second, 22.42551950461261612) <= 72.51938758237737659);

        path = find_path_with_walk_to_pick_up(148758, 147829, 19.89102086808226488, 4.56848485600839016, 27.53582984287076130);
        CHECK(path_is_legal(148758, 147829, 19.89102086808226488, 4.56848485600839016, 27.53582984287076130, path));
        CHECK(compute_path_travel_time(path.second, 19.89102086808226488) <= 1382.33690602843398665);

        path = find_path_with_walk_to_pick_up(162105, 141762, 18.12727336456592298, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(162105, 141762, 18.12727336456592298, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 18.12727336456592298) <= 977.65240912006959206);

        path = find_path_with_walk_to_pick_up(163296, 35783, 21.63952810726744502, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(163296, 35783, 21.63952810726744502, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 21.63952810726744502) <= 1982.71622314894807459);

        path = find_path_with_walk_to_pick_up(228788, 4467, 19.76185147836938683, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(228788, 4467, 19.76185147836938683, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 19.76185147836938683) <= 4280.64558587007650203);

        path = find_path_with_walk_to_pick_up(230574, 150660, 18.22304086160225722, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(230574, 150660, 18.22304086160225722, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 18.22304086160225722) <= 2842.09354876509723908);

        path = find_path_with_walk_to_pick_up(15148, 226284, 17.03745712321989103, 3.43447944824956064, 196.72098233849041549);
        CHECK(path_is_legal(15148, 226284, 17.03745712321989103, 3.43447944824956064, 196.72098233849041549, path));
        CHECK(compute_path_travel_time(path.second, 17.03745712321989103) <= 2404.23062718659230086);

        path = find_path_with_walk_to_pick_up(77998, 97178, 21.67519896478087560, 3.86521214689720694, 189.60650191258073960);
        CHECK(path_is_legal(77998, 97178, 21.67519896478087560, 3.86521214689720694, 189.60650191258073960, path));
        CHECK(compute_path_travel_time(path.second, 21.67519896478087560) <= 1301.70951075219682025);

        path = find_path_with_walk_to_pick_up(79984, 106582, 22.45990359895624877, 4.68897467196018791, 142.52859647010359367);
        CHECK(path_is_legal(79984, 106582, 22.45990359895624877, 4.68897467196018791, 142.52859647010359367, path));
        CHECK(compute_path_travel_time(path.second, 22.45990359895624877) <= 2692.54921746751051614);

        path = find_path_with_walk_to_pick_up(106548, 22100, 24.45437769244494319, 2.77490814491235582, 41.93471005470038193);
        CHECK(path_is_legal(106548, 22100, 24.45437769244494319, 2.77490814491235582, 41.93471005470038193, path));
        CHECK(compute_path_travel_time(path.second, 24.45437769244494319) <= 2028.75121830612283702);

        path = find_path_with_walk_to_pick_up(131313, 104126, 18.17953959676264120, 1.93533970074921680, 88.16806559754864736);
        CHECK(path_is_legal(131313, 104126, 18.17953959676264120, 1.93533970074921680, 88.16806559754864736, path));
        CHECK(compute_path_travel_time(path.second, 18.17953959676264120) <= 2779.54708101041615009);

        path = find_path_with_walk_to_pick_up(136653, 237598, 23.96866203997892342, 1.80389743142182679, 257.61776710016329162);
        CHECK(path_is_legal(136653, 237598, 23.96866203997892342, 1.80389743142182679, 257.61776710016329162, path));
        CHECK(compute_path_travel_time(path.second, 23.96866203997892342) <= 2538.05738734312262750);

        path = find_path_with_walk_to_pick_up(16239, 209556, 20.98177824478972298, 1.39999999999999991, 70.03409454965564862);
        CHECK(path_is_legal(16239, 209556, 20.98177824478972298, 1.39999999999999991, 70.03409454965564862, path));
        CHECK(compute_path_travel_time(path.second, 20.98177824478972298) <= 53.85664264176647720);

        path = find_path_with_walk_to_pick_up(67625, 207964, 16.71504981361658437, 2.09773067571534222, 87.70654758349419922);
        CHECK(path_is_legal(67625, 207964, 16.71504981361658437, 2.09773067571534222, 87.70654758349419922, path));
        CHECK(compute_path_travel_time(path.second, 16.71504981361658437) <= 1509.49208488677822970);

        path = find_path_with_walk_to_pick_up(158520, 158518, 17.78989965221452607, 1.39999999999999991, 49.62870890294288984);
        CHECK(path_is_legal(158520, 158518, 17.78989965221452607, 1.39999999999999991, 49.62870890294288984, path));
        CHECK(compute_path_travel_time(path.second, 17.78989965221452607) <= 2.62578951914155567);

        path = find_path_with_walk_to_pick_up(160196, 180922, 15.90075664158679203, 1.61076364647576797, 77.43564537261509884);
        CHECK(path_is_legal(160196, 180922, 15.90075664158679203, 1.61076364647576797, 77.43564537261509884, path));
        CHECK(compute_path_travel_time(path.second, 15.90075664158679203) <= 3270.33161535710269163);

        path = find_path_with_walk_to_pick_up(180826, 237419, 16.65614495382273930, 2.75862476886342334, 151.58627932028602459);
        CHECK(path_is_legal(180826, 237419, 16.65614495382273930, 2.75862476886342334, 151.58627932028602459, path));
        CHECK(compute_path_travel_time(path.second, 16.65614495382273930) <= 1974.28319193377888041);

        path = find_path_with_walk_to_pick_up(187394, 78626, 23.53722743820737406, 2.71072767974519957, 146.73259977531978393);
        CHECK(path_is_legal(187394, 78626, 23.53722743820737406, 2.71072767974519957, 146.73259977531978393, path));
        CHECK(compute_path_travel_time(path.second, 23.53722743820737406) <= 5433.77911616015444451);

        path = find_path_with_walk_to_pick_up(192985, 31515, 20.45841070499538716, 1.20695364002606320, 126.82637949508593067);
        CHECK(path_is_legal(192985, 31515, 20.45841070499538716, 1.20695364002606320, 126.82637949508593067, path));
        CHECK(compute_path_travel_time(path.second, 20.45841070499538716) <= 2991.01766367465825169);

        path = find_path_with_walk_to_pick_up(200250, 200255, 20.96228998539799093, 1.39999999999999991, 90.89732617029939377);
        CHECK(path_is_legal(200250, 200255, 20.96228998539799093, 1.39999999999999991, 90.89732617029939377, path));
        CHECK(compute_path_travel_time(path.second, 20.96228998539799093) <= 24.08985069317224870);

        path = find_path_with_walk_to_pick_up(215557, 162354, 16.58850420671487313, 1.71777433561344273, 274.02713409085180274);
        CHECK(path_is_legal(215557, 162354, 16.58850420671487313, 1.71777433561344273, 274.02713409085180274, path));
        CHECK(compute_path_travel_time(path.second, 16.58850420671487313) <= 694.22802838331404018);

        path = find_path_with_walk_to_pick_up(217805, 89186, 18.52929843128670484, 1.89185028781409170, 107.21274765032889320);
        CHECK(path_is_legal(217805, 89186, 18.52929843128670484, 1.89185028781409170, 107.21274765032889320, path));
        CHECK(compute_path_travel_time(path.second, 18.52929843128670484) <= 3233.14578173777408665);

        path = find_path_with_walk_to_pick_up(220163, 70950, 15.29998809213639177, 1.39999999999999991, 153.56921843142052353);
        CHECK(path_is_legal(220163, 70950, 15.29998809213639177, 1.39999999999999991, 153.56921843142052353, path));
        CHECK(compute_path_travel_time(path.second, 15.29998809213639177) <= 6.59232936763346533);

        path = find_path_with_walk_to_pick_up(224552, 200994, 15.41652320998716696, 1.70539632939149310, 91.38353873260361127);
        CHECK(path_is_legal(224552, 200994, 15.41652320998716696, 1.70539632939149310, 91.38353873260361127, path));
        CHECK(compute_path_travel_time(path.second, 15.41652320998716696) <= 1530.94237814612301918);

        path = find_path_with_walk_to_pick_up(231576, 98287, 23.32528392383026627, 1.39999999999999991, 60.71252529790830010);
        CHECK(path_is_legal(231576, 98287, 23.32528392383026627, 1.39999999999999991, 60.71252529790830010, path));
        CHECK(compute_path_travel_time(path.second, 23.32528392383026627) <= 68.97783822559433986);

        path = find_path_with_walk_to_pick_up(220926, 216535, 16.36906750429234236, 4.54090149687136524, 194.82443623747644779);
        CHECK(path_is_legal(220926, 216535, 16.36906750429234236, 4.54090149687136524, 194.82443623747644779, path));
        CHECK(compute_path_travel_time(path.second, 16.36906750429234236) <= 4037.54613442904064868);

        path = find_path_with_walk_to_pick_up(238603, 171327, 20.41577714410001221, 3.02242961318037118, 22.44809494514823101);
        CHECK(path_is_legal(238603, 171327, 20.41577714410001221, 3.02242961318037118, 22.44809494514823101, path));
        CHECK(compute_path_travel_time(path.second, 20.41577714410001221) <= 730.81724234265527684);

        path = find_path_with_walk_to_pick_up(172363, 152903, 15.81979213732948075, 2.58476219833258769, 132.50007107741168966);
        CHECK(path_is_legal(172363, 152903, 15.81979213732948075, 2.58476219833258769, 132.50007107741168966, path));
        CHECK(compute_path_travel_time(path.second, 15.81979213732948075) <= 4277.69167200928313832);

        //Generate random inputs
        std::vector<IntersectionIndex> intersection_ids1;
        std::vector<IntersectionIndex> intersection_ids2;
        std::vector<double> turn_penalties;
        std::vector<double> walking_speeds;
        std::vector<double> walking_time_limits;
        for(size_t i = 0; i < 25; i++) {
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
            ECE297_TIME_CONSTRAINT(5961);
            std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> result;
            for(size_t i = 0; i < 25; i++) {
                result = find_path_with_walk_to_pick_up(intersection_ids1[i], intersection_ids2[i], turn_penalties[i],
                        walking_speeds[i], walking_time_limits[i]);
            }
        }
    } //find_path_with_walk_to_pick_up_perf_hard

} //path_with_walk_to_pickup_perf_hard_public

