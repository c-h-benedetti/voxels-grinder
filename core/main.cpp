#include <iostream>
#include <string>
#include <variant>
#include "gtest/gtest.h"
#include "Bucket.hpp"
#include "general.hpp"


TEST(TaskUsage, FromCore) {
    RecordProperty("description", "Verifies that the architecture of a task allows to use it conveniently from C++.");

    VoxelsCanvas* vc1 = new VoxelsCanvas(128, 128, 16, 2, 4);
    VoxelsCanvas* vc2(vc1);

    /** - 1 - 
     * We want to automatically threshold the first image with Otsu method.
     * We want to access the thresholding value after we applied them.
     */
    ThresholdTask t1{vc1, ThresholdTask::method::OTSU};
    t1.process_bounds();
    t1.apply_threshold();

    float f1 = t1.get_lower_bound();
    float f2 = t1.get_upper_bound();

    std::unique_ptr<VoxelsCanvas> vc3 = t1.get_mask();

    /** - 2 - 
     * We want to manually threshold the second image, by giving the upper and lower values manually.
     */
    ThresholdTask t2{vc2};
    t1.set_threshold_values(f1, f2);
    t1.apply_threshold();

    std::unique_ptr<VoxelsCanvas> vc4 = t1.get_mask();

    /** - 3 - 
     * Make sure that both output canvas still exist and were not overwritten along the process.
     */
    float d = vc3->distance_from(vc4);
    EXPECT_TRUE(d < 0.001f);

    delete vc1;
    delete vc2;
}


TEST(TaskUsage, FromAction) {
    RecordProperty("description", "Verifies that the architecture of a task allows to use it conveniently from a generic Action.");

    /** - 1 -
     * The Action will receive its settings through a string that is going to be parsed.
     * Parsing is implemented at the Action level, specialisations are not responsible for this feature.
     * Arguments are available in a map.
     * Some arguments (such as images) are passed via a representation (an ID for example).
     * In this function, we imagine that we are in the body of the function that received the arguments and has to apply the action.
     */

     std::map<std::string, std::variant<float, int, std::string>> arguments = {
        {"target_img", 14},
        {"method", 1},
        {"v1", 0.0f},
        {"v2", 1.0f}
     };

     ThresholdTask t1{};

     // Here, we should have something like a get_image_by_id(arguments["target_img"])
     VoxelsCanvas* vc1 = new VoxelsCanvas(128, 128, 16, 2, 4);
     t1.set_target(vc1);

     t1.use_auto_threshold(static_cast<ThresholdTask::method>(arguments["method"]));

     if (arguments["method"] > 0) {
        t1.set_threshold_values(arguments["v1"], arguments["v2"]);
     }

     t1.apply_threshold();

     std::unique_ptr<VoxelsCanvas> vc2 = t1.get_mask();
}


TEST(TaskUsage, OnGeneric) {
    RecordProperty("description", "Can we call an action on the generic type 'Data' and expect the correct overload to be used?");

}


TEST(TaskUsage, UsesBuckets) {
    RecordProperty("description", "Does the summoner task can access and use the ThreadsPool?");

}


TEST(IOBehavior, FromDisk) {
    RecordProperty("description", "Can we open every data types with a generic function?");

    Data* d1 = IOManager::open_from_disk(""); // invalid path

    Data* d2 = IOManager::open_from_disk(GET_UTEST("invalid_img.tif"));
    Data* d3 = IOManager::open_from_disk(GET_UTEST("valid_img.tif"));

    Data* d4 = IOManager::open_from_disk(GET_UTEST("invalid_mesh.obj"));
    Data* d5 = IOManager::open_from_disk(GET_UTEST("valid_mesh.obj"));

    EXPECT_EQ(d1, nullptr);
    EXPECT_EQ(d2, nullptr);
    EXPECT_NE(d3, nullptr);
    EXPECT_EQ(d4, nullptr);
    EXPECT_NE(d5, nullptr);

    Data* data[] = {d1, d2, d3, d4, d5};

    ThresholdTask t1;

    for (int i = 0 ; i < 5 ; i++) {
        t1.set_target(data[i]);
        t1.process_threshold();
        t1.apply_threshold();
    }
}


int main(int argc, char* argv[], char* env[]) {
    testing::GTEST_FLAG(output) = "xml:./architecture_tests.xml";

    testing::InitGoogleTest();

    if (RUN_ALL_TESTS()){
        std::cerr << "At least a test failed" << std::endl;
    }

	return 0;
}