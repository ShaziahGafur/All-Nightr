#include <iostream>
#include <string>
#include <unittest++/UnitTest++.h>

extern void set_disable_event_loop (bool new_setting);

int main() {
    //Disable interactive graphics
    set_disable_event_loop(true);

    //Run the unit tests
    int num_failures = UnitTest::RunAllTests();

    return num_failures;
}
