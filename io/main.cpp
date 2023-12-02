#include <exception>
#include <iostream>
#include "gtest/gtest.h"
#include "Data.hpp"
#include "IOManager.hpp"


TEST(BehaviorTest, TaskUsage) {
    /*
    // This image is not composed of labels.
    Data* im1 = IOManager::iomanager().open_from_disk("some/path/to/image.tif");

    // This image is composed of labels.
    Data* im2 = IOManager::iomanager().open_from_disk("some/path/to/labels.tif");

    // This image is composed of labels.
    LabeledCanvas* im3 = new LabeledCanvas(1024, 1024);

    int r1=-1, r2=-1, r3=-1;

    try {
        UniqueLabelsTask ul1(im1);
        ul1.make_labels_list(true);
        ul1.execute();
    }
    catch (std::exception& e) {}

    try {
        UniqueLabelsTask ul2(im2);
        ul2.make_labels_list(true);
        ul2.execute();
    }
    catch (std::exception& e) {}

    try {
        UniqueLabelsTask ul3(im3);
        ul3.make_labels_list(true);
        ul3.execute();
    }
    catch (std::exception& e) {}

    delete im3;
    */
}

int main(int argc, char* argv[], char* env[]) {
    ::testing::GTEST_FLAG(output) = "xml:./io_tests.xml";
    ::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
