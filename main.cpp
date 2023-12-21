#include <iostream>
#include "gtest/gtest.h"


int main(int argc, char* argv[], char* env[]) {

    testing::GTEST_FLAG(output) = "xml:./behavior-tests.xml";

    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}