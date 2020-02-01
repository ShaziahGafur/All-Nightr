
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

SUITE(distance_time_queries_public_saint_helena) {

    struct BaseMapFixture {
        BaseMapFixture() {
            //Load the map
            try {
                load_map("/cad2/ece297s/public/maps/saint-helena.streets.bin");
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

    TEST_FIXTURE(MapFixture, distance_between_two_points) {
        double expected;

        expected = 533.67273782537722582;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(-15.96500301361083984, -5.73743915557861328), LatLon(-15.96105766296386719, -5.73459911346435547))), 0.001000000);

        expected = 781.04487220694420557;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(-15.94136810302734375, -5.67632865905761719), LatLon(-15.93824386596679688, -5.66978836059570312))), 0.001000000);

        expected = 1555.20541039505997105;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(-15.98100376129150391, -5.70508718490600586), LatLon(-15.98956108093261719, -5.69358444213867188))), 0.001000000);

        expected = 2087.76344887593995736;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(-15.96608734130859375, -5.74632930755615234), LatLon(-15.97782325744628906, -5.76156663894653320))), 0.001000000);

        expected = 2381.66534915951342555;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(-15.92990398406982422, -5.73861217498779297), LatLon(-15.93484401702880859, -5.71694469451904297))), 0.001000000);

        expected = 3180.64645869247715382;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(-15.95084381103515625, -5.74608373641967773), LatLon(-15.96496868133544922, -5.72022294998168945))), 0.001000000);

        expected = 3550.67153415110124115;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(-15.96121406555175781, -5.66072654724121094), LatLon(-15.99313545227050781, -5.66105508804321289))), 0.001000000);

        expected = 3788.70268015585224930;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(-15.99737358093261719, -5.77325439453125000), LatLon(-15.96873569488525391, -5.75406932830810547))), 0.001000000);

        expected = 6228.83780151572682371;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(-15.99687480926513672, -5.75948953628540039), LatLon(-15.95215511322021484, -5.72442626953125000))), 0.001000000);

        expected = 6778.89342495606342709;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(-15.95145702362060547, -5.74287843704223633), LatLon(-15.99796295166015625, -5.70190429687500000))), 0.001000000);

        expected = 6997.49198010715008422;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(-15.96410846710205078, -5.73165512084960938), LatLon(-15.92557716369628906, -5.67993307113647461))), 0.001000000);

        expected = 8874.92889832758010016;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(-15.93802452087402344, -5.68293094635009766), LatLon(-15.92058086395263672, -5.76390171051025391))), 0.001000000);

        expected = 9890.39729273709417612;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(-15.92969417572021484, -5.64759063720703125), LatLon(-15.92397212982177734, -5.73987007141113281))), 0.001000000);

        expected = 11053.46618153271447227;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(-15.92223834991455078, -5.74537706375122070), LatLon(-16.01629066467285156, -5.71199226379394531))), 0.001000000);

        expected = 13419.90131970157017349;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(std::make_pair(LatLon(-15.95754241943359375, -5.64926624298095703), LatLon(-15.94291782379150391, -5.77382612228393555))), 0.001000000);

    } //distance_between_two_points

    TEST_FIXTURE(MapFixture, street_segment_length) {
        double expected;

        expected = 6.01499246631190587;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(266), 0.001000000);

        expected = 15.11445034174861135;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(349), 0.001000000);

        expected = 37.39519121424380188;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(0), 0.001000000);

        expected = 40.21689070648031361;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(315), 0.001000000);

        expected = 57.06124089315957804;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(119), 0.001000000);

        expected = 97.37768341259490512;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(125), 0.001000000);

        expected = 126.27513995861261265;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(318), 0.001000000);

        expected = 207.20894630133020087;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(107), 0.001000000);

        expected = 254.32475817565372722;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(423), 0.001000000);

        expected = 297.66580288127738640;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(363), 0.001000000);

        expected = 319.45995523208523537;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(200), 0.001000000);

        expected = 351.31106607031216527;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(255), 0.001000000);

        expected = 580.07266661545077113;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(90), 0.001000000);

        expected = 591.87589918236017184;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(376), 0.001000000);

        expected = 964.85577290594585520;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(369), 0.001000000);

    } //street_segment_length

    TEST_FIXTURE(MapFixture, street_segment_travel_time) {
        double expected;

        expected = 0.43307945757445721;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(266), 0.001000000);

        expected = 1.36030053075737500;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(349), 0.001000000);

        expected = 2.99939517651367282;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(315), 0.001000000);

        expected = 3.36556720928194242;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(0), 0.001000000);

        expected = 5.13551168038436234;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(119), 0.001000000);

        expected = 8.76399150713354125;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(125), 0.001000000);

        expected = 9.41766106359938426;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(318), 0.001000000);

        expected = 18.64880516711971481;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(107), 0.001000000);

        expected = 21.43193780745196975;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(363), 0.001000000);

        expected = 22.88922823580884014;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(423), 0.001000000);

        expected = 25.29439675706247215;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(255), 0.001000000);

        expected = 28.75139597088767118;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(200), 0.001000000);

        expected = 52.20653999539057111;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(90), 0.001000000);

        expected = 53.26883092641241291;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(376), 0.001000000);

        expected = 69.46961564922810339;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(369), 0.001000000);

    } //street_segment_travel_time

    TEST_FIXTURE(MapFixture, feature_area) {
        double expected;

        expected = 0.00000000000000000;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(475), 0.001000000);

        expected = 0.00000000000000000;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(651), 0.001000000);

        expected = 26.45990824007036579;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(1548), 0.001000000);

        expected = 53.86393759098163514;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(1549), 0.001000000);

        expected = 131.82321832665618899;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(1193), 0.001000000);

        expected = 141.96979027095352421;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(2354), 0.001000000);

        expected = 195.27680842449041165;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(1176), 0.001000000);

        expected = 207.21115869195008941;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(1543), 0.001000000);

        expected = 226.62922398060780438;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(1556), 0.001000000);

        expected = 247.44370927806915006;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(1542), 0.001000000);

        expected = 273.03720445783648074;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(139), 0.001000000);

        expected = 349.67657555903565481;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(421), 0.001000000);

        expected = 506.12739842822099945;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(346), 0.001000000);

        expected = 714.75287182057638802;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(1481), 0.001000000);

        expected = 4536.42077741445609718;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_feature_area(342), 0.001000000);

    } //feature_area

    TEST_FIXTURE(MapFixture, way_length) {
        double expected;

        expected = 43.91670281573115631;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(255618582)), 0.001000000);

        expected = 45.66610007404617022;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(413442648)), 0.001000000);

        expected = 47.13450683527246099;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(541766670)), 0.001000000);

        expected = 47.59622239011629574;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(557556635)), 0.001000000);

        expected = 53.68795698977002928;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(658849068)), 0.001000000);

        expected = 55.15378643279508708;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(557203489)), 0.001000000);

        expected = 56.74322587920835304;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(557556580)), 0.001000000);

        expected = 60.14952353067982926;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(205324675)), 0.001000000);

        expected = 68.68037142615564505;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(556912631)), 0.001000000);

        expected = 76.58109257224683120;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(166651789)), 0.001000000);

        expected = 110.72009139037993464;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(500944189)), 0.001000000);

        expected = 380.93621551312452311;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(557556535)), 0.001000000);

        expected = 685.02179265005304387;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(255707511)), 0.001000000);

        expected = 3573.74636234848912864;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(556912652)), 0.001000000);

        expected = 19095.80119767479845905;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_way_length(OSMID(23087262)), 0.001000000);

    } //way_length

} //distance_time_queries_public_saint_helena

