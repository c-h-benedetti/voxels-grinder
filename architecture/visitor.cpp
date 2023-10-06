#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <exception>
#include <ctime>
#include <atomic>
#include <utility>
#include "gtest/gtest.h"

#define N_THREADS 8



class Data {

};

class VoxelsCanvas : public Data {
    float* data = nullptr;
    Bucket global;
    Bucket loaded;
};


