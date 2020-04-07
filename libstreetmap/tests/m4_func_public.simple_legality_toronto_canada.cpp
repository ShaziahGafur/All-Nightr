#include <random>
#include <iostream>
#include <unittest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"
#include "m4.h"

#include "unit_test_util.h"
#include "courier_verify.h"

using ece297test::relative_error;
using ece297test::courier_path_is_legal_with_capacity;


SUITE(simple_legality_toronto_canada_public) {
    TEST(simple_legality_toronto_canada) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<IntersectionIndex> depots;
        std::vector<CourierSubpath> result_path;
        float turn_penalty;
        float truck_capacity;

        deliveries = {DeliveryInfo(12329, 12927, 50.03543), DeliveryInfo(15162, 71331, 60.50912), DeliveryInfo(147022, 141242, 35.54058)};
        depots = {29107, 44932, 40220};
        turn_penalty = 15.000000000;
        truck_capacity = 13465.874023438;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(8961, 25655, 68.27714), DeliveryInfo(146720, 111130, 5.74371), DeliveryInfo(139180, 109292, 54.44788), DeliveryInfo(611, 97791, 53.46008), DeliveryInfo(115131, 136402, 171.11781)};
        depots = {137819, 130433, 98839};
        turn_penalty = 15.000000000;
        truck_capacity = 8434.584960938;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(25800, 30752, 156.64452)};
        depots = {6};
        turn_penalty = 15.000000000;
        truck_capacity = 5000.000000000;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(8779, 61994, 182.53178), DeliveryInfo(24894, 122514, 192.09618), DeliveryInfo(32775, 62947, 74.25520), DeliveryInfo(45889, 149900, 172.51857), DeliveryInfo(107436, 82717, 44.38072)};
        depots = {54560, 112316, 40555};
        turn_penalty = 15.000000000;
        truck_capacity = 7508.874511719;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(18461, 126220, 122.91624), DeliveryInfo(18461, 77238, 47.65577), DeliveryInfo(71346, 122474, 162.54781), DeliveryInfo(146187, 36121, 27.45177), DeliveryInfo(5162, 100304, 126.61769), DeliveryInfo(18461, 56999, 191.67456), DeliveryInfo(146187, 146650, 124.65140), DeliveryInfo(50528, 11072, 90.33015)};
        depots = {15009, 115194, 22009};
        turn_penalty = 15.000000000;
        truck_capacity = 10859.388671875;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(28038, 28409, 115.91381), DeliveryInfo(33220, 131157, 75.65656)};
        depots = {93593};
        turn_penalty = 15.000000000;
        truck_capacity = 1328.184082031;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(28232, 120353, 35.08236), DeliveryInfo(82446, 60062, 60.36134), DeliveryInfo(82070, 120267, 102.00575)};
        depots = {37093, 52386, 34604};
        turn_penalty = 15.000000000;
        truck_capacity = 8968.174804688;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(57556, 4544, 159.20335)};
        depots = {141989};
        turn_penalty = 15.000000000;
        truck_capacity = 5000.000000000;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(60078, 74045, 28.59369), DeliveryInfo(49969, 12547, 46.00364), DeliveryInfo(52690, 111006, 41.82777)};
        depots = {2201, 67523, 62560};
        turn_penalty = 15.000000000;
        truck_capacity = 5104.119140625;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(68638, 141581, 4.68502), DeliveryInfo(90147, 129274, 150.65356)};
        depots = {24950};
        turn_penalty = 15.000000000;
        truck_capacity = 14852.536132812;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(70757, 43226, 55.20467), DeliveryInfo(138790, 119668, 99.69247), DeliveryInfo(62765, 133401, 6.81267), DeliveryInfo(55302, 133401, 119.94157), DeliveryInfo(54323, 133401, 54.26281), DeliveryInfo(60196, 74603, 196.75076), DeliveryInfo(134068, 74603, 155.84526), DeliveryInfo(90750, 65433, 184.55672)};
        depots = {58192, 62673, 2471};
        turn_penalty = 15.000000000;
        truck_capacity = 10305.068359375;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(72612, 95315, 175.53738)};
        depots = {35408, 24013};
        turn_penalty = 15.000000000;
        truck_capacity = 5000.000000000;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(85070, 25477, 72.53727)};
        depots = {58214, 67848};
        turn_penalty = 15.000000000;
        truck_capacity = 5000.000000000;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(73887, 27170, 24.15597)};
        depots = {79772};
        turn_penalty = 15.000000000;
        truck_capacity = 5000.000000000;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(76795, 89322, 131.37521), DeliveryInfo(30782, 18180, 117.05125), DeliveryInfo(19261, 125171, 77.03854), DeliveryInfo(46399, 3050, 59.02165), DeliveryInfo(76795, 21865, 13.02049), DeliveryInfo(76795, 2854, 112.16380), DeliveryInfo(79282, 89690, 34.13472), DeliveryInfo(19261, 86061, 127.61972)};
        depots = {76309, 119360, 98587};
        turn_penalty = 15.000000000;
        truck_capacity = 13573.001953125;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(108258, 6283, 16.54290), DeliveryInfo(35534, 45375, 142.46921), DeliveryInfo(48681, 44490, 131.73322), DeliveryInfo(107008, 80881, 93.85989), DeliveryInfo(52543, 17708, 110.77976)};
        depots = {123110, 124907, 60979};
        turn_penalty = 15.000000000;
        truck_capacity = 10332.823242188;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(110416, 35034, 148.86475), DeliveryInfo(36719, 109245, 186.24405), DeliveryInfo(36758, 43754, 49.75138), DeliveryInfo(27991, 109245, 186.60216), DeliveryInfo(110416, 12065, 124.07938), DeliveryInfo(142087, 104374, 35.82661), DeliveryInfo(110416, 43754, 50.41379), DeliveryInfo(110416, 43754, 50.41379), DeliveryInfo(36758, 109245, 90.22083)};
        depots = {2149, 8770, 79755};
        turn_penalty = 15.000000000;
        truck_capacity = 5430.350585938;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(114707, 97409, 19.22806), DeliveryInfo(114707, 149378, 113.67527), DeliveryInfo(44068, 71656, 157.47266), DeliveryInfo(15351, 25583, 90.84747), DeliveryInfo(71966, 25583, 162.92372), DeliveryInfo(114707, 149378, 113.67527), DeliveryInfo(26607, 25583, 129.00230), DeliveryInfo(26607, 149378, 70.30033), DeliveryInfo(114707, 97555, 67.31814)};
        depots = {10521, 87530, 132952};
        turn_penalty = 15.000000000;
        truck_capacity = 7369.531250000;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(116379, 4090, 91.05830), DeliveryInfo(92573, 108351, 75.33926), DeliveryInfo(111347, 144633, 101.23453), DeliveryInfo(127539, 125660, 92.10358), DeliveryInfo(145824, 132809, 131.64763)};
        depots = {72445, 77403, 151176};
        turn_penalty = 15.000000000;
        truck_capacity = 12779.961914062;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(115439, 7397, 35.97311), DeliveryInfo(115439, 113343, 57.91961), DeliveryInfo(115439, 119074, 37.27143), DeliveryInfo(59749, 133548, 44.35627), DeliveryInfo(32218, 133548, 113.11504), DeliveryInfo(63464, 111533, 129.07108), DeliveryInfo(40666, 133548, 190.33194), DeliveryInfo(59749, 113343, 121.80888), DeliveryInfo(115439, 113343, 57.91961)};
        depots = {88064, 129225, 40488};
        turn_penalty = 15.000000000;
        truck_capacity = 13193.651367188;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(118269, 57912, 66.15946), DeliveryInfo(139133, 148921, 45.42579)};
        depots = {20239, 100949};
        turn_penalty = 15.000000000;
        truck_capacity = 11119.174804688;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(134045, 11317, 1.66533), DeliveryInfo(141824, 23975, 69.64507)};
        depots = {39080, 52954};
        turn_penalty = 15.000000000;
        truck_capacity = 2802.345214844;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(25946, 57293, 159.62805), DeliveryInfo(50270, 137108, 6.15951), DeliveryInfo(87921, 82445, 125.63981), DeliveryInfo(147881, 56587, 159.22833), DeliveryInfo(129004, 146094, 111.09024)};
        depots = {82340, 106323, 65805};
        turn_penalty = 15.000000000;
        truck_capacity = 3171.211669922;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(91246, 77594, 173.83640), DeliveryInfo(78459, 149884, 56.51704), DeliveryInfo(123952, 118080, 102.99988), DeliveryInfo(47302, 55608, 127.61862), DeliveryInfo(91724, 55608, 44.21967), DeliveryInfo(75263, 143042, 78.60152), DeliveryInfo(51383, 118080, 18.57643), DeliveryInfo(91583, 55608, 107.25372)};
        depots = {119721, 76325, 140956};
        turn_penalty = 15.000000000;
        truck_capacity = 10054.899414062;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(121748, 35896, 46.58144), DeliveryInfo(136475, 46812, 168.12914), DeliveryInfo(143981, 123137, 176.08020), DeliveryInfo(38830, 46812, 38.50279), DeliveryInfo(92254, 123137, 94.24657), DeliveryInfo(15217, 124110, 161.54309), DeliveryInfo(63152, 74188, 132.61261), DeliveryInfo(53219, 46812, 167.31036)};
        depots = {143249, 48279, 73609};
        turn_penalty = 15.000000000;
        truck_capacity = 10738.934570312;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(149400, 82788, 28.66476)};
        depots = {30205, 104659};
        turn_penalty = 15.000000000;
        truck_capacity = 5000.000000000;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(123446, 89214, 184.61324), DeliveryInfo(132366, 125813, 65.88518)};
        depots = {121217};
        turn_penalty = 15.000000000;
        truck_capacity = 10739.620117188;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(135247, 71705, 76.93291), DeliveryInfo(132453, 39556, 28.54039), DeliveryInfo(126973, 138276, 73.34542), DeliveryInfo(34633, 93377, 56.93892), DeliveryInfo(70471, 25038, 98.85957)};
        depots = {83954, 33055, 45796};
        turn_penalty = 15.000000000;
        truck_capacity = 3812.814697266;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(149377, 23381, 65.56896), DeliveryInfo(60236, 101600, 182.31299)};
        depots = {125006, 124066};
        turn_penalty = 15.000000000;
        truck_capacity = 2286.546142578;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(133538, 96379, 20.42503), DeliveryInfo(92093, 136264, 120.76764), DeliveryInfo(92093, 10558, 174.53944), DeliveryInfo(34526, 72762, 166.97858), DeliveryInfo(54243, 6495, 193.52501), DeliveryInfo(92093, 31768, 94.40701), DeliveryInfo(101710, 100123, 45.52258), DeliveryInfo(34526, 15856, 22.48336)};
        depots = {140926, 67424, 136104};
        turn_penalty = 15.000000000;
        truck_capacity = 10248.227539062;
        result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

    } //simple_legality_toronto_canada

} //simple_legality_toronto_canada_public

