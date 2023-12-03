#include "actions-manager/ActionPathCategory.hpp"
#include "actions-manager/ActionsManager.hpp"
#include "actions-manager/BaseArgument.hpp"
#include "actions-manager/ActionPathAutomata.hpp"
#include "actions-manager/Receiver.hpp"
#include <iostream>
#include <functional>
#include <fstream>
#include <utility>
#include <chrono>
#include "gtest/gtest.h"


TEST(Receiver, WriteAndRead) {
    RecordProperty("description", "Is an instance of receivedMap able to hold objects of all types it's supposed to?");
    
    size_t nIDs = 8;
    ID ids[nIDs];

    for (size_t i = 0 ; i < nIDs ; i++) {
        ids[i] = ID::makeID();
    }

	rcv::receivedMap container;

    rcv::Float  a = 3.1415926;
    rcv::Int    b = -42;
    rcv::Uint   c = 42;
    rcv::String d = "Some string. éçµ 🌐.";
    rcv::Char   e = 'q';
    rcv::Bool   f = true;

    rcv::Vector<rcv::Float>  g = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
    rcv::Vector<rcv::String> h = {"one", "two", "three", "four", "five", "six"};

    container[ids[0]] = a;
    container[ids[1]] = b;
    container[ids[2]] = c;
    container[ids[3]] = d;
    container[ids[4]] = e;
    container[ids[5]] = f;

    container[ids[6]] = g;
    container[ids[7]] = h;
    
    EXPECT_EQ(std::get<rcv::Float>(container[ids[0]]), a);
    EXPECT_EQ(std::get<rcv::Int>(container[ids[1]]), b);
    EXPECT_EQ(std::get<rcv::Uint>(container[ids[2]]), c);
    EXPECT_EQ(std::get<rcv::String>(container[ids[3]]), d);
    EXPECT_EQ(std::get<rcv::Char>(container[ids[4]]), e);
    EXPECT_EQ(std::get<rcv::Bool>(container[ids[5]]), f);

    const rcv::Vector<rcv::Float>&  vg = std::get<rcv::Vector<rcv::Float>>(container[ids[6]]);
    const rcv::Vector<rcv::String>& vh = std::get<rcv::Vector<rcv::String>>(container[ids[7]]);

    for (size_t i = 0 ; i < vg.size() ; i++) {
        EXPECT_EQ(vg[i], g[i]);
        EXPECT_EQ(vh[i], h[i]);
    }
}


// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #


TEST(Hint, NumberOfBytes) {
    RecordProperty("description", "Is an instance of receivedMap able to hold objects of all types it's supposed to?");

    Hint hint(
        "SomeHint", 
        ID::makeID(), 
        "IMAGE", 
        true, 
        "🎯 C'est un hint pour un paramètre. À voir où il est utilisé. Pas de taille limite ici.", 
        ""
    );
}

// class PathAutomataTest : public ::testing::Test {
// protected:

//     //void SetUp() override {}

//     // void TearDown() override {}

//     std::vector<testing_arrays> testing_set_arrays = {
//         testing_arrays{{}, "action", true, true},
//         testing_arrays{{"aze"}, "action", true, true},
//         testing_arrays{{"aze", "aze"}, "action", true, true},

//         testing_arrays{{}, "", true, false},
//         testing_arrays{{"aze"}, "", true, false},
//         testing_arrays{{"aze", "aze"}, "", true, false},

//         testing_arrays{{"azertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbn"}, "", false, false},
//         testing_arrays{{"azertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbn"}, "action", false, false},
//         testing_arrays{{"#ert@", "&jhg="}, "action", false, false},
//         testing_arrays{{"", ""}, "action", false, false},
//     };

//     // This testing set has for purpose to be used with functions starting from a string to parse.
//     std::vector<testing_data> testing_set = {
//         testing_data{State::NAMED, ":action", {}, "action", true},
//         testing_data{State::NAMED, "first:action", {"first"}, "action", true},
//         testing_data{State::NAMED, "first.second:action", {"first", "second"}, "action", true},

//         testing_data{State::ANONYMOUS, "", {}, "", false},
//         testing_data{State::ANONYMOUS, "first", {"first"}, "", false},
//         testing_data{State::ANONYMOUS, "first.second", {"first", "second"}, "", false},

//         testing_data{State::INVALID, ".first", {}, "", false},
//         testing_data{State::INVALID, "first.second.:action", {}, "", false},
//         testing_data{State::INVALID, "first.second:.action", {}, "", false},
//         testing_data{State::INVALID, "first:second:third", {}, "", false},
//         testing_data{State::INVALID, "first:second.third", {}, "", false},
//         testing_data{State::INVALID, ":first.second", {}, "", false},
//         testing_data{State::INVALID, nullptr, {}, "", false},
//         testing_data{State::INVALID, "azertyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbn", {}, "", false},
//         testing_data{State::INVALID, "azer:azertyuiopqsdfghjklmtyuiopqsdfghjklmwxcvbnazertyuiopqsdfghjklmwxcvbn", {}, "", false},
//         testing_data{State::INVALID, "azer.#df@():&123", {}, "", false},
//         testing_data{State::INVALID, "azer.sec:&123", {}, "", false},

//         testing_data{State::CAPED, ":", {}, "", false},
//         testing_data{State::CAPED, "first:", {"first"}, "", false},
//         testing_data{State::CAPED, "first.second:", {"first", "second"}, "", false}
//     };
// };
