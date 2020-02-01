
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

SUITE(distance_time_queries_perf_public_toronto_canada) {

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


    struct MapFixture : BaseMapFixture {
        MapFixture()
            : BaseMapFixture()
            , rng(3)
            , rand_intersection(0, getNumIntersections()-1)
            , rand_street(1, getNumStreets()-1) // Start from 1 to avoid getting id 0 (<unknown>)
            , rand_segment(0, getNumStreetSegments()-1)
            , rand_poi(0, getNumPointsOfInterest()-1)
            , rand_feature(0, getNumFeatures()-1)
            , rand_node(0, 7780698)
            , rand_way(0, 975994)
            , rand_relation(0, 7144)
            , rand_lat(43.479999542, 43.919982910)
            , rand_lon(-79.789985657, -79.000000000)
        { }

        std::minstd_rand rng;
        std::uniform_int_distribution<int> rand_intersection;
        std::uniform_int_distribution<int> rand_street;
        std::uniform_int_distribution<int> rand_segment;
        std::uniform_int_distribution<int> rand_poi;
        std::uniform_int_distribution<int> rand_feature;
        std::uniform_int_distribution<int> rand_node;
        std::uniform_int_distribution<int> rand_way;
        std::uniform_int_distribution<int> rand_relation;
        std::uniform_real_distribution<double> rand_lat;
        std::uniform_real_distribution<double> rand_lon;
    };

    TEST_FIXTURE(MapFixture, distance_between_two_points_perf) {
        //Verify Functionality
        double expected;

        expected = 2356.64505946482040599;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.79279708862304688, -79.40152740478515625), LatLon(43.78813934326171875, -79.37289428710937500))), 0.001000000);

        expected = 2427.80270119967008213;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.71529388427734375, -79.55553436279296875), LatLon(43.73324203491210938, -79.53834533691406250))), 0.001000000);

        expected = 2949.60709968117043900;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.59196853637695312, -79.37844085693359375), LatLon(43.59408187866210938, -79.41493988037109375))), 0.001000000);

        expected = 3548.76272402025188057;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.82280731201171875, -79.18561553955078125), LatLon(43.83702087402343750, -79.14601898193359375))), 0.001000000);

        expected = 7082.94972707004035328;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.64250946044921875, -79.35969543457031250), LatLon(43.60358810424804688, -79.29006958007812500))), 0.001000000);

        expected = 9273.46528542371379444;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.54529571533203125, -79.10992431640625000), LatLon(43.56031417846679688, -79.22308349609375000))), 0.001000000);

        expected = 9502.16744354860202293;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.71036148071289062, -79.60935211181640625), LatLon(43.65697860717773438, -79.70158386230468750))), 0.001000000);

        expected = 10259.27792697115364717;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.59600830078125000, -79.11286163330078125), LatLon(43.52816772460937500, -79.19910430908203125))), 0.001000000);

        expected = 10811.38925209360422741;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.87495803833007812, -79.56263732910156250), LatLon(43.85248565673828125, -79.43147277832031250))), 0.001000000);

        expected = 14256.83954341826029122;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.61819839477539062, -79.22166442871093750), LatLon(43.72824859619140625, -79.31252288818359375))), 0.001000000);

        expected = 14466.80948114287093631;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.77970123291015625, -79.60786437988281250), LatLon(43.71545028686523438, -79.45131683349609375))), 0.001000000);

        expected = 16151.57112973360381147;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.73253250122070312, -79.09116363525390625), LatLon(43.58732604980468750, -79.09315490722656250))), 0.001000000);

        expected = 17244.38714028189860983;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.56804275512695312, -79.77233886718750000), LatLon(43.69831848144531250, -79.65620422363281250))), 0.001000000);

        expected = 23088.23867834949487587;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.84590911865234375, -79.17972564697265625), LatLon(43.66650009155273438, -79.32427978515625000))), 0.001000000);

        expected = 26697.57579962744057411;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.77907943725585938, -79.08943939208984375), LatLon(43.88920593261718750, -79.38510131835937500))), 0.001000000);

        expected = 28346.94087889042202733;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.57031250000000000, -79.68901824951171875), LatLon(43.77373886108398438, -79.47676086425781250))), 0.001000000);

        expected = 29222.10386375499729184;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.65841293334960938, -79.78363800048828125), LatLon(43.66227722167968750, -79.42051696777343750))), 0.001000000);

        expected = 30851.74677904916461557;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.77691650390625000, -79.58846282958984375), LatLon(43.56536865234375000, -79.34043121337890625))), 0.001000000);

        expected = 31806.78265816414932488;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.71936416625976562, -79.52052307128906250), LatLon(43.89463806152343750, -79.20742797851562500))), 0.001000000);

        expected = 34409.99452569775894517;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.74295806884765625, -79.03385162353515625), LatLon(43.64626312255859375, -79.44029235839843750))), 0.001000000);

        expected = 36763.18711706031172071;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.89926528930664062, -79.00794982910156250), LatLon(43.57030868530273438, -79.05247497558593750))), 0.001000000);

        expected = 38140.77731261219742009;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.87306594848632812, -79.04129028320312500), LatLon(43.88062286376953125, -79.51689147949218750))), 0.001000000);

        expected = 38384.63474691180454101;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.89250946044921875, -79.44643402099609375), LatLon(43.57640457153320312, -79.25479125976562500))), 0.001000000);

        expected = 40275.83257943762146169;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.83801651000976562, -79.22557830810546875), LatLon(43.91736984252929688, -79.71572113037109375))), 0.001000000);

        expected = 41625.99285252964182291;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.56082916259765625, -79.01220703125000000), LatLon(43.72190475463867188, -79.47900390625000000))), 0.001000000);

        expected = 44870.51818089283915469;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.87591171264648438, -79.01165008544921875), LatLon(43.90194320678710938, -79.57025146484375000))), 0.001000000);

        expected = 45647.26265787136071594;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.64857482910156250, -79.20729827880859375), LatLon(43.71326065063476562, -79.76768493652343750))), 0.001000000);

        expected = 50284.51768783960142173;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.90982818603515625, -79.60358428955078125), LatLon(43.48199844360351562, -79.40149688720703125))), 0.001000000);

        expected = 51319.73420976037596120;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.65852737426757812, -79.65677642822265625), LatLon(43.54740142822265625, -79.03836059570312500))), 0.001000000);

        expected = 60998.75850568081659731;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(43.74923324584960938, -79.02179718017578125), LatLon(43.81575775146484375, -79.77580261230468750))), 0.001000000);

        //Generate random inputs
        std::vector<LatLon> latlons_1;
        std::vector<LatLon> latlons_2;
        for(size_t i = 0; i < 30000000; i++) {
            latlons_1.push_back(LatLon(rand_lat(rng), rand_lon(rng)));
            latlons_2.push_back(LatLon(rand_lat(rng), rand_lon(rng)));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(1162);
            double result;
            for(size_t i = 0; i < 30000000; i++) {
                result += find_distance_between_two_points(std::make_pair(latlons_1[i], latlons_2[i]));
            }
        }
    } //distance_between_two_points_perf

    TEST_FIXTURE(MapFixture, street_segment_length_perf) {
        //Verify Functionality
        double expected;

        expected = 4.31280873966657730;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(193330), 0.001000000);

        expected = 9.51348675579425773;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(93960), 0.001000000);

        expected = 11.78072151740800244;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(119434), 0.001000000);

        expected = 15.71349032996043960;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(39152), 0.001000000);

        expected = 17.36607119585176306;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(60964), 0.001000000);

        expected = 17.99349788432688513;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(157923), 0.001000000);

        expected = 18.86090475545442047;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(149295), 0.001000000);

        expected = 20.38139778527202139;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(147907), 0.001000000);

        expected = 20.62569373896849712;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(117289), 0.001000000);

        expected = 22.05094202135635939;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(193882), 0.001000000);

        expected = 22.46923560183619628;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(163656), 0.001000000);

        expected = 23.32245156621486615;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(195352), 0.001000000);

        expected = 25.94955128609908357;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(170026), 0.001000000);

        expected = 29.10016435138784630;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(172867), 0.001000000);

        expected = 32.79992338435404520;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(154431), 0.001000000);

        expected = 42.36715806538496309;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(124726), 0.001000000);

        expected = 43.35451533048303219;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(138059), 0.001000000);

        expected = 43.89667046573810438;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(210188), 0.001000000);

        expected = 45.01237278631888472;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(215943), 0.001000000);

        expected = 46.61367907230290797;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(197221), 0.001000000);

        expected = 47.51506246278246692;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(176229), 0.001000000);

        expected = 47.94623084470583763;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(198105), 0.001000000);

        expected = 62.30570654538148290;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(108303), 0.001000000);

        expected = 122.27786259614822484;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(153092), 0.001000000);

        expected = 188.98141998662543983;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(50528), 0.001000000);

        expected = 223.77606192597608015;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(42525), 0.001000000);

        expected = 238.92309859297819230;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(55911), 0.001000000);

        expected = 242.52749386327775483;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(14), 0.001000000);

        expected = 261.85450993295478384;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(125798), 0.001000000);

        expected = 388.37572511922161311;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(58880), 0.001000000);

        //Generate random inputs
        std::vector<StreetSegmentIndex> segment_ids;
        for(size_t i = 0; i < 1600000; i++) {
            segment_ids.push_back(rand_segment(rng));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(976);
            double result;
            for(size_t i = 0; i < 1600000; i++) {
                result += find_street_segment_length(segment_ids[i]);
            }
        }
    } //street_segment_length_perf

    TEST_FIXTURE(MapFixture, street_segment_travel_time_perf) {
        //Verify Functionality
        double expected;

        expected = 0.31052222925599360;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(193330), 0.001000000);

        expected = 0.84821194925337617;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(119434), 0.001000000);

        expected = 0.85621380802148328;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(93960), 0.001000000);

        expected = 1.13137130375715156;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(39152), 0.001000000);

        expected = 1.25035712610132688;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(60964), 0.001000000);

        expected = 1.29553184767153584;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(157923), 0.001000000);

        expected = 1.35798514239271806;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(149295), 0.001000000);

        expected = 1.46746064053958536;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(147907), 0.001000000);

        expected = 1.48504994920573186;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(117289), 0.001000000);

        expected = 1.58766782553765773;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(193882), 0.001000000);

        expected = 1.61778496333220612;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(163656), 0.001000000);

        expected = 1.67921651276747053;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(195352), 0.001000000);

        expected = 1.86836769259913371;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(170026), 0.001000000);

        expected = 2.09521183329992500;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(172867), 0.001000000);

        expected = 2.36159448367349123;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(154431), 0.001000000);

        expected = 2.60127091982898184;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(138059), 0.001000000);

        expected = 3.05043538070771758;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(124726), 0.001000000);

        expected = 3.16056027353314350;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(210188), 0.001000000);

        expected = 3.24089084061495969;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(215943), 0.001000000);

        expected = 3.35618489320580915;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(197221), 0.001000000);

        expected = 3.42108449732033781;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(176229), 0.001000000);

        expected = 3.45212862081882044;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(198105), 0.001000000);

        expected = 4.48601087126746645;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(108303), 0.001000000);

        expected = 7.33667175576889274;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(153092), 0.001000000);

        expected = 16.11187645867027740;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(42525), 0.001000000);

        expected = 17.00832779879628731;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(50528), 0.001000000);

        expected = 17.46197955815599911;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(14), 0.001000000);

        expected = 21.50307887336803958;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(55911), 0.001000000);

        expected = 23.56690589396593083;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(125798), 0.001000000);

        expected = 34.95381526072994660;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(58880), 0.001000000);

        //Generate random inputs
        std::vector<StreetSegmentIndex> segment_ids;
        for(size_t i = 0; i < 250000000; i++) {
            segment_ids.push_back(rand_segment(rng));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(3887);
            double result;
            for(size_t i = 0; i < 250000000; i++) {
                result += find_street_segment_travel_time(segment_ids[i]);
            }
        }
    } //street_segment_travel_time_perf

    TEST_FIXTURE(MapFixture, feature_area_perf) {
        //Verify Functionality
        double expected;

        expected = 66.07340112893112405;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(104356), 0.001000000);

        expected = 71.21129116485421662;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(332444), 0.001000000);

        expected = 87.33736726268567452;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(318341), 0.001000000);

        expected = 95.06787403459452435;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(397932), 0.001000000);

        expected = 124.63272262217481057;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(516774), 0.001000000);

        expected = 124.89198429577338345;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(113347), 0.001000000);

        expected = 127.69426481997365386;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(149025), 0.001000000);

        expected = 136.62413863844287221;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(436208), 0.001000000);

        expected = 147.89413086730621671;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(156938), 0.001000000);

        expected = 147.95500099936847960;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(408052), 0.001000000);

        expected = 151.60495447376931111;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(460761), 0.001000000);

        expected = 163.67527621678476635;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(520692), 0.001000000);

        expected = 167.55743797275650309;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(453189), 0.001000000);

        expected = 168.32620519620968480;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(411622), 0.001000000);

        expected = 174.61160994397343416;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(288671), 0.001000000);

        expected = 180.69862315020088772;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(162493), 0.001000000);

        expected = 181.33663231218693568;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(560236), 0.001000000);

        expected = 183.26869539283023869;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(575575), 0.001000000);

        expected = 184.86484552246321300;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(394231), 0.001000000);

        expected = 194.20277467068325450;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(525674), 0.001000000);

        expected = 194.60406665242712165;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(469722), 0.001000000);

        expected = 197.62953766085576035;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(420928), 0.001000000);

        expected = 201.12393413109742824;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(367982), 0.001000000);

        expected = 201.57482399822535513;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(528030), 0.001000000);

        expected = 211.42451314563564324;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(335303), 0.001000000);

        expected = 220.47161832955816863;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(515301), 0.001000000);

        expected = 222.39691806219451564;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(312622), 0.001000000);

        expected = 223.74056986623583043;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(250442), 0.001000000);

        expected = 503.41853664836634152;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(134677), 0.001000000);

        expected = 7515.37143515675961680;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(39), 0.001000000);

        //Generate random inputs
        std::vector<FeatureIndex> feature_ids;
        for(size_t i = 0; i < 100000; i++) {
            feature_ids.push_back(rand_feature(rng));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(143);
            double result;
            for(size_t i = 0; i < 100000; i++) {
                result += find_feature_area(feature_ids[i]);
            }
        }
    } //feature_area_perf

    TEST_FIXTURE(MapFixture, way_length_perf) {
        //Verify Functionality
        double expected;

        expected = 42.44327282331557427;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(659531475)), 0.001000000);

        expected = 47.08123518830087306;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(662980285)), 0.001000000);

        expected = 47.09663011675480249;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(658018213)), 0.001000000);

        expected = 48.65584545493011603;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(660067135)), 0.001000000);

        expected = 49.28877472250260894;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(662993225)), 0.001000000);

        expected = 49.64329959217337773;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(662783848)), 0.001000000);

        expected = 49.87255109428355837;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(662786682)), 0.001000000);

        expected = 50.75832923919917761;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(659207136)), 0.001000000);

        expected = 50.76567888113510207;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(660831384)), 0.001000000);

        expected = 53.32896793584890105;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(661822639)), 0.001000000);

        expected = 54.38689375640944945;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(659454621)), 0.001000000);

        expected = 58.18633027838129834;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(661050798)), 0.001000000);

        expected = 58.44325558839419443;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(661262039)), 0.001000000);

        expected = 59.63513268058785854;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(661521432)), 0.001000000);

        expected = 61.17909679638916742;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(662802798)), 0.001000000);

        expected = 63.48520807108371855;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(673877251)), 0.001000000);

        expected = 64.87647164410020650;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(659246911)), 0.001000000);

        expected = 64.98376513423923484;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(661929398)), 0.001000000);

        expected = 70.07022385214352767;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(661027367)), 0.001000000);

        expected = 72.56147777462268778;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(663871364)), 0.001000000);

        expected = 73.04320261550576276;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(660636699)), 0.001000000);

        expected = 74.58012042363435512;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(662123827)), 0.001000000);

        expected = 137.88857107757056042;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(459816669)), 0.001000000);

        expected = 142.61090043548185236;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(110357684)), 0.001000000);

        expected = 260.26848051473501755;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(128831070)), 0.001000000);

        expected = 313.13329670033681396;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(75976176)), 0.001000000);

        expected = 362.23567603323050434;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(75940299)), 0.001000000);

        expected = 441.21805249880281963;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(141171748)), 0.001000000);

        expected = 548.23896512664896363;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(3979831)), 0.001000000);

        expected = 619.74388685741439531;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(93823700)), 0.001000000);

        //Generate random inputs
        std::vector<OSMID> way_ids;
        for(size_t i = 0; i < 100000; i++) {
            int way_idx1 = rand_way(rng);
            OSMID way_id1 = getWayByIndex(way_idx1)->id();
            way_ids.push_back(way_id1);
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(487);
            double result;
            for(size_t i = 0; i < 100000; i++) {
                result += find_way_length(way_ids[i]);
            }
        }
    } //way_length_perf

} //distance_time_queries_perf_public_toronto_canada

