cmake_minimum_required(VERSION 3.3)
project(didactic_woof)

#list(APPEND CMAKE_CONFIGURATION_TYPES MyDebug)
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "" FORCE)
#set(CMAKE_CONFIGURATION_TYPES "Release;Debug;MyDebug" CACHE STRING "" FORCE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wno-write-strings -Wall -pthread")
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

set(PARALLEL
		Parallel/tree_mutex.h
		Parallel/tree_mutex.cpp
		)
set(MAXIMAL_FLOW
		Flow/maximal_flow.cpp
		Flow/maximal_flow.h
		Flow/Network.h
		Flow/Dinic.h)
set(BIRTHDAY
		Flow/birthday.cpp
		Flow/birthday.h
		Flow/Network.h
		Flow/Dinic.h)

set(PARQUET
		Flow/BadParquet.cpp
		Flow/BadParquet.h
		Flow/Dinic.h
		Flow/Network.h)

set(CHANGINGFLOW
		Flow/changing_flow.cpp
		Flow/changing_flow.h
		Flow/Dinic.h
		Flow/Network.h Flow/edmonds_karp.h)

set(VERYCHANGINGFLOW
		Flow/very_changing_flow.cpp
		Flow/very_changing_flow.h
		Flow/Dinic.h
		Flow/Network.h Flow/edmonds_karp.h)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.59.0 COMPONENTS unit_test_framework REQUIRED)
if (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})

	add_executable(didactic_woof ${SOURCE_FILES})
	target_link_libraries(didactic_woof ${Boost_LIBRARIES})
	add_executable(parallel ${PARALLEL})
	add_executable(flow ${MAXIMAL_FLOW})
	add_executable(birthday ${BIRTHDAY})
	add_executable(parquet ${PARQUET})
	add_executable(changing_flow ${CHANGINGFLOW})
	add_executable(very_changing_flow ${VERYCHANGINGFLOW})

	add_executable(${TEST} ${TESTS_SOURCES} ${PARALLEL})
	target_link_libraries(${TEST} ${Boost_LIBRARIES})
	enable_testing()
	add_test(${TEST} ${TEST})
endif ()
