#include <iostream>
#include "gtest/gtest.h"
#include "Bucket.hpp"


TEST(Calibration, Con) {
    RecordProperty("description", "If a dimension is zero, we should get an exception.");
}


TEST(Bucket, DimToZero) {
    RecordProperty("description", "If a dimension is zero, we should get an exception.");

    // Constructor taking dimensions
    EXPECT_ANY_THROW(Bucket(0, 2, 2, 2));
    EXPECT_ANY_THROW(Bucket(2, 0, 2, 2));
    EXPECT_ANY_THROW(Bucket(2, 2, 0, 2));
    EXPECT_ANY_THROW(Bucket(2, 2, 2, 0));

    // Constructor taking ranges
    EXPECT_ANY_THROW(Bucket({0, 0}, {0, 1}, {0, 1}, {0, 1}));
    EXPECT_ANY_THROW(Bucket({0, 1}, {3, 2}, {0, 1}, {0, 1}));

    // Valid constructions
    EXPECT_NO_THROW(Bucket());
    EXPECT_NO_THROW(Bucket(1, 2, 3, 4));
    EXPECT_ANY_THROW(Bucket({0, 1}, {2, 4}, {4, 8}, {8, 16}));
}

TEST(Bucket, BoundingBox) {
    RecordProperty("description", "Does the bucket behave like a bounding-box?");
}


int main(int argc, char* argv[], char* env[]) {
    ::testing::GTEST_FLAG(output) = "xml:./core_tests.xml";
    ::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
