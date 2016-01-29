cmake_minimum_required(VERSION 3.3)
project(didactic_woof)

#list(APPEND CMAKE_CONFIGURATION_TYPES MyDebug)
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "" FORCE)
#set(CMAKE_CONFIGURATION_TYPES "Release;Debug;MyDebug" CACHE STRING "" FORCE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
#SET(CMAKE_CXX_FLAGS_MYDEBUG "-gdwarf-2 -Wall -Wabi -g -p" CACHE STRING
#		"My debug, lol."
#		FORCE)

set(TESTS_SOURCES
		Work/tests/DequeTests.cpp
		Work/tests/AVLTreeTests.h
		Work/tests/CalcTests.cpp
		Work/tests/GlobalFixture.cpp
		Work/tests/ExternalSortTests.cpp
		Work/tests/OpenAddressHashTableTests.cpp
		Work/tests/VectorTests.cpp
		Work/tests/MeldableHeapTests.h
		Work/tests/TestBracketSequence.cpp
		Work/tests/VectorTests.h
		Work/tests/SingleLinkedListTests.cpp
		Work/tests/MinMaxHeapTests.cpp
		Work/tests/SplayTreeTests.h
		Work/tests/SegmentTreeTests.h
		Work/tests/SparseTableTests.h
		Work/tests/CartesianTreeTests.h
		Work/Calc.cpp
		Work/Calc.h
		Work/ChainHashTable.h
		Work/Deque.h
		Work/ExternalSort.cpp
		Work/ExternalSort.h
		Work/File.cpp
		Work/File.h
		Work/HashTable.h
		Work/MinMaxHeap.h
		Work/HeapForMerge.cpp
		Work/HeapForMerge.h
		Work/OpenAddressHashTable.h
		Work/Smth.h
		Work/SortedListFromFile.cpp
		Work/SortedListFromFile.h
		Work/Sorts.h
		Work/Stack.h
		Work/stdafx.cpp
		Work/stdafx.h
		Work/Table.h
		Work/Vector.h
		Work/MeldableHeap.h
		Work/BinomialHeap.h
		Work/PtrOwner.h
		Work/SingleLinkedList.h
		Work/LeftistHeap.h
		Work/SkewHeap.h
		Work/XMLParser.h
		Work/BinarySearchTree.h
		Work/CartesianTree.h
		Work/AVLTree.h
		Work/ITree.h
		Work/SplayTree.h
		Work/SegmentTree.h
		Work/SparseTable.h
		)
set(TEST test_${PROJECT_NAME})

set(SOURCE_FILES
		Work/Calc.cpp
		Work/Calc.h
		Work/ChainHashTable.h
		Work/Deque.h
		Work/ExternalSort.cpp
		Work/ExternalSort.h
		Work/File.cpp
		Work/File.h
		Work/HashTable.h
		Work/MinMaxHeap.h
		Work/HeapForMerge.cpp
		Work/HeapForMerge.h
		Work/main.cpp
		Work/OpenAddressHashTable.h
		Work/Smth.h
		Work/SortedListFromFile.cpp
		Work/SortedListFromFile.h
		Work/Sorts.h
		Work/Stack.h
		Work/stdafx.cpp
		Work/stdafx.h
		Work/Table.h
		Work/Vector.h
		Work/MeldableHeap.h
		Work/BinomialHeap.h
		Work/PtrOwner.h
		Work/SingleLinkedList.h
		Work/LeftistHeap.h
		Work/SkewHeap.h
		Work/XMLParser.h
		Work/BinarySearchTree.h
		Work/CartesianTree.h
		Work/AVLTree.h
		Work/ITree.h
		Work/SplayTree.h
		Work/SegmentTree.h
		Work/SparseTable.h
		)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.59.0 COMPONENTS unit_test_framework REQUIRED)
if (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	add_executable(didactic_woof ${SOURCE_FILES})
	target_link_libraries(didactic_woof ${Boost_LIBRARIES})

	add_executable(${TEST} ${TESTS_SOURCES})
	target_link_libraries(${TEST} ${Boost_LIBRARIES})
	enable_testing()
	add_test(${TEST} ${TEST})
endif ()
