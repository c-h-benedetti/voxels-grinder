#include <iostream>
#include <cmath>
#include <utility>
#include <vector>
#include "gtest/gtest.h"
#include "core/Calibration.hpp"
#include "core/Bucket.hpp"


// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
// #    CALIBRATION                                                            #
// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #


TEST(Calibration, LexicographicOrder) {
    RecordProperty("description", "Lexicographic sorting is not affected by the case.");

    std::vector<std::pair<std::string, std::string>> valid = {
        {""  , ""},
        {"a" , "a"},
        {"a" , "A"},
        {"ab", "Ab"},
        {"Ab", "aB"},
        {"_b", "_B"},

    };

    std::vector<std::pair<std::string, std::string>> invalid = {
        {"" , "a"},
        {"a", "b"},
        {"-", "_"}
    };

    case_insensitive_compare cic;

    for (const std::pair<std::string, std::string>& input : valid) {
        EXPECT_FALSE(cic(input.first, input.second));
    }

    for (const std::pair<std::string, std::string>& input : invalid) {
        EXPECT_TRUE(cic(input.first, input.second));
    }
}

TEST(Calibration, ReadingCorrect) {
    RecordProperty("description", "All the units should be correctly read and recognized.");
    
    Calibration c;

    // Handle all "normal" pieces of text for size units
    for (const auto& item : txt_to_size_unit) {
        EXPECT_NO_THROW(c.set_size_unit(item.first));
    }

    // Handle text with capital letters for size units
    for (const auto& item : txt_to_size_unit) {
        std::string s = item.first;
        if (s[0] < 97 || s[0] > 122) { continue; }
        s[0] -= 32; // pass to capital letter
        EXPECT_NO_THROW(c.set_size_unit(s));
    }

    // Handle all "normal" pieces of text for time units
    for (const auto& item : txt_to_time_unit) {
        EXPECT_NO_THROW(c.set_time_unit(item.first));
    }

    // Handle text with capital letters for time units
    for (const auto& item : txt_to_time_unit) {
        std::string s = item.first;
        if (s[0] < 97 || s[0] > 122) { continue; }
        s[0] -= 32; // pass to capital letter
        EXPECT_NO_THROW(c.set_time_unit(s));
    }

}

TEST(Calibration, ReadingBad) {
    RecordProperty("description", "If we give to the function a piece of text that doesn't correspond to anything, it should throw an exception.");

    Calibration c;

    EXPECT_ANY_THROW(c.set_size_unit("text"));
    EXPECT_ANY_THROW(c.set_time_unit("text"));

}

TEST(Calibration, ValidValues) {
    RecordProperty("description", "Are the values stored in the object valid even if we use accessord with bad values?");

    Calibration c;

    EXPECT_TRUE(c.get_size_x() >= 0.0f);
    EXPECT_TRUE(c.get_size_y() >= 0.0f);
    EXPECT_TRUE(c.get_size_z() >= 0.0f);
    EXPECT_TRUE(c.get_time_interval() >= 0.0f);

    float values[] = {-1.0f, 0.0f, 10.0f};

    for (int i = 0 ; i < 3 ; i++) {
        c.set_size_x(values[i]);
        c.set_size_y(values[i]);
        c.set_size_z(values[i]);
        c.set_time_interval(values[i]);

        EXPECT_TRUE(c.get_size_x() >= 0.0f);
        EXPECT_TRUE(c.get_size_y() >= 0.0f);
        EXPECT_TRUE(c.get_size_z() >= 0.0f);
        EXPECT_TRUE(c.get_time_interval() >= 0.0f);
    }

}


// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
// #    BUCKET                                                                 #
// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #


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
    EXPECT_NO_THROW(Bucket({0, 1}, {2, 4}, {4, 8}, {8, 16}));
}


TEST(Bucket, CorrectSize) {
    RecordProperty("description", "Does the bucket have the correct size?");

    glm::vec3 dims(0.05f, 0.05f, 0.2f);
    Calibration c;

    c.set_dimensions(dims);
    c.set_size_unit(Calibration::SizeUnit::Micrometers);

    size_t height=1024, width=1024, depth=1024;
    Bucket b(height, width, depth, 1);
    b.set_calibration(c);

    EXPECT_EQ(b.nVoxelsX(), width);
    EXPECT_EQ(b.nVoxelsY(), height);
    EXPECT_EQ(b.nVoxelsZ(), depth);

    float threshold = 0.0000001f;
    EXPECT_TRUE(std::abs(b.width() - static_cast<float>(width)*c.get_size_x()) < threshold);
    EXPECT_TRUE(std::abs(b.height() - static_cast<float>(height)*c.get_size_y()) < threshold);
    EXPECT_TRUE(std::abs(b.depth() - static_cast<float>(depth)*c.get_size_z()) < threshold);
}
