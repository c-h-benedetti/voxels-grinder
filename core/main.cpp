#include <iostream>
#include "gtest/gtest.h"
#include "Bucket.hpp"

int main(int argc, char* argv[], char* env[]) {
    testing::GTEST_FLAG(output) = "xml:./architecture_tests.xml";

    testing::InitGoogleTest();

    if (RUN_ALL_TESTS()){
        std::cerr << "At least a test failed" << std::endl;
    }

	return 0;
}