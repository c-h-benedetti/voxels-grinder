cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(VoxelsGrinder LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory("googletest")

option(DEBUG_MODE "Build in debug mode" ON)

if(DEBUG_MODE)
    add_definitions(-DDEBUG_MODE)
endif()

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
	"core/Bucket.cpp"
	"core/Data.hpp"
	"core/Data.cpp"
	"core/MetadataManager.hpp"
	"core/MetadataManager.cpp"
	"core/Task.hpp"
	"core/Task.cpp"
	"core/Processor.hpp"
	"core/Processor.cpp"
	"core/ThreadsPool.hpp"
	"core/ThreadsPool.cpp"
	"core/DataProxy.hpp"
	"core/DataProxy.cpp"
	"core/Calibration.hpp"
	"core/Calibration.cpp"
)

set(CONTAINERS_SRC
	"containers/GeometryCanvas.hpp"
	"containers/GeometryCanvas.cpp"
	"containers/VoxelsCanvas.hpp"
	"containers/VoxelsCanvas.cpp"
)

set(IO_SRC
	"io/IOManager.hpp"
	"io/IOManager.cpp"
)

set(PROXIES_SRC
	"proxies/CziProxy.hpp"
	"proxies/CziProxy.cpp"
	"proxies/ObjProxy.hpp"
	"proxies/ObjProxy.cpp"
	"proxies/PgmProxy.hpp"
	"proxies/PgmProxy.cpp"
	"proxies/TiffProxy.hpp"
	"proxies/TiffProxy.cpp"
	"proxies/XmlProxy.hpp"
	"proxies/XmlProxy.cpp"
)

set(UTILS_SRC
	"utils/Color.hpp"
	"utils/Color.cpp"
	"utils/general.hpp"
	"utils/general.cpp"
	"utils/Logger.hpp"
	"utils/Logger.cpp"
)

set(PROCESSORS_SRC
	"processors/binary/ThresholdProcessor.hpp"
	"processors/binary/ThresholdProcessor.cpp"
)

# = = = = = = = = = = = = = = = = STACK SEGMENTS = = = = = = = = = = = = = = = =

add_library(STACK_SEGMENT_LIB
	${STACK_SEGMENT_SRC}
)

target_include_directories(STACK_SEGMENT_LIB
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# = = = = = = = = = = = = = = = = UTILS = = = = = = = = = = = = = = = =

add_library(UTILS_LIB
	${UTILS_SRC}
)

target_include_directories(UTILS_LIB
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# = = = = = = = = = = = = = = = = CORE = = = = = = = = = = = = = = = =

add_library(CORE_LIB
	${CORE_SRC}
)

target_include_directories(CORE_LIB
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

include_directories(coreTest, "utils")

# = = = = = = = = = = = = = = = = CONTAINERS = = = = = = = = = = = = = = = =

add_library(CONTAINERS_LIB
	${CONTAINERS_SRC}
)

target_include_directories(CONTAINERS_LIB
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# = = = = = = = = = = = = = = = = I/O = = = = = = = = = = = = = = = =

add_library(IO_LIB
	${IO_SRC}
)

target_include_directories(IO_LIB
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# = = = = = = = = = = = = = = = = PROXIES = = = = = = = = = = = = = = = =

add_library(PROXIES_LIB
	${PROXIES_SRC}
)

target_include_directories(PROXIES_LIB
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# = = = = = = = = = = = = = = = = PROCESSORS = = = = = = = = = = = = = = = =

add_library(PROCESSORS_LIB
	${PROCESSORS_SRC}
)

target_include_directories(PROCESSORS_LIB
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# = = = = = = = = = = = = = = = = ACTIONS MANAGER = = = = = = = = = = = = = = = =

add_library(ACTIONS_LIB
	${ACTIONS_SRC}
)

target_include_directories(ACTIONS_LIB
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# = = = = = = = = = = = = = = = = |   UNIT TESTS   | = = = = = = = = = = = = = = = =

add_executable("unitTests"
	"unit-tests/actions-manager-tests.cpp"
	"unit-tests/containers-tests.cpp"
	"unit-tests/core-tests.cpp"
	"unit-tests/io-tests.cpp"
	"unit-tests/processors-tests.cpp"
	"unit-tests/proxies-tests.cpp"
	"unit-tests/stack-segment-tests.cpp"
	"unit-tests/utils-tests.cpp"
	"unit-tests/main.cpp"
)

target_link_libraries("unitTests"
	"-Wl,--whole-archive"
	ACTIONS_LIB
	"-Wl,--no-whole-archive"
	STACK_SEGMENT_LIB
	CORE_LIB
	CONTAINERS_LIB
	IO_LIB
	PROXIES_LIB
	PROCESSORS_LIB
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
	CORE_LIB
	CONTAINERS_LIB
	IO_LIB
	PROXIES_LIB
	PROCESSORS_LIB
	gtest_main
)