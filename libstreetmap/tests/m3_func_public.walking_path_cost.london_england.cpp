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
    TEST(compute_path_travel_time) {
        std::vector<StreetSegmentIndex> path;
        double turn_penalty;
        double walking_speed;
        double expected;
        double actual;

        path = {288116, 288115, 44584, 44583};
        turn_penalty = 18.26087682029633541;
        walking_speed = 2.43167678446974200;
        expected = 14.14436448036499527;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {115913, 115914, 131684, 147105, 276117, 276110, 140232, 140234, 140233, 232393, 232548, 232547, 146119, 275109, 275108, 275107};
        turn_penalty = 34.51993359273959072;
        walking_speed = 0.86994370304351953;
        expected = 23.18376959688377426;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {155349, 155348, 276395, 209547, 46287, 219003};
        turn_penalty = 20.52818021603572873;
        walking_speed = 3.33517607336110089;
        expected = 27.52281052639915870;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {54907, 99186, 99185, 234937, 12045, 12046, 234940, 14818, 142563, 142564, 142565, 157388};
        turn_penalty = 34.66734537616928691;
        walking_speed = 2.11775904418693184;
        expected = 28.73033215221298065;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {141270, 141097, 141098, 141099, 141079, 141080, 141081, 141082, 141083, 141084, 141085, 141086, 141071, 141072, 247888, 245257, 220664, 167404, 167409, 167408};
        turn_penalty = 29.64832794497377222;
        walking_speed = 3.43498870222091934;
        expected = 34.17094642228902046;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {270492, 270493, 270424, 270425, 208515, 208516, 208517, 208518, 208519, 208524, 198256, 208526, 208484, 195670, 284697, 284698, 208533};
        turn_penalty = 33.54275001978079729;
        walking_speed = 2.71194833105055011;
        expected = 36.84458866448905212;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {130843, 88264, 88263, 134383, 17616, 132542, 132543, 132544, 232464, 285523, 285533, 285528, 285529, 153966, 285534, 183975, 279496, 285527};
        turn_penalty = 43.62876494617410117;
        walking_speed = 2.00392001764460481;
        expected = 38.54631559993120504;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {42272, 42271, 42270, 42269, 42268, 42267, 42266, 11290, 11289, 11288, 230728, 230714, 11297};
        turn_penalty = 26.39729671158957203;
        walking_speed = 4.12556905952097619;
        expected = 44.03628450138499062;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {28661, 28660, 272449, 123846, 123845, 123844, 123843, 123842, 306090, 306089, 102655, 306092};
        turn_penalty = 12.97117716583002434;
        walking_speed = 3.36902423514803484;
        expected = 49.63519021549545585;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {192228, 192227, 192226, 192225, 192224, 192223, 192222, 192221, 192220, 193452, 193453, 193454, 193455, 193456, 193457, 297403, 297404, 297405, 297406, 297407, 297408, 297409, 254111, 254112, 254113, 254114, 254115, 254116, 220393, 220392, 220391, 220390, 220389, 220388, 220387, 220386, 220385, 220384, 220383, 220382, 220381, 220380, 220379, 220446, 220445, 220444, 242670, 253772, 253771, 253770, 253769, 253768, 253767, 253766, 253765, 253764, 253763, 253762, 253761, 253760, 253759, 253758, 253757, 253756, 253755, 253754, 253753, 253752, 253751, 253750, 253749, 253748, 253747, 253746, 253745};
        turn_penalty = 37.53834279945513686;
        walking_speed = 2.70788313432656125;
        expected = 54.30927161829053063;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {68140, 303871, 73121, 73120, 73119, 73118, 73035, 255144, 255145, 288970, 288969, 250650, 73000, 72999};
        turn_penalty = 35.73388314152779088;
        walking_speed = 4.25714570821250859;
        expected = 63.79979439035926703;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {189462, 189466, 74721, 74722, 74744, 74743, 74742, 74741, 245643};
        turn_penalty = 10.52221169261282618;
        walking_speed = 3.24321991146385713;
        expected = 79.01117151251540349;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {279745, 291877, 35461, 151665, 279742, 120667, 279744, 120668, 279746, 279747, 229058, 150518, 150519, 150520, 150521, 249408, 249409, 249410, 249411, 249412, 274961, 274962, 274963, 274964, 274965, 274966, 274967, 164909, 58399, 275841, 30554};
        turn_penalty = 20.85227712661118815;
        walking_speed = 3.20877666847904308;
        expected = 85.60641153637475043;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {135111, 135104, 135105, 135106, 135107, 135108, 135109, 135100, 235693, 235694, 187376, 187377, 187378, 187379, 123831, 123825, 123923, 302678, 269563, 269562, 110725};
        turn_penalty = 26.65500735658102016;
        walking_speed = 2.90148829288311383;
        expected = 85.95382746445598343;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {279661, 279662, 285230, 279658, 181620, 181621, 181622, 67104, 284076, 67102, 18958, 278814, 278813, 149327, 168978, 168977};
        turn_penalty = 20.48237311733243615;
        walking_speed = 3.61309987964352253;
        expected = 86.00936952853388107;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {236888, 134740, 148618, 148617, 148616, 148615, 148614, 148613, 148612, 233934, 233933, 233932, 73295, 149476, 72934, 72933};
        turn_penalty = 16.81407069785912611;
        walking_speed = 2.18940457641120290;
        expected = 93.01058714291443152;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {310567, 28458, 28457, 28456, 28455, 28454, 28453, 28452, 28451, 28450, 28449, 28448, 28447, 28446, 28445, 28444, 66246, 236400, 73241, 73240, 73239, 65093, 99137, 284994, 292355, 26623, 26624, 26625, 26626, 26627, 26628, 64909, 64908, 64907, 64906, 64905, 64904, 64903, 64902, 64901, 64900, 64899, 64898, 64897, 64896};
        turn_penalty = 33.99182029046458098;
        walking_speed = 3.46933916344308280;
        expected = 96.97997127606116408;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {243631, 173874, 173873, 173872, 173871, 173870, 173869, 173868, 233836, 233835, 233834, 233837, 288445, 233822, 233821, 233825, 233824, 233823, 233820, 306261, 279637, 224120, 288446, 238481, 138913, 138914, 224119, 224095, 224094, 224098, 224107, 224106, 224110, 159522, 224111, 224113, 288439, 224108, 224109, 224104, 224105, 272250, 130590, 130589, 233675, 264446, 264448, 264447};
        turn_penalty = 15.11409834525278484;
        walking_speed = 2.76050899288038876;
        expected = 114.52974590620605966;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {102609, 102608, 71299, 71297, 168381, 308077, 308076, 124892, 124891, 124890, 124889, 307414, 307416, 307415, 138967, 23179, 23178, 249703, 23180, 249702, 143612, 23192, 23191, 23190, 23189, 23188, 23187, 41731, 272060, 73838, 73839, 140264, 267438, 140262, 31146, 31147, 31148, 74663};
        turn_penalty = 30.62471546934541422;
        walking_speed = 4.89973621411523919;
        expected = 124.69800204792150566;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {213844, 200579, 200578, 200577, 200576, 200575, 200588, 200589, 241269, 200590, 237763, 288256, 172560, 200784, 200785, 200786, 239848, 239847, 239846, 239844, 17284, 17283, 43638, 235948, 307298, 239537, 239546, 227216, 291700, 306777, 306778, 291699, 306780, 291705, 291703, 291702, 291701, 306782, 291704, 306783, 176887, 143725, 134965, 220910, 259822, 259820};
        turn_penalty = 15.33745718399443092;
        walking_speed = 3.99376719618737219;
        expected = 131.84178701946930801;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {190148, 190131, 190161, 190160, 190159, 190158, 190157, 189488, 189489, 189490, 189491, 273656, 273657, 273658, 273659, 273660, 273661, 273662, 273663, 273670, 188511, 196718, 189878, 273698, 273697};
        turn_penalty = 16.63331716611783051;
        walking_speed = 2.63934167393095143;
        expected = 140.07395502672130760;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {277848, 277847, 81081, 62006, 62007, 62008, 158600, 158601, 70958, 70959, 70960, 70961, 297339, 299883, 299882, 299881, 299880, 299879, 190427, 282525, 190429, 190428, 67240, 237993, 237992};
        turn_penalty = 9.40665919658631999;
        walking_speed = 4.14471597040462481;
        expected = 168.72866541193860712;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {297828, 297844, 297840, 297839, 297838, 73094, 73095, 26715, 297850, 297851, 297852, 297853, 253139, 253126, 166760, 253127, 253128, 253129, 253130, 253131, 253132, 253133, 253134, 253135, 253136, 253137, 253138, 10378, 10379, 10380, 10381, 10382, 73590, 73589, 73588, 73587, 73586, 64079};
        turn_penalty = 16.54538144767878549;
        walking_speed = 2.92744955311141464;
        expected = 172.68704302352324476;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {78790, 255668, 255669, 80528, 255667, 255666, 255664, 255661, 271831, 271830, 87835, 87834, 199628, 13867, 13866, 13865, 13864, 13944, 270103, 270104, 270105, 270106, 214539, 301303, 121401, 121402, 121400, 31886, 31887, 31891};
        turn_penalty = 7.31448838274191271;
        walking_speed = 1.63954236367476947;
        expected = 216.12360720163306382;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {25781, 25780, 25779, 25778, 25777, 110728, 155126, 117425, 75449, 110474, 52577, 25264, 25270, 110477, 273033, 273026, 273035, 273032, 174242, 267920, 267921, 267922};
        turn_penalty = 15.36062383199156578;
        walking_speed = 4.20088107805602018;
        expected = 218.13412692501705692;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {288736, 154049, 153543, 153544, 153545, 38912, 38913, 38914, 248477, 248476, 248475, 248474, 112891, 112892, 154315, 154314, 79500, 79499, 79498};
        turn_penalty = 7.82606251372725037;
        walking_speed = 1.86915130961948428;
        expected = 220.29072606565705428;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {18, 19, 261494, 149055, 149056, 149057, 149058, 149059, 149060, 149061, 149062, 149063, 149064, 149065, 149066, 149067, 149068, 149069, 308909, 308908, 272513, 272512, 64236, 64235, 154917, 154918, 215212, 253992, 13089, 13088, 13087, 117180, 202673, 13691, 13690, 13689, 13688, 202675, 13670, 13671, 202788, 202789, 202790};
        turn_penalty = 18.24346895180306305;
        walking_speed = 4.46165579394548306;
        expected = 221.10466196995233190;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {262421, 262422, 262423, 262424, 262425, 262426, 262427, 262428, 262418, 188868, 188869, 234415, 234414, 128599, 276585, 276584, 276583, 276582, 276581, 79574, 79573, 79572, 79571, 79570, 79569, 276586, 79568, 79567, 79566, 79565, 79564, 277131, 277132, 156895, 17396, 17395, 17394, 17393, 17392, 17391, 17390, 34444};
        turn_penalty = 8.47841745985060946;
        walking_speed = 3.31156119423954554;
        expected = 326.97098216963144068;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {12589, 12590, 265303, 265307, 108673, 246923, 245584};
        turn_penalty = 2.32310977358730941;
        walking_speed = 2.30440253117113114;
        expected = 508.45032576125862533;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {267359, 267358, 59263, 59262, 229595, 229606, 229605, 229604, 229603, 229602, 229601, 229600, 229599, 229598, 229597, 272745, 131828, 131827, 131826, 131825, 59155, 59165, 59166, 59167, 59168, 59178, 59177, 59176, 59175, 285238, 285266, 285267, 285271, 285272, 285273, 285274, 285275, 285276};
        turn_penalty = 1.30596128124808364;
        walking_speed = 4.26384373645593939;
        expected = 1655.80994365115452638;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

    } //compute_path_travel_time

} //walking_path_cost_public

