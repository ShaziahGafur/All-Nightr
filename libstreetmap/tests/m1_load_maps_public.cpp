#include <unittest++/UnitTest++.h>
#include "m1.h"

std::string map_dir = "/cad2/ece297s/public/maps/";

SUITE(M1_Public_Load_Maps) {
    TEST(load_saint_helena) {
        {
            UNITTEST_TIME_CONSTRAINT(3000);
            CHECK(load_map(map_dir + "saint-helena.streets.bin"));
        }
        close_map();
    }
    
    TEST(load_hamilton) {
        {
            UNITTEST_TIME_CONSTRAINT(3000);
            CHECK(load_map(map_dir + "hamilton_canada.streets.bin"));
        }
        close_map();
    }

    TEST(load_moscow) {
        {
            UNITTEST_TIME_CONSTRAINT(10000);
            CHECK(load_map(map_dir + "moscow_russia.streets.bin"));
        }
        close_map();
    }

    TEST(load_toronto) {
        {
            UNITTEST_TIME_CONSTRAINT(15000);
            CHECK(load_map(map_dir + "toronto_canada.streets.bin"));
        }
        close_map();
    }

    TEST(load_newyork) {
        {
            UNITTEST_TIME_CONSTRAINT(20000);
            CHECK(load_map(map_dir + "new-york_usa.streets.bin"));
        }
        close_map();
    }

    TEST(load_golden_horseshoe) {
        {
            UNITTEST_TIME_CONSTRAINT(20000);
            CHECK(load_map(map_dir + "golden-horseshoe_canada.streets.bin"));
        }
        close_map();
    }

    TEST(load_invalid_map_path) {
        {
            UNITTEST_TIME_CONSTRAINT(3000);
            CHECK(!load_map("/this/path/does/not/exist"));
        }
    }
}
