#include <random>
#include <unittest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"

#include "unit_test_util.h"
#include "path_verify.h"

using ece297test::relative_error;
using ece297test::path_is_legal;


SUITE(path_with_walk_to_pickup_optimality_public) {
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
    TEST(find_path_with_walk_to_pick_up_optimality) {
        std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> path;
        path = find_path_with_walk_to_pick_up(14685, 145134, 22.58759087598575732, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(14685, 145134, 22.58759087598575732, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 22.58759087598575732) <= 3532.14845253336034148);

        path = find_path_with_walk_to_pick_up(90134, 11485, 17.38266612778162568, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(90134, 11485, 17.38266612778162568, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 17.38266612778162568) <= 2526.04665795545679430);

        path = find_path_with_walk_to_pick_up(99384, 138862, 18.40129796266718643, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(99384, 138862, 18.40129796266718643, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 18.40129796266718643) <= 2144.73895767853491634);

        path = find_path_with_walk_to_pick_up(99479, 99478, 16.82374630314258113, 1.39999999999999991, 116.54834174783871958);
        CHECK(path_is_legal(99479, 99478, 16.82374630314258113, 1.39999999999999991, 116.54834174783871958, path));
        CHECK(compute_path_travel_time(path.second, 16.82374630314258113) <= 18.12177707827149931);

        path = find_path_with_walk_to_pick_up(146595, 150028, 22.04203310234851898, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(146595, 150028, 22.04203310234851898, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 22.04203310234851898) <= 883.32653841785486293);

        path = find_path_with_walk_to_pick_up(87809, 59217, 24.74049806129059093, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(87809, 59217, 24.74049806129059093, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 24.74049806129059093) <= 1292.26360307709046538);

        path = find_path_with_walk_to_pick_up(159375, 138824, 19.18277652140982070, 1.39999999999999991, 53.25736922532109219);
        CHECK(path_is_legal(159375, 138824, 19.18277652140982070, 1.39999999999999991, 53.25736922532109219, path));
        CHECK(compute_path_travel_time(path.second, 19.18277652140982070) <= 9.01800882798833214);

        path = find_path_with_walk_to_pick_up(171135, 171136, 18.39392660871553886, 1.39999999999999991, 44.75989715180019601);
        CHECK(path_is_legal(171135, 171136, 18.39392660871553886, 1.39999999999999991, 44.75989715180019601, path));
        CHECK(compute_path_travel_time(path.second, 18.39392660871553886) <= 3.61030887510554521);

        path = find_path_with_walk_to_pick_up(221820, 212406, 24.80338191831117456, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(221820, 212406, 24.80338191831117456, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 24.80338191831117456) <= 1354.42625410591517721);

        path = find_path_with_walk_to_pick_up(138549, 83509, 15.72537179186508638, 1.95998220954890856, 221.50917848517320863);
        CHECK(path_is_legal(138549, 83509, 15.72537179186508638, 1.95998220954890856, 221.50917848517320863, path));
        CHECK(compute_path_travel_time(path.second, 15.72537179186508638) <= 3035.95369248596807665);

        path = find_path_with_walk_to_pick_up(59664, 67297, 16.36815020855561542, 2.97811561002735914, 278.14988328299739351);
        CHECK(path_is_legal(59664, 67297, 16.36815020855561542, 2.97811561002735914, 278.14988328299739351, path));
        CHECK(compute_path_travel_time(path.second, 16.36815020855561542) <= 1493.72811915631336888);

        path = find_path_with_walk_to_pick_up(65486, 171031, 24.40556743742627077, 4.87746590549877457, 83.74759460738201255);
        CHECK(path_is_legal(65486, 171031, 24.40556743742627077, 4.87746590549877457, 83.74759460738201255, path));
        CHECK(compute_path_travel_time(path.second, 24.40556743742627077) <= 1921.61616262260827170);

        path = find_path_with_walk_to_pick_up(77605, 102829, 23.46132436906174945, 1.39999999999999991, 76.95773059552976747);
        CHECK(path_is_legal(77605, 102829, 23.46132436906174945, 1.39999999999999991, 76.95773059552976747, path));
        CHECK(compute_path_travel_time(path.second, 23.46132436906174945) <= 35.29829108032804896);

        path = find_path_with_walk_to_pick_up(79948, 105868, 21.73103624073327467, 3.66142372886648637, 139.74431518657036122);
        CHECK(path_is_legal(79948, 105868, 21.73103624073327467, 3.66142372886648637, 139.74431518657036122, path));
        CHECK(compute_path_travel_time(path.second, 21.73103624073327467) <= 1712.51980034791722574);

        path = find_path_with_walk_to_pick_up(109551, 82424, 16.96346928813747468, 4.39258247139310054, 245.91440137455811055);
        CHECK(path_is_legal(109551, 82424, 16.96346928813747468, 4.39258247139310054, 245.91440137455811055, path));
        CHECK(compute_path_travel_time(path.second, 16.96346928813747468) <= 1319.76216891244007456);

        path = find_path_with_walk_to_pick_up(95461, 224336, 15.34326529172978404, 1.61925668015020863, 193.39804073111608318);
        CHECK(path_is_legal(95461, 224336, 15.34326529172978404, 1.61925668015020863, 193.39804073111608318, path));
        CHECK(compute_path_travel_time(path.second, 15.34326529172978404) <= 3473.91175735358820020);

        path = find_path_with_walk_to_pick_up(153333, 109805, 19.76557700056410027, 1.76248252325472277, 79.85307212485773221);
        CHECK(path_is_legal(153333, 109805, 19.76557700056410027, 1.76248252325472277, 79.85307212485773221, path));
        CHECK(compute_path_travel_time(path.second, 19.76557700056410027) <= 5985.53563724032937898);

        path = find_path_with_walk_to_pick_up(50071, 89829, 24.44070097577280265, 3.93065666851287432, 136.56773793218215474);
        CHECK(path_is_legal(50071, 89829, 24.44070097577280265, 3.93065666851287432, 136.56773793218215474, path));
        CHECK(compute_path_travel_time(path.second, 24.44070097577280265) <= 2594.15161190308936057);

        path = find_path_with_walk_to_pick_up(169300, 200209, 17.66813009934161016, 1.39999999999999991, 105.24706516757079555);
        CHECK(path_is_legal(169300, 200209, 17.66813009934161016, 1.39999999999999991, 105.24706516757079555, path));
        CHECK(compute_path_travel_time(path.second, 17.66813009934161016) <= 54.25196001794324019);

        path = find_path_with_walk_to_pick_up(195001, 23108, 15.86880151536486494, 1.39999999999999991, 51.12510540104166523);
        CHECK(path_is_legal(195001, 23108, 15.86880151536486494, 1.39999999999999991, 51.12510540104166523, path));
        CHECK(compute_path_travel_time(path.second, 15.86880151536486494) <= 4.33026437910524464);

        path = find_path_with_walk_to_pick_up(179710, 45539, 21.61257862409224373, 0.82600328218227714, 166.99301702503507272);
        CHECK(path_is_legal(179710, 45539, 21.61257862409224373, 0.82600328218227714, 166.99301702503507272, path));
        CHECK(compute_path_travel_time(path.second, 21.61257862409224373) <= 4291.71893441621705279);

        path = find_path_with_walk_to_pick_up(207287, 91687, 15.68126672736697813, 1.63754608479986441, 165.61400579768482544);
        CHECK(path_is_legal(207287, 91687, 15.68126672736697813, 1.63754608479986441, 165.61400579768482544, path));
        CHECK(compute_path_travel_time(path.second, 15.68126672736697813) <= 1899.15739361275814190);

        path = find_path_with_walk_to_pick_up(20991, 83553, 17.09036871035251437, 4.14471597040462481, 214.52685229983640625);
        CHECK(path_is_legal(20991, 83553, 17.09036871035251437, 4.14471597040462481, 214.52685229983640625, path));
        CHECK(compute_path_travel_time(path.second, 17.09036871035251437) <= 2207.19255966306900518);

        path = find_path_with_walk_to_pick_up(35783, 16684, 24.33272943060254079, 3.95595261295719602, 199.83517625315866439);
        CHECK(path_is_legal(35783, 16684, 24.33272943060254079, 3.95595261295719602, 199.83517625315866439, path));
        CHECK(compute_path_travel_time(path.second, 24.33272943060254079) <= 1911.00161571129274307);

        path = find_path_with_walk_to_pick_up(217129, 49739, 21.34049581338407364, 1.67858173634296537, 297.65626810602265095);
        CHECK(path_is_legal(217129, 49739, 21.34049581338407364, 1.67858173634296537, 297.65626810602265095, path));
        CHECK(compute_path_travel_time(path.second, 21.34049581338407364) <= 2004.47232727066580082);

        path = find_path_with_walk_to_pick_up(52474, 164195, 17.54559835623686581, 4.53318008616064194, 136.54915411554958382);
        CHECK(path_is_legal(52474, 164195, 17.54559835623686581, 4.53318008616064194, 136.54915411554958382, path));
        CHECK(compute_path_travel_time(path.second, 17.54559835623686581) <= 1960.25563308972550658);

        path = find_path_with_walk_to_pick_up(141589, 228669, 22.67413715053528733, 3.80778235480690253, 248.89837117703206104);
        CHECK(path_is_legal(141589, 228669, 22.67413715053528733, 3.80778235480690253, 248.89837117703206104, path));
        CHECK(compute_path_travel_time(path.second, 22.67413715053528733) <= 1391.32632456456303771);

        path = find_path_with_walk_to_pick_up(230522, 109842, 22.60420760996993295, 4.28225700794757991, 243.76710879661345643);
        CHECK(path_is_legal(230522, 109842, 22.60420760996993295, 4.28225700794757991, 243.76710879661345643, path));
        CHECK(compute_path_travel_time(path.second, 22.60420760996993295) <= 2604.88762259249506315);

        path = find_path_with_walk_to_pick_up(151284, 231042, 23.56226065994096786, 3.47316765099659630, 186.99188207513665816);
        CHECK(path_is_legal(151284, 231042, 23.56226065994096786, 3.47316765099659630, 186.99188207513665816, path));
        CHECK(compute_path_travel_time(path.second, 23.56226065994096786) <= 3194.13851404718616322);

        path = find_path_with_walk_to_pick_up(185971, 237181, 19.18735206180715736, 4.27008632345226591, 295.04521521733437339);
        CHECK(path_is_legal(185971, 237181, 19.18735206180715736, 4.27008632345226591, 295.04521521733437339, path));
        CHECK(compute_path_travel_time(path.second, 19.18735206180715736) <= 3684.32496617672859429);

    } //find_path_with_walk_to_pick_up_optimality

} //path_with_walk_to_pickup_optimality_public

