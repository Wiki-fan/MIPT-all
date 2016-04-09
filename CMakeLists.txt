cmake_minimum_required(VERSION 3.3)
project(didactic_woof)

#list(APPEND CMAKE_CONFIGURATION_TYPES MyDebug)
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "" FORCE)
#set(CMAKE_CONFIGURATION_TYPES "Release;Debug;MyDebug" CACHE STRING "" FORCE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wno-write-strings -Werror")
#SET(CMAKE_CXX_FLAGS_MYDEBUG "-gdwarf-2 -Wall -Wabi -g -p" CACHE STRING
#		"My debug."
#		FORCE)

set(TESTS_SOURCES
		tests/DequeTests.cpp
		tests/AVLTreeTests.h
		tests/CalcTests.cpp
		tests/GlobalFixture.cpp
		tests/ExternalSortTests.cpp
		tests/OpenAddressHashTableTests.cpp
		tests/VectorTests.cpp
		tests/MeldableHeapTests.h
		tests/TestBracketSequence.cpp
		tests/SingleLinkedListTests.cpp
		tests/MinMaxHeapTests.cpp
		tests/SplayTreeTests.h
		tests/SegmentTreeTests.h
		tests/SparseTableTests.h
		tests/CartesianTreeTests.h
		Calc.cpp
		Calc.h
		ChainHashTable.h
		Deque.h
		ExternalSort.cpp
		ExternalSort.h
		File.cpp
		File.h
		HashTable.h
		MinMaxHeap.h
		HeapForMerge.cpp
		HeapForMerge.h
		OpenAddressHashTable.h
		Smth.h
		SortedListFromFile.cpp
		SortedListFromFile.h
		Sorts.h
		Stack.h
		stdafx.cpp
		stdafx.h
		Table.h
		MeldableHeap.h
		BinomialHeap.h
		PtrOwner.h
		SingleLinkedList.h
		LeftistHeap.h
		SkewHeap.h
		XMLParser.h
		BinarySearchTree.h
		CartesianTree.h
		AVLTree.h
		ITree.h
		SplayTree.h
		SegmentTree.h
		SparseTable.h
		ImplicitSplayTree.h tests/ImplicitTreeTests.h ImplicitTree.h Graph.h)
set(TEST test_${PROJECT_NAME})

set(SOURCE_FILES
		Calc.cpp
		Calc.h
		ChainHashTable.h
		Deque.h
		ExternalSort.cpp
		ExternalSort.h
		File.cpp
		File.h
		HashTable.h
		MinMaxHeap.h
		HeapForMerge.cpp
		HeapForMerge.h
		main.cpp
		OpenAddressHashTable.h
		Smth.h
		SortedListFromFile.cpp
		SortedListFromFile.h
		Sorts.h
		Stack.h
		stdafx.cpp
		stdafx.h
		Table.h
		Vector.h
		MeldableHeap.h
		BinomialHeap.h
		PtrOwner.h
		SingleLinkedList.h
		LeftistHeap.h
		SkewHeap.h
		XMLParser.h
		BinarySearchTree.h
		CartesianTree.h
		AVLTree.h
		ITree.h
		SplayTree.h
		SegmentTree.h
		SparseTable.h
		ImplicitSplayTree.h tests/ImplicitTreeTests.h ImplicitTree.h ImplicitSplayTree.cpp ImplicitCartesianTree.cpp ImplicitCartesianTree.h Graph.h)

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
