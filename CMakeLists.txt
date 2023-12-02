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
	PUBLIC CORE_LIB
)

include_directories(coreTest, "utils")

# = = = = = = = = = = = = = = = = CONTAINERS = = = = = = = = = = = = = = = =

add_library(CONTAINERS_LIB
	${CONTAINERS_SRC}
)

target_include_directories(CONTAINERS_LIB
	PUBLIC "glm"
	PUBLIC "utils"
	PUBLIC "core"
	PUBLIC "stack-segments"
	PUBLIC "googletest/googletest/include"
	PUBLIC "googletest/googlemock/include"
)

add_executable(containersTest
	"containers/main.cpp"
)

target_link_libraries(containersTest
	PUBLIC STACK_SEGMENT_LIB
	PUBLIC CORE_LIB
	PUBLIC CONTAINERS_LIB
	PRIVATE gtest_main
)

# = = = = = = = = = = = = = = = = I/O = = = = = = = = = = = = = = = =

add_library(IO_LIB
	${IO_SRC}
)

target_include_directories(IO_LIB
	PUBLIC "glm"
	PUBLIC "utils"
	PUBLIC "core"
	PUBLIC "containers"
	PUBLIC "proxies"
	PUBLIC "stack-segments"
	PUBLIC "googletest/googletest/include"
	PUBLIC "googletest/googlemock/include"
)

add_executable(ioTest
	"io/main.cpp"
)

target_link_libraries(ioTest
	PUBLIC STACK_SEGMENT_LIB
	CORE_LIB
	CONTAINERS_LIB
	IO_LIB
	PRIVATE gtest_main
)

# = = = = = = = = = = = = = = = = PROXIES = = = = = = = = = = = = = = = =

add_library(PROXIES_LIB
	${PROXIES_SRC}
)

target_include_directories(PROXIES_LIB
	PUBLIC "glm"
	PUBLIC "utils"
	PUBLIC "core"
	PUBLIC "containers"
	PUBLIC "stack-segments"
	PUBLIC "proxies"
	PUBLIC "io"
	PUBLIC "googletest/googletest/include"
	PUBLIC "googletest/googlemock/include"
)

add_executable(proxiesTest
	"proxies/main.cpp"
)

target_link_libraries(proxiesTest
	PUBLIC STACK_SEGMENT_LIB
	PUBLIC CORE_LIB
	PUBLIC CONTAINERS_LIB
	PUBLIC IO_LIB
	PUBLIC PROXIES_LIB
	PRIVATE gtest_main
)

# = = = = = = = = = = = = = = = = PROCESSORS = = = = = = = = = = = = = = = =

add_library(PROCESSORS_LIB
	${PROCESSORS_SRC}
)

target_include_directories(PROCESSORS_LIB
	PUBLIC "glm"
	PUBLIC "utils"
	PUBLIC "core"
	PUBLIC "containers"
	PUBLIC "stack-segments"
	PUBLIC "proxies"
	PUBLIC "io"
	PUBLIC "tasks"
	PUBLIC "googletest/googletest/include"
	PUBLIC "googletest/googlemock/include"
)

add_executable(tasksTest
	"processors/main.cpp"
)

target_link_libraries(tasksTest
	PUBLIC STACK_SEGMENT_LIB
	PUBLIC CORE_LIB
	PUBLIC CONTAINERS_LIB
	PUBLIC IO_LIB
	PUBLIC PROXIES_LIB
	PUBLIC PROCESSORS_LIB
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
	CORE_LIB
	CONTAINERS_LIB
	IO_LIB
	PROXIES_LIB
	PROCESSORS_LIB
	gtest_main
)