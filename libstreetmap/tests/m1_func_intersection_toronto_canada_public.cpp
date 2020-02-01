
#include "m1.h"
#include "unit_test_util.h"

#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"

#include <unittest++/UnitTest++.h>

#include <random>
#include <algorithm>
#include <set>

using ece297test::relative_error;
using ece297test::sorted;

SUITE(intersection_queries_public_toronto_canada) {

    struct BaseMapFixture {
        BaseMapFixture() {
            //Load the map
            try {
                load_map("/cad2/ece297s/public/maps/toronto_canada.streets.bin");
            } catch (...) {
                std::cout << "!!!! BaseMapFixture test setup: load_map threw an exceptinon !!!!" << std::endl;
                throw; // re-throw exceptinon
            }
        }
    
        ~BaseMapFixture() {
            //Clean-up
            try {
                close_map();
            } catch (const std::exception& e) {
                std::cout << "!!!! BaseMapFixture test teardown: close_map threw an exceptinon. what(): " << e.what() << " !!!!" << std::endl;
                std::terminate(); // we're in a destructor
            } catch (...) {
                std::cout << "!!!! BaseMapFixture test teardown: close_map threw an exceptinon !!!!" << std::endl;
                std::terminate(); // we're in a destructor
            }
        }
    };


    struct MapFixture : BaseMapFixture {};

    TEST_FIXTURE(MapFixture, intersection_street_segments) {
        std::vector<StreetSegmentIndex> expected;

        expected = {5, 7811, 102107};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(10)));

        expected = {24821, 24822, 36814};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(27101)));

        expected = {27049, 27050, 27056};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(29435)));

        expected = {32703, 75704, 75705};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(34975)));

        expected = {36718, 36719, 36721};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(38701)));

        expected = {39235, 39236, 39323};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(40756)));

        expected = {40679, 124834, 212566, 212567};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(42199)));

        expected = {68750, 68767, 128254};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(65038)));

        expected = {82678, 136261, 136262};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(74966)));

        expected = {91207, 91431};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(81186)));

        expected = {93174, 180669, 180673, 180682};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(82672)));

        expected = {98040, 98041, 106569};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(86334)));

        expected = {99080, 99154, 99158};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(87076)));

        expected = {111298, 111318, 111322, 141351, 159502};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(95563)));

        expected = {121876};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(102380)));

        expected = {123437, 123438, 134241, 134243};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(103341)));

        expected = {127437, 127460, 127461};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(105969)));

        expected = {128935, 128936, 128949};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(106896)));

        expected = {132911, 132912, 132922};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(109313)));

        expected = {139808, 139818, 139819};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(113281)));

        expected = {147526, 181228, 181237};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(117691)));

        expected = {151332, 218770};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(119658)));

        expected = {155838, 155843};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(121985)));

        expected = {178999, 179000, 179023};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(133821)));

        expected = {179810, 218811};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(134204)));

        expected = {182658, 182659, 195593};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(135221)));

        expected = {186606, 186609, 186610};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(136515)));

        expected = {187909, 187910, 187918, 187919};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(137127)));

        expected = {202703, 202704, 211356};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(145491)));

        expected = {213128, 213131, 213132};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_segments_of_intersection(149474)));

    } //intersection_street_segments

    TEST_FIXTURE(MapFixture, intersection_street_names) {
        std::vector<std::string> expected;

        expected = {"<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(102380)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(121985)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(38701)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(40756)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(87076)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(105969)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(106896)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(109313)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(113281)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(133821)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(136515)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(145491)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(149474)));

        expected = {"<unknown>", "<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(137127)));

        expected = {"<unknown>", "<unknown>", "<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(95563)));

        expected = {"<unknown>", "Bovaird Drive East", "Bovaird Drive East"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(117691)));

        expected = {"<unknown>", "Denison Street", "Denison Street"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(135221)));

        expected = {"<unknown>", "Highway 401 Express", "Highway 401 Express"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(10)));

        expected = {"<unknown>", "Kitimat Road", "Kitimat Road"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(27101)));

        expected = {"<unknown>", "North Service Road", "North Service Road"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(86334)));

        expected = {"<unknown>", "South Millway", "South Millway"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(34975)));

        expected = {"Athabasca Common", "Wheat Boom Drive"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(134204)));

        expected = {"Claremont Street", "Queen Street West", "Queen Street West"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(74966)));

        expected = {"Constitution Boulevard", "Constitution Boulevard", "Vera Cruz Drive"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(29435)));

        expected = {"Don Mills Road", "Don Mills Road", "Eglinton Avenue East", "Eglinton Avenue East"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(42199)));

        expected = {"Don Mills Road", "Don Mills Road", "York Mills Road", "York Mills Road"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(103341)));

        expected = {"Hurontario Street", "Hurontario Street", "Queensway", "Queensway East"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(82672)));

        expected = {"Isabella Street", "South Unionville Avenue", "South Unionville Avenue"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(65038)));

        expected = {"Moraine Drive", "Moraine Drive"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(81186)));

        expected = {"The East Mall", "The East Mall"};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_names_of_intersection(119658)));

    } //intersection_street_names

    TEST_FIXTURE(MapFixture, directly_connected) {
        ECE297_CHECK(!are_directly_connected(std::make_pair(10229, 96006)));

        ECE297_CHECK(!are_directly_connected(std::make_pair(28262, 56035)));

        ECE297_CHECK(!are_directly_connected(std::make_pair(32615, 87321)));

        ECE297_CHECK(!are_directly_connected(std::make_pair(33063, 142972)));

        ECE297_CHECK(!are_directly_connected(std::make_pair(38216, 82535)));

        ECE297_CHECK(!are_directly_connected(std::make_pair(48021, 35797)));

        ECE297_CHECK(!are_directly_connected(std::make_pair(72388, 48326)));

        ECE297_CHECK(!are_directly_connected(std::make_pair(101772, 51749)));

        ECE297_CHECK(!are_directly_connected(std::make_pair(117913, 42614)));

        ECE297_CHECK(!are_directly_connected(std::make_pair(121164, 81132)));

        ECE297_CHECK(!are_directly_connected(std::make_pair(125686, 45024)));

        ECE297_CHECK(!are_directly_connected(std::make_pair(128969, 111876)));

        ECE297_CHECK(!are_directly_connected(std::make_pair(140389, 82634)));

        ECE297_CHECK(are_directly_connected(std::make_pair(10, 10)));

        ECE297_CHECK(are_directly_connected(std::make_pair(27101, 38792)));

        ECE297_CHECK(are_directly_connected(std::make_pair(38701, 38701)));

        ECE297_CHECK(are_directly_connected(std::make_pair(40756, 40808)));

        ECE297_CHECK(are_directly_connected(std::make_pair(74966, 1109)));

        ECE297_CHECK(are_directly_connected(std::make_pair(82672, 24063)));

        ECE297_CHECK(are_directly_connected(std::make_pair(95563, 114114)));

        ECE297_CHECK(are_directly_connected(std::make_pair(105969, 105992)));

        ECE297_CHECK(are_directly_connected(std::make_pair(106896, 106897)));

        ECE297_CHECK(are_directly_connected(std::make_pair(109313, 109319)));

        ECE297_CHECK(are_directly_connected(std::make_pair(113281, 113278)));

        ECE297_CHECK(are_directly_connected(std::make_pair(117691, 134829)));

        ECE297_CHECK(are_directly_connected(std::make_pair(119658, 35816)));

        ECE297_CHECK(are_directly_connected(std::make_pair(121985, 121984)));

        ECE297_CHECK(are_directly_connected(std::make_pair(122694, 122695)));

        ECE297_CHECK(are_directly_connected(std::make_pair(135221, 102630)));

        ECE297_CHECK(are_directly_connected(std::make_pair(137127, 137128)));

    } //directly_connected

    TEST_FIXTURE(MapFixture, adjacent_intersections) {
        std::vector<IntersectionIndex> expected;

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(4338)));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(112860)));

        expected = {1236, 76603, 76604};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(76602)));

        expected = {3709};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(3708)));

        expected = {4056, 4058, 143782};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(4057)));

        expected = {7020, 7021, 17415};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(7019)));

        expected = {7989};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(88834)));

        expected = {13853, 74339, 136641};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(74338)));

        expected = {14666, 106912};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(107457)));

        expected = {27998, 89642, 89644};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(89643)));

        expected = {34494, 89805};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(34495)));

        expected = {35507, 35510, 35786, 35790};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(35785)));

        expected = {38811, 38813, 38814};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(38812)));

        expected = {41550, 41553, 73705};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(41552)));

        expected = {42354, 42363, 117651};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(42361)));

        expected = {42860, 42862, 42997};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(42861)));

        expected = {47152, 47154};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(47153)));

        expected = {47902, 51507, 52533, 104934};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(47903)));

        expected = {50295, 51378, 51640};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(51497)));

        expected = {62341, 62354};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(62342)));

        expected = {66516, 66518, 66543};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(66517)));

        expected = {76845, 76848, 76850};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(76851)));

        expected = {93448, 93450};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(109721)));

        expected = {97050, 124625, 130211};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(124624)));

        expected = {108588};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(108587)));

        expected = {123751, 123770, 123772};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(123773)));

        expected = {125236, 125238, 125240};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(125237)));

        expected = {126916, 126932, 126936};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(126931)));

        expected = {131029, 131030, 131031};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(131032)));

        expected = {143484, 143520, 143525};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(143527)));

    } //adjacent_intersections

} //intersection_queries_public_toronto_canada

