cmake_minimum_required(VERSION 3.3)
project(didactic_woof)

#list(APPEND CMAKE_CONFIGURATION_TYPES MyDebug)
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "" FORCE)
#set(CMAKE_CONFIGURATION_TYPES "Release;Debug;MyDebug" CACHE STRING "" FORCE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wno-write-strings -Wall -pthread -D_GLIBCXX_USE_CXX11_ABI=0")
#SET(CMAKE_CXX_FLAGS_MYDEBUG "-gdwarf-2 -Wall -Wabi -g -p" CACHE STRING
#		"My debug."
#		FORCE)

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

	add_subdirectory(Flow)
	add_subdirectory(Parallel)

	add_executable(didactic_woof ${SOURCE_FILES})
	target_link_libraries(didactic_woof ${Boost_LIBRARIES})

	#enable_testing()
	#add_test(${TEST} ${TEST})
	#add_test (NAME MyTest COMMAND hamming_distance_test)
endif ()
