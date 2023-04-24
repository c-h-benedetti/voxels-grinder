cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(VoxelsGrinder LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory("googletest")

set(STACK_SEGMENT_SRC
	"stack-segments/ID.hpp"
	"stack-segments/ID.cpp"
	"stack-segments/LUT_IDs.h"
	"stack-segments/StackSegment.hpp"
	"stack-segments/StackSegment.cpp"
	"stack-segments/StackSegment.tpp"
)

set(ACTIONS_SRC
	"actions-manager/Action.hpp"
	"actions-manager/Action.cpp"
	"actions-manager/action_key.hpp"
	"actions-manager/action_key.cpp"
	"actions-manager/ActionPathAutomata.hpp"
	"actions-manager/ActionPathAutomata.cpp"
	"actions-manager/ActionPathCategory.hpp"
	"actions-manager/ActionsManager.hpp"
	"actions-manager/ActionsManager.cpp"
	"actions-manager/BaseArgument.hpp"
	"actions-manager/classifier.hpp"
	"actions-manager/Receiver.hpp"
	"actions-manager/Receiver.cpp"
)

set(MEDIAS_SRC
	"medias/bit_depth.hpp"
	"medias/Dimensions.hpp"
	"medias/Dimensions.cpp"
	"medias/MediaData.hpp"
	"medias/MediaData.cpp"
	"medias/Media.hpp"
	"medias/Media.cpp"
	"medias/MediaManager.hpp"
	"medias/MediaManager.cpp"
	"medias/DataUtils.hpp"
	"medias/DataUtils.cpp"
)

set(UTILS_SRC
	"utils/Color.hpp"
	"utils/Color.cpp"
)

# = = = = = = = = = = = = = = = = STACK SEGMENTS = = = = = = = = = = = = = = = =

add_library(STACK_SEGMENT_LIB
	${STACK_SEGMENT_SRC}
)

target_include_directories(STACK_SEGMENT_LIB
	PUBLIC "stack-segments"
	PUBLIC "googletest/googletest/include"
	PUBLIC "googletest/googlemock/include"
)

add_executable(stackSegmentTest
	stack-segments/main.cpp
)

target_link_libraries(stackSegmentTest
	PUBLIC STACK_SEGMENT_LIB
	PRIVATE gtest_main
)

# = = = = = = = = = = = = = = = = UTILS = = = = = = = = = = = = = = = =

add_library(UTILS_LIB
	${UTILS_SRC}
)

target_include_directories(UTILS_LIB
	PUBLIC "stack-segments"
	PUBLIC "utils"
	PUBLIC "googletest/googletest/include"
	PUBLIC "googletest/googlemock/include"
)

add_executable(utilsTest
	utils/main.cpp
)

target_link_libraries(utilsTest
	PUBLIC UTILS_LIB
	PRIVATE gtest_main
)

# = = = = = = = = = = = = = = = = MEDIAS = = = = = = = = = = = = = = = =

add_library(MEDIAS_LIB
	${MEDIAS_SRC}
)

target_include_directories(MEDIAS_LIB
	PUBLIC "stack-segments"
	PUBLIC "medias"
	PUBLIC "utils"
	PUBLIC "googletest/googletest/include"
	PUBLIC "googletest/googlemock/include"
)

add_executable(mediasTest
	medias/main.cpp
)

target_link_libraries(mediasTest
	MEDIAS_LIB
	STACK_SEGMENT_LIB
	UTILS_LIB
	gtest_main
)

# = = = = = = = = = = = = = = = = ACTIONS MANAGER = = = = = = = = = = = = = = = =

add_library(ACTIONS_LIB
	${ACTIONS_SRC}
)

target_include_directories(ACTIONS_LIB
	PUBLIC "stack-segments"
	PUBLIC "medias"
	PUBLIC "actions-manager"
	PUBLIC "googletest/googletest/include"
	PUBLIC "googletest/googlemock/include"
)

add_executable(actionsManagerTest
	actions-manager/main.cpp
)

target_link_libraries(actionsManagerTest
	"-Wl,--whole-archive"
	ACTIONS_LIB
	"-Wl,--no-whole-archive"
	STACK_SEGMENT_LIB
	MEDIAS_LIB
	gtest_main
)

# = = = = = = = = = = = = = = = = | GLOBAL PROJECT | = = = = = = = = = = = = = = = =

add_executable(${PROJECT_NAME}
	main.cpp
)

target_link_libraries(${PROJECT_NAME}
	"-Wl,--whole-archive"
	ACTIONS_LIB
	"-Wl,--no-whole-archive"
	STACK_SEGMENT_LIB
)