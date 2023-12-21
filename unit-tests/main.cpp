#include "gtest/gtest.h"

int main(int argc, char* argv[], char* env[]){

	testing::GTEST_FLAG(output) = "xml:./unit-tests.xml";
    testing::InitGoogleTest();

    if (RUN_ALL_TESTS()){
        std::cerr << "At least a test failed" << std::endl;
    }

	return 0;
}