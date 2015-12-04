cmake_minimum_required(VERSION 3.3)
project(didactic_woof)

#list(APPEND CMAKE_CONFIGURATION_TYPES MyDebug)
#set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "" FORCE)
set(CMAKE_CONFIGURATION_TYPES "Release;Debug;MyDebug" CACHE STRING "" FORCE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
SET(CMAKE_CXX_FLAGS_MYDEBUG "-Wall -Wabi -g -p" CACHE STRING
		"My debug, lol."
		FORCE)

set(SOURCE_FILES
		Work/Calc.cpp
		Work/Calc.h
		Work/CalcTests.cpp
		Work/ChainHashTable.h
		Work/Deque.h
		Work/DequeTests.cpp
		Work/ExternalSort.cpp
		Work/ExternalSort.h
		Work/ExternalSortTests.cpp
		Work/File.cpp
		Work/File.h
		Work/HashTable.h
		Work/MinMaxHeap.h
		Work/HeapForMerge.cpp
		Work/HeapForMerge.h
		Work/main.cpp
		Work/OpenAddressHashTable.h
		Work/OpenAddressHashTableTests.cpp
		Work/Smth.h
		Work/SortedListFromFile.cpp
		Work/SortedListFromFile.h
		Work/Sorts.h
		Work/Stack.h
		Work/stdafx.cpp
		Work/stdafx.h
		Work/Table.h
		Work/Vector.h
		Work/VectorTests.cpp
		Work/MeldableHeap.h
		Work/MeldableHeapTests.h
		Work/TestBracketSequence.cpp
		Work/TestBracketSequence.h
		Work/BinomialHeap.h
		Work/SingleLinkedList.cpp
		Work/CalcTests.h
		Work/PtrOwner.h
		Work/VectorTests.h
		Work/SingleLinkedList.h
		Work/SingleLinkedListTests.h
		Work/SingleLinkedListTests.cpp
		Work/MinMaxHeapTests.cpp
		Work/MinMaxHeapTests.h
		Work/LeftistHeap.h
		Work/SkewHeap.h Work/SingleLinkedListTests.cpp)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.59.0 COMPONENTS)
if (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	add_executable(didactic_woof ${SOURCE_FILES})
	target_link_libraries(didactic_woof ${Boost_LIBRARIES})
endif ()