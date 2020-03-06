#include <random>
#include <unittest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"

#include "unit_test_util.h"
#include "path_verify.h"

using ece297test::relative_error;
using ece297test::path_is_legal;


SUITE(walking_path_cost_public) {
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
    TEST(compute_path_travel_time) {
        std::vector<StreetSegmentIndex> path;
        double turn_penalty;
        double walking_speed;
        double expected;
        double actual;

        path = {81515, 81514, 81513, 132835, 164923, 81483};
        turn_penalty = 20.52818021603572873;
        walking_speed = 3.33517607336110089;
        expected = 9.49868699434773767;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {5798, 5799, 5800, 5803};
        turn_penalty = 18.26087682029633541;
        walking_speed = 2.43167678446974200;
        expected = 17.69303284992707859;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {184196, 108313, 108314, 36555, 88470};
        turn_penalty = 34.66734537616928691;
        walking_speed = 2.11775904418693184;
        expected = 20.61248692060908283;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {87353, 205895, 80193, 80214, 80215, 80216, 205934, 148889, 183207, 118104};
        turn_penalty = 34.51993359273959072;
        walking_speed = 0.86994370304351953;
        expected = 22.04007293496762898;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {198746, 198747, 198723, 73067, 73066, 73065, 73064, 73063, 73062, 49269, 73087, 73088, 120686, 120683, 120687, 108228, 108194, 108193};
        turn_penalty = 33.54275001978079729;
        walking_speed = 2.71194833105055011;
        expected = 30.41252122900539590;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {62879, 128233, 62695, 62694, 62985, 62651, 62944, 63211, 217137, 169234, 216971, 159134, 217138, 63240, 62842, 63116, 63230, 177995, 62785, 177993, 63106, 63107, 62787};
        turn_penalty = 43.62876494617410117;
        walking_speed = 2.00392001764460481;
        expected = 35.35960314659224935;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {106259, 17278, 17279, 17280, 17281, 17282, 17283, 17284, 17285, 17286, 17287, 17288, 83944, 83936, 83937, 83938, 83939, 209552, 209553};
        turn_penalty = 29.64832794497377222;
        walking_speed = 3.43498870222091934;
        expected = 43.31564736476268251;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {144096, 144095, 144094, 144093, 144092, 144102, 9918, 9917, 9916, 9915, 141593, 141592, 141591, 141590, 9927, 136202, 136201, 136213, 136212, 136211, 182920, 182923, 182922, 1643, 1642, 1641, 1640, 182921, 185404, 185403, 182928, 211970, 176317, 200929};
        turn_penalty = 37.53834279945513686;
        walking_speed = 2.70788313432656125;
        expected = 54.24875543962002666;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {180955, 180956, 180957, 180958, 180959, 180951, 180949, 180950, 180952, 180953, 180954, 180948, 27325, 27326, 19316, 19315, 65334, 65337};
        turn_penalty = 26.39729671158957203;
        walking_speed = 4.12556905952097619;
        expected = 60.19786466490783994;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {45810, 45809, 45808, 45807, 86996, 67292, 67309, 64351, 64350, 64345, 64348, 64347, 64354, 64355, 64356, 64357, 64358, 64359, 64360, 218332, 64342, 64339, 64338, 64343, 64344, 164562, 164563, 164564, 164565, 164566, 218336, 218333, 127306, 127307, 127308, 127309, 127310, 67082, 67083};
        turn_penalty = 35.73388314152779088;
        walking_speed = 4.25714570821250859;
        expected = 65.68767427448260321;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {18375, 5905, 108603, 5913, 45350, 45349, 45348, 45347, 10029, 178826, 10030, 179751, 45355, 45356, 5221};
        turn_penalty = 12.97117716583002434;
        walking_speed = 3.36902423514803484;
        expected = 67.21249832487247033;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {181305, 181306, 113276, 181312, 181310, 181311, 101246, 101247, 101248, 101249, 101250, 101251, 23913, 23914, 23941};
        turn_penalty = 20.48237311733243615;
        walking_speed = 3.61309987964352253;
        expected = 82.09973508399201592;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {101678, 101676, 211570, 25412, 25411, 25410, 25409, 25408, 25407, 25406, 25405, 25404, 25403, 25402, 181859, 211566, 181861, 100707, 100708, 88869, 100685, 100686};
        turn_penalty = 26.65500735658102016;
        walking_speed = 2.90148829288311383;
        expected = 86.13495227181465452;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {124316, 124315, 124314, 124309, 22835, 176301, 22769, 82980, 82979, 22767, 22766, 82983, 82982, 22768, 116548, 116552, 189210, 191209, 116546, 181155, 181154, 181153, 181152, 181151, 207754, 181149, 181150, 181304, 181309, 181315, 166277, 23805, 23806, 23807, 23808};
        turn_penalty = 20.85227712661118815;
        walking_speed = 3.20877666847904308;
        expected = 88.70854133156127830;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {184555, 184556, 184528, 169250, 169249, 116033, 116036, 116037, 52140, 52139, 52138, 52137, 52136, 52135, 103365, 103364, 103363, 30471, 30470};
        turn_penalty = 15.11409834525278484;
        walking_speed = 2.76050899288038876;
        expected = 94.08534271558762896;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {202274, 86699, 94245, 94244, 94243, 94177, 94178, 94179, 94180, 94219, 94220, 94221, 94222, 200989, 200997, 200998, 200990, 152132, 152133, 95243, 93088, 93093, 93091, 93092, 91544, 91543, 91530, 180799, 180800, 116455};
        turn_penalty = 33.99182029046458098;
        walking_speed = 3.46933916344308280;
        expected = 97.29431000343660685;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {131401, 132707, 131382, 131383, 131384, 131385, 131386, 131387, 131388, 131389, 131390, 131391, 131392, 131393, 132704, 132705, 132706, 207783, 207784};
        turn_penalty = 15.33745718399443092;
        walking_speed = 3.99376719618737219;
        expected = 97.55362730697218865;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {51458, 93525, 93526, 93471, 93470, 93469, 93468, 93454, 93530, 121777, 121776, 121775, 121774, 121773, 121772, 121762, 121763, 121765, 109996, 47460, 47915};
        turn_penalty = 16.81407069785912611;
        walking_speed = 2.18940457641120290;
        expected = 103.72684073788420278;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {167726, 142420, 142424, 184328, 184325, 154165, 213412, 205931, 91851, 92231, 91978, 91782, 92365, 92357, 91991, 167722, 167721, 91793, 40258, 40259, 40260, 40261, 212283, 212284, 212270, 212272};
        turn_penalty = 16.63331716611783051;
        walking_speed = 2.63934167393095143;
        expected = 132.78188632859999530;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {139363, 132803, 132804, 132805, 132371, 132372, 160410, 160409, 160408, 160407, 160406};
        turn_penalty = 10.52221169261282618;
        walking_speed = 3.24321991146385713;
        expected = 134.70894278724895798;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {75318, 75319, 75308, 75322, 75331, 119262, 168568, 113182, 168566, 86958, 86951, 98312, 86950, 86955, 86957, 173097, 113050, 101687, 27430, 113052, 27431, 112567, 113133, 22008, 16319, 169043, 173057, 86058, 86059, 86068};
        turn_penalty = 30.62471546934541422;
        walking_speed = 4.89973621411523919;
        expected = 134.76944444456654537;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {55945, 55946, 55925, 55927, 55928, 55913, 140929, 140925, 140928, 140927, 61483, 140902, 61482, 61481};
        turn_penalty = 9.40665919658631999;
        walking_speed = 4.14471597040462481;
        expected = 148.35262615450091062;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {50328, 60331, 18280, 155574, 155575, 159853, 159852, 159851, 83977, 83978, 82647, 82648, 82649, 82650, 82651, 82652, 82653, 82654, 83985, 83986, 83987, 83988, 83989, 83990, 83991, 130556, 130557, 130558, 130564, 130552, 141531};
        turn_penalty = 16.54538144767878549;
        walking_speed = 2.92744955311141464;
        expected = 172.53380703045326072;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {53699, 152973, 53622, 53611, 53614, 53615, 84055, 53617, 53616, 84052, 84053, 53625, 53685, 53686, 45954, 45953, 45952, 45951, 45950};
        turn_penalty = 7.31448838274191271;
        walking_speed = 1.63954236367476947;
        expected = 211.74325155181577429;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {7, 93186, 93187, 22, 24, 116388, 25, 116390, 138052, 127, 110444, 110445, 13750, 183353, 125, 126, 106822, 106823, 214235, 106825, 134887, 134890, 134889, 134888, 82730, 82731, 106824, 110471, 82732, 5352, 134882, 110472, 152454, 115736, 115737};
        turn_penalty = 18.24346895180306305;
        walking_speed = 4.46165579394548306;
        expected = 214.92393258981888948;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {115269, 33058, 33057, 132806, 137430, 137435, 168608, 21541, 129244, 129245, 172532, 129248, 129237, 129238, 129239, 129240};
        turn_penalty = 7.82606251372725037;
        walking_speed = 1.86915130961948428;
        expected = 224.56339723108004591;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {16440, 188558, 161884, 161886, 161885, 213932, 65932, 65931, 65930, 6760, 6759, 183582, 183579, 183578, 183577, 183586, 183583, 210540, 151111, 151113, 151112, 151110, 159190, 159189, 159188, 6673, 6672};
        turn_penalty = 15.36062383199156578;
        walking_speed = 4.20088107805602018;
        expected = 230.40400288673544082;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {188322, 188321, 181052, 181051, 183871, 181048, 181046, 102448, 102503, 102476, 102455, 102491, 213371, 102483, 102464, 102465, 153682, 48387, 216998, 216999, 217006, 217005, 152444, 217003, 217004, 50730, 51747, 48588, 51891, 51661, 47368, 49940, 48090, 49864, 49891};
        turn_penalty = 8.47841745985060946;
        walking_speed = 3.31156119423954554;
        expected = 315.41329235044196366;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {211191, 109285, 156531, 176640, 176641, 81388, 142390, 81389, 81439, 82870, 26130, 37989, 97295, 37990};
        turn_penalty = 2.32310977358730941;
        walking_speed = 2.30440253117113114;
        expected = 554.31382954364357829;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {197290, 197289, 25333, 25334, 25335, 25336, 25337, 25338, 25339, 25340, 25341, 25342, 25343, 25344, 120735, 120730, 120729, 120727, 120717, 120716, 120723, 120722, 120719, 120725, 96815, 96814, 96813, 96812, 96807, 48332, 47864, 34367, 128790, 128791, 120959};
        turn_penalty = 1.30596128124808364;
        walking_speed = 4.26384373645593939;
        expected = 1893.20123926858786945;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

    } //compute_path_travel_time

} //walking_path_cost_public

