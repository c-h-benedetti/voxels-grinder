cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(VoxelsGrinder LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory("googletest")

add_definitions(-DUTEST_DATA="../utests_data/")

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

set(CORE_SRC
	"core/Bucket.hpp"
	"core/Data.hpp"
	"core/Data.cpp"
	"core/ThreadsPool.hpp"
	"core/ThreadsPool.cpp"
	"core/VoxelsCanvas.hpp"
	"core/VoxelsCanvas.cpp"	
	"core/Vertices.hpp"
	"core/Vertices.cpp"	
	"core/PolyLine.hpp"
	"core/PolyLine.cpp"	
	"core/Mesh.hpp"
	"core/Mesh.cpp"	
)

set(UTILS_SRC
	"utils/Color.hpp"
	"utils/Color.cpp"
	"utils/general.hpp"
)

# = = = = = = = = = = = = = = = = STACK SEGMENTS = = = = = = = = = = = = = = = =

add_library(STACK_SEGMENT_LIB
	${STACK_SEGMENT_SRC}
)

target_include_directories(STACK_SEGMENT_LIB
	PUBLIC "glm"
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
	PUBLIC "glm"
	PUBLIC "stack-segments"
	PUBLIC "utils"
	PUBLIC "googletest/googletest/include"
	PUBLIC "googletest/googlemock/include"
)

add_executable(utilsTest
	"utils/main.cpp"
)

target_link_libraries(utilsTest
	PUBLIC UTILS_LIB
	PRIVATE gtest_main
)

# = = = = = = = = = = = = = = = = CORE = = = = = = = = = = = = = = = =

add_library(CORE_LIB
	${CORE_SRC}
)

target_include_directories(CORE_LIB
	PUBLIC "glm"
	PUBLIC "utils"
	PUBLIC "stack-segments"
	PUBLIC "googletest/googletest/include"
	PUBLIC "googletest/googlemock/include"
)

add_executable(coreTest
	"core/main.cpp"
)

target_link_libraries(coreTest
	PUBLIC STACK_SEGMENT_LIB
	PRIVATE gtest_main
)

# = = = = = = = = = = = = = = = = ACTIONS MANAGER = = = = = = = = = = = = = = = =

add_library(ACTIONS_LIB
	${ACTIONS_SRC}
)

target_include_directories(ACTIONS_LIB
	PUBLIC "glm"
	PUBLIC "stack-segments"
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