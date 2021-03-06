cmake_minimum_required(VERSION 3.3)
project(didactic_woof)

set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "" FORCE)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fsanitize=address -O0 -g -Wall -latomic -pthread -Wextra")

set(TESTS_SOURCES
		main.cpp
		)

set(TEST test_${PROJECT_NAME})

set(SOURCE_FILES
		main.cpp
		)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.59.0 COMPONENTS unit_test_framework REQUIRED)
if (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})

	#add_subdirectory(Flow)
	#add_subdirectory(Parallel)
	#add_subdirectory(MoreFlows)
	#add_subdirectory(ParallelWorkers)
	#add_subdirectory(Strings)
	#add_subdirectory(BinarySearchTrees)
	#add_subdirectory(LockFree)
	add_subdirectory(MainParallels)

	add_executable(didactic_woof ${SOURCE_FILES})
	target_link_libraries(didactic_woof ${Boost_LIBRARIES})

	#enable_testing()
	#add_test(${TEST} ${TEST})
	#add_test (NAME MyTest COMMAND hamming_distance_test)
endif ()
