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
    TEST(compute_path_travel_time) {
        std::vector<StreetSegmentIndex> path;
        double turn_penalty;
        double walking_speed;
        double expected;
        double actual;

        path = {62, 1914, 1913};
        turn_penalty = 18.26087682029633541;
        walking_speed = 2.43167678446974200;
        expected = 9.08654868371561264;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {797, 796, 795};
        turn_penalty = 20.52818021603572873;
        walking_speed = 3.33517607336110089;
        expected = 16.17855218510216275;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {285, 1647, 1648, 785, 784, 783, 782, 781};
        turn_penalty = 26.39729671158957203;
        walking_speed = 4.12556905952097619;
        expected = 17.51062623157704934;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {843, 108, 762, 245, 241, 1881, 1882, 1880, 900, 2028, 2027, 1336};
        turn_penalty = 34.51993359273959072;
        walking_speed = 0.86994370304351953;
        expected = 22.74230263060296764;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {377, 378, 379, 97, 96, 109, 339, 340, 341, 342, 343};
        turn_penalty = 34.66734537616928691;
        walking_speed = 2.11775904418693184;
        expected = 24.09447729433993501;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1384, 1383, 282, 789, 788, 787, 786, 785, 784, 783, 782, 781, 780, 779, 778, 777, 776, 775, 774, 773};
        turn_penalty = 37.53834279945513686;
        walking_speed = 2.70788313432656125;
        expected = 28.46868848596223245;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1909, 61, 1906, 11, 12, 1140, 24, 25, 26, 27};
        turn_penalty = 33.54275001978079729;
        walking_speed = 2.71194833105055011;
        expected = 31.30284251206523649;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {2061, 31, 30, 29, 1350, 1349, 1348, 157, 192, 193, 231, 1337, 1336, 2027, 2028, 900, 897, 1989};
        turn_penalty = 29.64832794497377222;
        walking_speed = 3.43498870222091934;
        expected = 36.26389439652371038;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {612, 613, 614, 677, 678, 679, 680, 681, 409, 634, 633, 632, 631, 630, 566, 567, 568, 569, 570, 641};
        turn_penalty = 43.62876494617410117;
        walking_speed = 2.00392001764460481;
        expected = 36.53498406935181464;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {2004, 125, 917, 916, 915, 140, 141, 142};
        turn_penalty = 12.97117716583002434;
        walking_speed = 3.36902423514803484;
        expected = 49.82698116418250578;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1423, 1424, 1425, 1228, 1227, 1226, 1225, 276, 275, 465, 464, 1559, 1335, 274, 1347, 1346, 1345, 1344, 1350, 1349, 1348, 157, 192, 193, 1351, 2009, 2010, 2005, 2006};
        turn_penalty = 35.73388314152779088;
        walking_speed = 4.25714570821250859;
        expected = 50.90231435309129893;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {989, 990, 991, 778, 779, 780, 781, 782, 783, 784, 785, 786, 787, 788, 789, 790, 1819, 1820, 1814, 1815, 1816, 1804, 1805, 808, 809, 1655, 1653};
        turn_penalty = 26.65500735658102016;
        walking_speed = 2.90148829288311383;
        expected = 62.45123349262873802;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {240, 239, 238, 1451, 1338, 1339, 1340, 1341, 1342, 1343, 1855, 1883, 1884, 1885, 1886, 157, 1348, 1349, 1350, 1344, 1345, 1346, 1347};
        turn_penalty = 20.85227712661118815;
        walking_speed = 3.20877666847904308;
        expected = 72.39503541388957331;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1657, 960, 959, 958, 827, 764, 1806, 1805, 1804, 1816, 1815, 1814, 1820, 1819, 790, 789};
        turn_penalty = 20.48237311733243615;
        walking_speed = 3.61309987964352253;
        expected = 76.09253263434325731;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1352, 2175, 998, 1846, 1389, 2114, 1532, 214, 215, 216, 217, 218, 219, 29};
        turn_penalty = 10.52221169261282618;
        walking_speed = 3.24321991146385713;
        expected = 76.47927104470453230;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {2012, 343, 342, 341, 340, 339, 109, 1307, 1306, 1995, 1994, 1993, 1992, 1991, 1990, 1989, 897, 900, 2028, 2027, 1336, 1337, 231, 193, 192, 80, 79, 78, 1545, 1531, 1532, 1533, 1546, 58, 210, 209, 208, 207, 59, 2059, 1025, 1024, 1023, 1022, 1821, 206, 205, 818, 817, 531, 530, 529, 20, 21, 22, 23, 24, 25, 26};
        turn_penalty = 33.99182029046458098;
        walking_speed = 3.46933916344308280;
        expected = 96.15666808056502646;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1876, 1840, 1841, 1048, 308, 309, 310, 311, 312, 313, 314, 315, 1715, 1718, 1238, 1237, 370, 369, 1058, 1236, 1235, 1234, 1190};
        turn_penalty = 15.33745718399443092;
        walking_speed = 3.99376719618737219;
        expected = 96.76599658228177248;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1325, 1324, 1323, 1322, 1321, 1320, 1319, 1318, 1317, 385, 1961, 1960, 1959, 348, 347, 346, 345, 344, 343, 342, 341, 340, 339, 95, 94, 93, 92, 91};
        turn_penalty = 16.81407069785912611;
        walking_speed = 2.18940457641120290;
        expected = 99.78566363986837473;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {750, 82, 176, 1936, 1937, 2014, 1788, 1789, 1790, 1791, 1792, 1793, 1794, 1795, 1796, 1797, 1798, 911, 912, 385, 1317, 1318, 1319, 1320, 1321, 1322, 1323, 1324, 1325, 470, 742, 657, 1942, 386, 388, 1693, 2160};
        turn_penalty = 30.62471546934541422;
        walking_speed = 4.89973621411523919;
        expected = 113.91761750078661919;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1672, 854, 852, 851, 850, 474, 473, 2106, 472, 400, 488, 1222, 655, 654};
        turn_penalty = 15.11409834525278484;
        walking_speed = 2.76050899288038876;
        expected = 124.09052111168355736;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1374, 2226, 3, 929, 928, 927, 926, 1064, 1063, 1062, 433, 432, 431, 430, 429, 428, 1908, 28, 27, 26, 25, 24, 23, 22, 21, 20, 529, 530, 531, 817, 818, 205, 206, 1821, 1022, 1023, 1024, 1025, 2059};
        turn_penalty = 16.63331716611783051;
        walking_speed = 2.63934167393095143;
        expected = 135.10658102140777714;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {556, 557, 412, 413, 414, 415, 944, 945, 1722, 1723, 399, 710, 711, 712, 713, 628, 629, 2086, 857};
        turn_penalty = 9.40665919658631999;
        walking_speed = 4.14471597040462481;
        expected = 149.41041774966728894;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1222, 653, 1747, 1746, 1745, 1744, 1743, 291, 1787, 1786, 199, 743, 82, 176, 1936, 1937, 2014, 1788, 1789, 1790, 1791, 1792, 1793, 1794, 1795, 1796, 1797, 1798, 911, 912, 1961, 1960, 1959, 348, 347, 346, 345, 344, 343, 342, 341, 340, 339, 109, 96, 97, 379, 378, 377, 1591};
        turn_penalty = 16.54538144767878549;
        walking_speed = 2.92744955311141464;
        expected = 172.15904557561191268;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {181, 1791, 1792, 1793, 1794, 1795, 1796, 1797, 1798, 911, 912, 1961, 1960, 1959, 348, 347, 346, 345, 344, 343, 342, 341, 340, 339, 109, 950, 951, 952, 247, 243, 244, 762, 108, 846, 155, 1451, 1338, 156, 1861, 1862, 1863, 1864, 736};
        turn_penalty = 15.36062383199156578;
        walking_speed = 4.20088107805602018;
        expected = 185.89323920101108456;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {0, 1016, 2200, 2201, 51, 50, 54, 57, 2100, 1043, 550, 49, 2194, 1938, 47, 2129, 2124, 223, 475, 121};
        turn_penalty = 18.24346895180306305;
        walking_speed = 4.46165579394548306;
        expected = 194.63531406598650619;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {538, 1026, 1707, 1706, 1705, 1708, 1709, 2158, 2159, 2097, 2046};
        turn_penalty = 7.31448838274191271;
        walking_speed = 1.63954236367476947;
        expected = 204.66083910615250829;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {2169, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 2029, 1997, 1998, 898, 899, 1309, 1308, 1994, 1995, 1306, 1307, 109, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 1959, 356, 1311};
        turn_penalty = 7.82606251372725037;
        walking_speed = 1.86915130961948428;
        expected = 213.96799321272555972;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1699, 1702, 649, 650, 651, 652, 656, 1744, 1743, 291, 605, 606, 607, 608, 953, 551, 552, 553, 554, 555, 556, 557, 412, 413, 564, 565, 566, 567, 568};
        turn_penalty = 8.47841745985060946;
        walking_speed = 3.31156119423954554;
        expected = 324.63222988722048967;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {83, 87, 88, 89, 90, 91, 92, 93, 94, 95, 339, 340, 341, 107, 106, 105, 357};
        turn_penalty = 2.32310977358730941;
        walking_speed = 2.30440253117113114;
        expected = 399.72145315646577046;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1854, 1883, 1884, 1885, 1886, 80, 79, 78, 1545, 1531, 1532, 1533, 1546, 58, 210, 209, 208, 207, 59, 2059, 1025, 1024, 1023, 1022, 1821, 206, 205, 204, 961, 532, 531, 530, 529, 20, 21, 22, 23, 1140, 19};
        turn_penalty = 1.30596128124808364;
        walking_speed = 4.26384373645593939;
        expected = 935.46527171794343758;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

    } //compute_path_travel_time

} //walking_path_cost_public

