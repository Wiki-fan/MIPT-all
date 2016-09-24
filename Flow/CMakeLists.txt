cmake_minimum_required(VERSION 3.3)

set(ALGO
		dinic.h
		Network.h
		edmonds_karp.h)
set(MAXIMAL_FLOW
		maximal_flow.cpp
		maximal_flow.h)
set(BIRTHDAY
		birthday.cpp
		birthday.h)
set(PARQUET
		bad_parquet.cpp
		bad_parquet.h)
set(CHANGINGFLOW
		changing_flow.cpp
		changing_flow.h)
set(VERYCHANGINGFLOW
		very_changing_flow.cpp
		very_changing_flow.h)

set(HAMMINGDISTANCE
		hamming.cpp
		hamming.h)

set(HAMMINGDISTANCETEST
		hamming.h
		hamming_test.cpp)

set(ALLFLOWS
		maximal_flow.cpp
		maximal_flow.h
		birthday.cpp
		birthday.h
		bad_parquet.cpp
		bad_parquet.h
		changing_flow.cpp
		changing_flow.h
		very_changing_flow.cpp
		very_changing_flow.h
		hamming.cpp
		hamming.h
		dinic.h
		Network.h hamming_test.cpp)

#find_package (Boost 1.59.0 COMPONENTS unit_test_framework REQUIRED)
#include_directories(${Boost_INCLUDE_DIRS})

add_executable(flow ${MAXIMAL_FLOW} ${ALGO})
add_executable(birthday ${BIRTHDAY} ${ALGO})
add_executable(parquet ${PARQUET} ${ALGO})
add_executable(changing_flow ${CHANGINGFLOW} ${ALGO})
add_executable(very_changing_flow ${VERYCHANGINGFLOW} ${ALGO})
add_executable(hamming_distance ${HAMMINGDISTANCE} ${ALGO})
add_executable(allflows ${ALLFLOWS})
add_executable(hamming_distance_test ${HAMMINGDISTANCETEST} ${ALGO})
target_link_libraries(hamming_distance_test ${Boost_LIBRARIES})

enable_testing()
#add_test (NAME MyTest COMMAND hamming_distance_test)

