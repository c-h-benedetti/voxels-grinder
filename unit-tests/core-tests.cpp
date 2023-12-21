#include <iostream>
#include <cmath>
#include <utility>
#include <vector>
#include <chrono>
#include <thread>
#include "gtest/gtest.h"
#include "utils/streams.hpp"

#include "core/Calibration.hpp"
#include "core/Bucket.hpp"
#include "core/MetadataManager.hpp"
#include "core/Task.hpp"
#include "core/ThreadsPool.hpp"
#include "core/Processor.hpp"


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

TEST(Calibration, NoCaps) {
    RecordProperty("description", "Are the input with capital recorded without case?");

    std::vector<std::pair<std::string, std::string>> equivalent = {
        {""  , ""},
        {"a" , "a"},
        {"A" , "a"},
        {"Ab", "ab"},
        {"Ab", "ab"},
        {"_B", "_b"},

    };

    std::map<std::string, Calibration::SizeUnit, case_insensitive_compare> nocase_map;

    for (const std::pair<std::string, std::string>& input : equivalent) {
        nocase_map[input.first] = Calibration::SizeUnit::Micrometers;
        EXPECT_TRUE(nocase_map.find(input.second) != nocase_map.end());
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
    RecordProperty("short", "0 is not a valid dimension for any axis.");
    RecordProperty(
        "description", 
        "A canvas cannot have any null axis, hence, a bucket cannot either.\
         A still canvas (not a timelapse) is a single-frames canvas."
    );

    // Constructor taking dimensions
    EXPECT_ANY_THROW(Bucket(0, 2, 2, 2));
    EXPECT_ANY_THROW(Bucket(2, 0, 2, 2));
    EXPECT_ANY_THROW(Bucket(2, 2, 0, 2));
    EXPECT_ANY_THROW(Bucket(2, 2, 2, 0));

    // Valid constructions
    EXPECT_NO_THROW(Bucket());
    Bucket b1;
    EXPECT_NO_THROW(b1 = Bucket(1, 2, 3, 4));
}


TEST(Bucket, BadRanges) {
    RecordProperty("short", "No range equal or below 0.");
    RecordProperty(
        "description", 
        "A sub-range in a bucket cannot be 0-sized.\
         The range must be included into the main axis."
    );

    constexpr size_t len = 20;
    Bucket b(len, len, len, len);

    // Constructor taking dimensions
    EXPECT_ANY_THROW(b.set_local_columns({0, 0}));
    EXPECT_ANY_THROW(b.set_local_rows({10, 5}));
    EXPECT_ANY_THROW(b.set_local_slices({5, 5}));
    EXPECT_ANY_THROW(b.set_local_frames({0, 21}));
    EXPECT_ANY_THROW(b.set_local_columns({20, 30}));

    EXPECT_NO_THROW(b.set_local_columns({0, 10}));
    EXPECT_NO_THROW(b.set_local_rows({5, 15}));
    EXPECT_NO_THROW(b.set_local_slices({5, 15}));
    EXPECT_NO_THROW(b.set_local_frames({0, 20}));
    EXPECT_NO_THROW(b.set_local_columns({19, 20}));
}


TEST(Bucket, CorrectSize) {
    RecordProperty("short", "Correct calibrated size.");
    RecordProperty(
        "description", 
        "The calibrated dimensions are consistent on each axis and each sub-range."
    );

    glm::vec3 dims(0.05f, 0.05f, 0.2f);
    Calibration c;

    c.set_dimensions(dims);
    c.set_size_unit(Calibration::SizeUnit::Micrometers);

    size_t height=128, width=256, depth=1024, frames=2048;
    Bucket b(width, height, depth, frames);
    b.set_calibration(c);

    EXPECT_EQ(b.get_canvas_columns(), width);
    EXPECT_EQ(b.get_canvas_rows(), height);
    EXPECT_EQ(b.get_canvas_slices(), depth);
    EXPECT_EQ(b.get_canvas_frames(), frames);

    float threshold = 0.0000001f;
    EXPECT_TRUE(std::abs(b.get_canvas_width() - static_cast<float>(width)*c.get_size_x()) < threshold);
    EXPECT_TRUE(std::abs(b.get_canvas_height() - static_cast<float>(height)*c.get_size_y()) < threshold);
    EXPECT_TRUE(std::abs(b.get_canvas_depth() - static_cast<float>(depth)*c.get_size_z()) < threshold);
}


TEST(Bucket, CorrectDataSize) {
    RecordProperty("description", "Correct data size.");

    size_t height=128, width=256, depth=1024, frames=2048;
    std::pair<size_t, size_t> r1={4, 10}, r2={50, 100}, r3={200, 500}, r4={1000, 2000};
    size_t r_w = r1.second - r1.first;
    size_t r_h = r2.second - r2.first;
    size_t r_s = r3.second - r3.first;
    size_t r_f = r4.second - r4.first;
    
    Bucket b1(width, height, depth, frames);
    Bucket b2(b1,
        r1,
        r2,
        r3,
        r4
    );

    EXPECT_EQ(b2.get_canvas_size(), height*width*depth*frames);
    EXPECT_EQ(b2.get_local_size(), r_w*r_h*r_s*r_f);
}


TEST(Bucket, CorrectDataIndex) {
    RecordProperty("description", "Correct index is data segment from Bucket instance.");

    size_t height=128, width=256, depth=33, frames=14;
    std::pair<size_t, size_t> r1={4, 10}, r2={50, 100}, r3={2, 25}, r4={1, 10};
    
    Bucket b1(width, height, depth, frames);
    Bucket b2(b1,
        r1,
        r2,
        r3,
        r4
    );

    EXPECT_EQ(b1.index_of(10, 15, 12, 3), 3641098);
    EXPECT_EQ(b2.index_of(10, 15, 12, 3), 3641098);
}


TEST(Bucket, CorrectIteratorIndex) {
    RecordProperty("description", "Correct index is data segment from Bucket::Iterator instance.");

    size_t width=512, height=512, depth=15, frames=7;    
    Bucket b1(width, height, depth, frames);
    Bucket::Iterator it = b1.get_iterator();

    size_t last = 0, idx = 0;
    bool first = true;

    while (it) {
        idx = it;
        EXPECT_TRUE((idx == last+1) || (first && last == 0));
        last = idx;
        first = false;
        it.next();
    }

    EXPECT_EQ(b1.get_canvas_size()-1, last);
}

// In this test, we want to process individually each slice of each frame.
TEST(Bucket, SplitBucket) {
    RecordProperty("description", "A bucket can be splitted into several ranges.");

    size_t width=512, height=512, depth=10, frames=5;    
    Bucket b1(width, height, depth, frames);
    std::vector<Bucket> buckets;

    for (size_t f = 0 ; f < frames ; f++) {
        for (size_t d = 0 ; d < depth ; d++) {
            Bucket b2(b1, {0, 0}, {0, 0}, {d, d+1}, {f, f+1});
            buckets.push_back(b2);
        }
    }

    EXPECT_EQ(buckets.size(), frames*depth);

    for (const Bucket& b : buckets) {
        EXPECT_EQ(width, b.get_canvas_columns());
        EXPECT_EQ(height, b.get_canvas_rows());
        EXPECT_EQ(depth, b.get_canvas_slices());
        EXPECT_EQ(frames, b.get_canvas_frames());

        EXPECT_EQ(width, b.get_local_columns());
        EXPECT_EQ(height, b.get_local_rows());
        EXPECT_EQ(1, b.get_local_slices());
        EXPECT_EQ(1, b.get_local_frames());
    }
    
    size_t last = 0, idx = 0;
    bool first = true;

    for (const Bucket& b : buckets) {
        Bucket::Iterator it = b.get_iterator();
        while (it) {
            idx = it;
            EXPECT_TRUE((idx == last+1) || (first && last == 0));
            last = idx;
            first = false;
            it.next();
        }
    }

    EXPECT_EQ(b1.get_canvas_size()-1, last);
}


// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
// #    METADATA MANAGER                                                       #
// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #


TEST(MetadataManager, StoreValues) {
    RecordProperty("short", "Store ints, floats and strings.");

    MetadataManager mm;

    mm.set_key<int>("key_int", 42);
    mm.set_key<float>("key_float", 3.1415926f);
    mm.set_key<std::string>("key_string", "some string");

    int key_int;
    float key_float;
    std::string key_string;
    
    EXPECT_TRUE(mm.get_key<int>("key_int", key_int));
    EXPECT_TRUE(mm.get_key<float>("key_float", key_float));
    EXPECT_TRUE(mm.get_key<std::string>("key_string", key_string));

    EXPECT_EQ(key_int, 42);
    EXPECT_EQ(key_float, 3.1415926f);
    EXPECT_EQ(key_string, "some string");

    int dummy = 0;
    EXPECT_FALSE(mm.get_key<int>(key_string, dummy));
}


TEST(MetadataManager, RecordKey) {
    RecordProperty("short", "Determine whether a key exists or not.");

    MetadataManager mm;

    EXPECT_FALSE(mm.has_key("key_int"));

    mm.set_key<int>("key_int", 42);

    EXPECT_TRUE(mm.has_key("key_int"));

}


TEST(MetadataManager, UpdateKey) {
    RecordProperty("short", "Edit the value of an existing key.");

    MetadataManager mm;
    int key_int;

    mm.set_key<int>("key_int", 42);
    mm.get_key<int>("key_int", key_int);
    EXPECT_EQ(key_int, 42);

    mm.set_key<int>("key_int", 0);
    mm.get_key<int>("key_int", key_int);
    EXPECT_EQ(key_int, 0);
}


// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
// #    THREADS POOL                                                           #
// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #


struct DummyLongThreadsPoolTask : public Task {
    
    std::vector<int> times = {1, 5, 2, 2, 5, 1};

    int run(Bucket b) override {
        std::this_thread::sleep_for(std::chrono::seconds(this->times[b.get_slices_range().first]));
        return 0;
    }

    int execute(Bucket b) {
        return this->run(b);
    }
};


struct DummyThreadsPoolTask : public Task {
    int run(Bucket b) override {
        return b.get_slices_range().first % 2 == 0 ? 0 : -1;
    }

    int execute(Bucket b) {
        return this->run(b);
    }
};


struct DummyBrokenTask : public Task {
    int run(Bucket b) override {
        throw "stuff";
        return 0;
    }

    int execute(Bucket b) {
        return this->run(b);
    }
};

// --- 

TEST(ThreadsPool, InstanciatePool) {
    RecordProperty("short", "Create and stop the threads pool.");
    
    ThreadsPool* tp = nullptr;
    EXPECT_ANY_THROW(tp = ThreadsPool::threads_pool(0));

    tp = ThreadsPool::threads_pool();
    EXPECT_TRUE(tp->n_workers() > 0);
}


TEST(ThreadsPool, Terminates) {
    RecordProperty("short", "A task entrusted to the pool must finish.");
    
    ThreadsPool* tp = ThreadsPool::threads_pool();
    Bucket b1(10, 10, 6);
    DummyLongThreadsPoolTask dltpt;

    std::vector<Bucket> bs1 = {
        Bucket(b1, {0, 0}, {0, 0}, {0, 1}, {0, 0}),
        Bucket(b1, {0, 0}, {0, 0}, {1, 2}, {0, 0}),
        Bucket(b1, {0, 0}, {0, 0}, {2, 3}, {0, 0}),
        Bucket(b1, {0, 0}, {0, 0}, {3, 4}, {0, 0}),
        Bucket(b1, {0, 0}, {0, 0}, {4, 5}, {0, 0}),
        Bucket(b1, {0, 0}, {0, 0}, {5, 6}, {0, 0}),
    };

    float total = 0.0f;
    for (int t : dltpt.times) { total += static_cast<float>(t); }

    auto start = std::chrono::high_resolution_clock::now();
    tp->delegate_task(&dltpt, bs1);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    
    EXPECT_TRUE(tp->success());
    EXPECT_TRUE(duration.count() < total);
}


TEST(ThreadsPool, Loop) {
    RecordProperty("short", "If there is more buckets than workers.");
    
    ThreadsPool* tp = ThreadsPool::threads_pool();
    Bucket b1(10, 10, 100);
    DummyThreadsPoolTask dtpt;

    std::vector<Bucket> bs1;
    bs1.reserve(100);
    for (size_t s = 0 ; s < 100 ; s++) {
        bs1.push_back(Bucket(b1, {0, 0}, {0, 0}, {s, s+1}, {0, 0}));
    }

    tp->delegate_task(&dtpt, bs1);
    
    EXPECT_FALSE(tp->success());
    EXPECT_TRUE(tp->completion() > 0.99999f);
}


TEST(ThreadsPool, Empty) {
    RecordProperty("short", "Passing a null task or an empty buckets list is forbidden.");
    
    ThreadsPool* tp = ThreadsPool::threads_pool();
    std::vector<Bucket> bs1;
    std::vector<Bucket> bs2;
    Bucket b1(10, 10, 100);
    DummyThreadsPoolTask dtpt;

    bs1.reserve(100);
    for (size_t s = 0 ; s < 100 ; s++) {
        bs1.push_back(Bucket(b1, {0, 0}, {0, 0}, {s, s+1}, {0, 0}));
    }

    EXPECT_ANY_THROW(tp->delegate_task(&dtpt, bs2));
    EXPECT_ANY_THROW(tp->delegate_task(nullptr, bs1));
}


TEST(ThreadsPool, ExceptionSafe) {
    RecordProperty("short", "The pool is not dead if a worker runs into an exception.");
    
    ThreadsPool* tp = ThreadsPool::threads_pool();
    std::vector<Bucket> bs1;
    Bucket b1(10, 10, 100);
    DummyBrokenTask dtpt;

    bs1.reserve(100);
    for (size_t s = 0 ; s < 100 ; s++) {
        bs1.push_back(Bucket(b1, {0, 0}, {0, 0}, {s, s+1}, {0, 0}));
    }

    EXPECT_NO_THROW(tp->delegate_task(&dtpt, bs1));
}
