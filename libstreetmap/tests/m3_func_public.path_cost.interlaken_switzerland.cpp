#include <random>
#include <unittest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"

#include "unit_test_util.h"
#include "path_verify.h"

using ece297test::relative_error;
using ece297test::path_is_legal;


SUITE(path_cost_public) {
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
        double expected;
        double actual;

        path = {568, 567, 566, 565, 564, 563, 398};
        turn_penalty = 25.16084927376740765;
        expected = 62.45569240537550115;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {500, 499, 138, 152, 484, 485, 1355, 1356, 1293, 1277, 1297};
        turn_penalty = 14.24546986562031847;
        expected = 113.54898555985357689;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {2023, 2176, 1353, 2175, 998, 1846, 1389, 2115, 158, 1544, 78, 79, 80, 1886, 1885, 1884, 1883, 1854, 1853, 1852, 1851, 1129};
        turn_penalty = 2.68072293181142607;
        expected = 121.91284624600798736;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1877, 1878, 1879, 1021, 2061, 31, 30, 29, 219, 218, 217, 216, 215, 214, 1533, 1546, 58, 210, 209, 208, 207, 59, 2059, 1025, 1024, 1023, 1022, 1821, 206, 205, 204, 961, 532, 531};
        turn_penalty = 0.90258765451972178;
        expected = 136.80985185575741525;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {84, 1548, 1549, 1550, 1551, 1552, 1553, 862, 1843, 1848, 863, 1911, 1105, 1104, 1547, 2174, 2176, 1353, 2175, 998};
        turn_penalty = 43.83224127580765384;
        expected = 166.94484476329824929;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1846, 1389, 2115, 158, 1544, 78, 79, 80, 1886, 1885, 993, 995, 1561};
        turn_penalty = 27.22910222223687171;
        expected = 173.92615282043755087;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {562, 677, 678, 679, 680, 681, 409, 634, 633, 632, 631, 630, 566, 567, 568, 569, 570};
        turn_penalty = 32.80671457076476116;
        expected = 180.51256493818888771;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {0, 1, 1940, 1896, 1897, 1898, 1899, 1900, 1901, 1902, 1903, 1904, 1905, 1906, 526, 527, 461, 21, 1119, 1120, 931, 930, 910, 909, 908, 434, 435, 436, 466, 467, 512, 511, 510, 509};
        turn_penalty = 0.00000000000000000;
        expected = 184.34977688633455273;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1593, 1870, 1869, 1868, 1596, 1592, 917, 918, 1603, 1867, 598, 1354, 2016, 2015, 271, 919, 920, 921, 757, 925, 1061, 253, 256, 255, 33, 32, 294, 295, 1216};
        turn_penalty = 6.71398457277002869;
        expected = 186.04232707601249786;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {2079, 2080, 60, 2085, 2131, 2132, 2133, 48, 1761, 2136, 491, 492, 2137, 489, 2134, 490, 2124, 2129, 47, 1938, 2194, 49, 550, 1043, 2100, 56, 760, 761, 1252, 1253, 1254, 1255, 1256, 1836, 837, 478, 2187, 479, 838, 1664, 1663, 1662, 1749};
        turn_penalty = 12.86716044314528418;
        expected = 191.86366140133245040;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {611, 610, 609, 639, 638, 637, 618, 605, 1787, 1786, 199, 744, 745, 1977, 1972, 202, 203, 292, 592, 591, 584, 486, 718, 580, 581, 578, 716, 579, 763};
        turn_penalty = 4.11236351442220283;
        expected = 209.05546092690730120;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {625, 561, 560, 559, 558, 608, 607, 606, 605, 1787, 1786, 199, 743, 82, 176, 1936, 1937, 2014, 1788, 1789, 1790, 1791, 1792, 1793, 1794, 1795, 1796, 1797, 1798, 911, 912, 1961, 1960, 1959, 348, 347, 346, 345, 344, 343, 342, 341, 340, 339, 109, 1307, 1306, 1995, 1994, 1993, 1992, 1991, 1990, 1989, 897, 900, 947};
        turn_penalty = 0.00000000000000000;
        expected = 213.04216964087845554;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1827, 1826, 1014, 1013, 527, 528, 20, 1119, 1120, 931, 930, 910, 909, 908, 720, 719, 189, 1224, 1223, 425, 426, 427, 503, 504, 505, 506};
        turn_penalty = 13.71927624544100688;
        expected = 231.10243946539125659;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1828, 1827, 1826, 688, 689, 690, 691, 692, 693, 1833, 1821, 1022, 1023, 1024, 1025, 2059, 59, 207, 208, 209, 210, 58, 1546, 1533, 1532, 1531, 1545, 78, 79, 80, 192, 193, 1351, 2009, 2010, 2005, 2006, 2007, 2008};
        turn_penalty = 17.26744121680091126;
        expected = 249.73507524928700718;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {426, 425, 1223, 1224, 189, 719, 720, 908, 909, 910, 930, 931, 932, 933, 934, 961, 204, 205, 206, 1821, 1022, 1023, 1024, 1025, 2059, 59, 207, 208, 209, 210, 58, 1546, 1533, 1532, 1531, 1545, 78, 79, 80, 1886, 1885, 1884, 1883, 1855, 1343, 1342, 1341, 1340, 1339, 1338, 221, 1440, 1441, 1442, 1443, 1444, 1445, 1446, 1447, 1448, 1449, 1450, 1039, 1598, 1599, 1600, 1601, 1396, 1392};
        turn_penalty = 5.46601715552688994;
        expected = 316.75677636344323673;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1699, 1703, 1700, 1696, 822, 675, 477, 476, 1299, 1300, 1301, 1302, 1303, 1141, 368, 367, 1965};
        turn_penalty = 31.68914896056834252;
        expected = 344.21014285719405734;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {542, 543, 544, 2094, 2089, 403, 2199, 122, 495, 497, 1985, 1782, 1460, 1459, 161, 85, 1975, 46, 749, 82, 176, 1936, 1937, 2014, 1788, 1789, 1790, 1791, 1792, 1793, 1794, 1795, 1796, 1797, 1798, 911, 912, 1961, 1960, 1959, 348, 347, 346, 353, 354, 355};
        turn_penalty = 10.72199757501731554;
        expected = 346.95837300106938983;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {980, 776, 775, 774, 773, 772, 799, 798, 797, 796, 795, 794, 793, 771, 770, 769, 768, 767, 766, 765, 165, 145, 146, 147, 148, 149, 1850, 1842, 1671, 150, 1839, 1449, 1448, 1447, 1446, 1445, 1444, 1443, 1442, 1441, 1440, 221, 1338, 1339, 1340, 1341, 1342, 1343, 1855, 1883, 1884, 1885, 1886, 192, 193, 1351};
        turn_penalty = 7.96910429139791976;
        expected = 357.15308456294451389;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1129, 1851, 1852, 1853, 1854, 1883, 1884, 1885, 1886, 192, 193, 1351, 2009, 2010, 2005, 2006, 2007, 2008, 2029, 1997, 1998, 898, 899, 1309, 1308, 1994, 1995, 1306, 1307, 109, 339, 340, 341, 342, 343, 344, 345, 353, 354, 355, 359, 360, 361, 102, 103, 1849, 1141, 1303, 1302, 1301, 1300, 1299, 476, 477, 675, 674, 673};
        turn_penalty = 7.92470683533765108;
        expected = 376.66841676809883666;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {729, 296, 295, 294, 32, 33, 255, 256, 253, 1061, 925, 758, 923, 151, 583, 577, 792, 1007, 1008, 582, 165, 765, 766, 767, 768, 769, 770, 771, 793, 794, 795, 796, 797, 798, 799, 772, 773, 774, 775, 776, 777, 778, 779, 780};
        turn_penalty = 31.07746946120639464;
        expected = 420.92130084099676424;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1811, 1812, 1007, 792, 577, 763, 597, 586, 269, 270, 715, 589, 590, 717, 1783, 260, 261, 200, 201, 1973, 175, 1983, 1984, 1785, 160, 1458, 121, 475, 1781, 496, 493, 494, 2198, 403, 2089, 1987, 2046};
        turn_penalty = 29.50603330874932340;
        expected = 423.78717604782997341;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {538, 1026, 1707, 1706, 1705, 1708, 1709, 2158, 2159, 2097, 2046, 164, 2094, 2089, 403, 2199, 42, 2079, 2080, 60, 2085, 2131, 2132, 2133, 48, 1761, 2136, 491, 492, 2137, 489, 2134, 490, 2124, 2129, 47, 1938, 2194, 49, 550, 1043, 2100, 57, 54, 50, 51, 52, 53, 2098, 2099, 1939, 1940, 1896, 1897, 1898, 1899, 1900, 1901, 1902, 1756, 1909, 1910, 946, 1726};
        turn_penalty = 7.66876581015625014;
        expected = 431.26096138700074789;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {608, 607, 606, 605, 1787, 1786, 199, 743, 82, 176, 1936, 1937, 2014, 1788, 1789, 1790, 1791, 1792, 1793, 1794, 1795, 1796, 1797, 1798, 911, 912, 1961, 1960, 1959, 348, 347, 346, 345, 344, 343, 342, 341, 340, 339, 95, 94, 93, 92, 91, 90, 89, 88, 87, 83, 84, 1548, 1333, 1332, 866, 68, 67, 66};
        turn_penalty = 44.11521863240028551;
        expected = 436.99062684295040526;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1615, 1614, 1778, 1775, 1773, 1304, 139, 332, 1312, 1313, 1943, 1945, 1944, 1949, 1948, 1947, 1946, 380, 1953, 1952, 1951, 1950, 97, 96, 1307, 1306, 1995, 1994, 1993, 1992, 1991, 1990, 1989, 897, 900, 2028, 2027, 1336, 1337, 231, 193, 192, 80, 79, 78, 1545, 1531, 1532, 1533, 1546, 58, 210, 209, 208, 207, 59, 2059, 1025, 1024, 1023, 1022, 1821, 206, 205, 204, 961, 934, 933, 932, 931, 930, 910, 909, 908, 434, 435, 436, 437, 438, 1907, 1908, 428, 429, 430, 431, 432, 433, 1062, 1063, 1064, 926, 927, 928, 929, 3, 2226, 2225, 1380, 1379};
        turn_penalty = 0.00000000000000000;
        expected = 437.49024139059241634;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {285, 1647, 1648, 785, 784, 783, 782, 781, 780, 779, 778, 777, 776, 775, 774, 773, 772, 799, 798, 797, 796, 795, 794, 793, 771, 770, 769, 768, 767, 766, 765, 165, 582, 1008, 1007, 792, 577, 763, 597, 586, 269, 270, 715, 589, 590, 717, 1783, 260, 261, 200, 201, 1973, 81, 1937, 1936, 176, 82, 743, 199, 1786, 1787, 605, 606, 607, 608, 953, 954, 393, 394, 395, 396, 397, 563, 564, 565};
        turn_penalty = 3.90960681914189401;
        expected = 459.56613007684779859;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {2012, 344, 345, 346, 347, 348, 1959, 1960, 1961, 912, 911, 1798, 1797, 1796, 1795, 1794, 1793, 1792, 1791, 1790, 1789, 1788, 2014, 1937, 1936, 176, 82, 743, 199, 1786, 1787, 291, 1743, 1744, 656, 1784, 732, 188, 483, 1233, 1234, 1235, 1236, 1058, 369, 370, 1237, 381};
        turn_penalty = 29.15722898780660799;
        expected = 496.34345929940275255;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {488, 1222, 655, 654, 1398, 1745, 1744, 1743, 291, 1787, 1786, 199, 743, 82, 176, 1936, 1937, 2014, 1788, 1789, 1790, 1791, 1792, 1793, 1794, 1795, 1796, 1797, 1798, 911, 912, 1961, 1960, 1959, 348, 347, 346, 345, 344, 343, 342, 341, 340, 339, 109, 1307, 1306, 1995, 1994, 1993, 1992, 1991, 1990, 1989, 897, 900, 2028, 2027, 1336, 1337, 231, 193, 192, 1886, 1885, 1884};
        turn_penalty = 38.78946586502576110;
        expected = 542.84430114416318247;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {25, 24, 23, 22, 21, 20, 528, 527, 526, 1906, 1905, 1904, 1903, 1902, 1901, 1900, 1899, 1898, 1897, 1896, 1940, 1939, 2099, 2098, 53, 52, 51, 50, 54, 57, 2100, 1043, 550, 49, 2194, 1938, 47, 2128, 2125, 2126, 2135, 489, 2138, 2139, 2142, 735, 1782, 1460, 1459, 161, 85, 1975, 46, 749, 82, 176, 1936, 1937, 2014, 1788, 1789, 1790, 1791, 1792, 1793, 1794, 1795, 1796, 1797, 1798, 911, 1151, 1152, 1310, 1311, 1954, 1955, 1956, 1957};
        turn_penalty = 31.34028327739956765;
        expected = 579.98745628080905590;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {2024, 1240, 1239, 1838, 1006, 1005, 1004, 1003, 722, 1597, 1039, 1450, 1449, 1448, 1447, 1446, 1445, 1444, 1443, 1442, 1441, 1440, 221, 1338, 1339, 1340, 1341, 1342, 1343, 1855, 1883, 1884, 1885, 1886, 80, 79, 78, 1545, 1531, 1532, 1533, 1546, 58, 210, 209, 208, 207, 59, 2059, 1025, 1024, 1023, 1022, 1821, 1833, 1895, 1894, 1893, 1892, 1891, 1890, 1889, 1888, 1887, 1909, 1910, 946, 1726, 1727, 1728, 1729, 1730, 1731, 1732, 1733, 1734, 1735, 1736, 1737, 1040, 248, 249, 2193};
        turn_penalty = 38.07595966077786898;
        expected = 668.18077304081691636;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1384, 1383, 282, 789, 788, 787, 786, 785, 784, 783, 782, 781, 780, 779, 778, 777, 776, 775, 774, 773, 772, 799, 798, 797, 796, 795, 794, 793, 771, 770, 769, 768, 767, 766, 765, 165, 582, 1008, 1007, 792, 577, 763, 597, 586, 269, 270, 715, 589, 590, 717, 1783, 260, 261, 200, 201, 1973, 175, 1983, 1984, 1785, 160, 1458, 121, 475, 223, 2124, 2129, 47, 1938, 2194, 49, 550, 1043, 2100, 57, 54, 50, 51, 52, 53, 2098, 2099, 1939, 1940, 1896, 1897, 1898, 1899, 1126, 1125, 1124, 1123, 1122, 15, 16, 1135, 1134};
        turn_penalty = 43.34477103046508972;
        expected = 857.66286090879827952;
        actual = compute_path_travel_time(path, turn_penalty);
        CHECK(relative_error(expected, actual) < 0.001000000);

    } //compute_path_travel_time

} //path_cost_public

