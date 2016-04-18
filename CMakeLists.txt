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
		StandardDataStructures/DequeTests.cpp
		BinarySearchTrees/AVLTreeTests.h
		Calc/CalcTests.cpp
		tests/GlobalFixture.cpp
		ExternalSort/ExternalSortTests.cpp
		HashTables/OpenAddressHashTableTests.cpp
		StandardDataStructures/VectorTests.cpp
		MeldableHeaps/MeldableHeapTests.h
		Calc/TestBracketSequence.cpp
		StandardDataStructures/SingleLinkedListTests.cpp
		StandardDataStructures/MinMaxHeapTests.cpp
		BinarySearchTrees/SplayTreeTests.h
		StructuresForMassModification/SegmentTreeTests.h
		StructuresForMassModification/SparseTableTests.h
		BinarySearchTrees/CartesianTreeTests.h
		Calc/Calc.cpp
		Calc/Calc.h
		HashTables/ChainHashTable.h
		StandardDataStructures/Deque.h
		ExternalSort/ExternalSort.cpp
		ExternalSort/ExternalSort.h
		ExternalSort/File.cpp
		ExternalSort/File.h
		HashTables/HashTable.h
		StandardDataStructures/MinMaxHeap.h
		ExternalSort/HeapForMerge.cpp
		ExternalSort/HeapForMerge.h
		HashTables/OpenAddressHashTable.h
		Other/Smth.h
		ExternalSort/SortedListFromFile.cpp
		ExternalSort/SortedListFromFile.h
		Other/Sorts.h
		StandardDataStructures/Stack.h
		stdafx.cpp
		stdafx.h
		StandardDataStructures/Table.h
		MeldableHeaps/MeldableHeap.h
		MeldableHeaps/BinomialHeap.h
		Other/PtrOwner.h
		StandardDataStructures/SingleLinkedList.h
		MeldableHeaps/LeftistHeap.h
		MeldableHeaps/SkewHeap.h
		Other/XMLParser.h
		BinarySearchTrees/BinarySearchTree.h
		BinarySearchTrees/CartesianTree.h
		BinarySearchTrees/AVLTree.h
		BinarySearchTrees/ITree.h
		BinarySearchTrees/SplayTree.h
		StructuresForMassModification/SegmentTree.h
		StructuresForMassModification/SparseTable.h
		ImplicitTrees/ImplicitSplayTree.h ImplicitTrees/ImplicitTreeTests.h ImplicitTrees/ImplicitTree.h Graph/Graph.h Graph/GraphTraversal.h Graph/GraphTests.h)
set(TEST test_${PROJECT_NAME})

set(SOURCE_FILES
		Calc/Calc.cpp
		Calc/Calc.h
		HashTables/ChainHashTable.h
		StandardDataStructures/Deque.h
		ExternalSort/ExternalSort.cpp
		ExternalSort/ExternalSort.h
		ExternalSort/File.cpp
		ExternalSort/File.h
		HashTables/HashTable.h
		StandardDataStructures/MinMaxHeap.h
		ExternalSort/HeapForMerge.cpp
		ExternalSort/HeapForMerge.h
		main.cpp
		HashTables/OpenAddressHashTable.h
		Other/Smth.h
		ExternalSort/SortedListFromFile.cpp
		ExternalSort/SortedListFromFile.h
		Other/Sorts.h
		StandardDataStructures/Stack.h
		stdafx.cpp
		stdafx.h
		StandardDataStructures/Table.h
		StandardDataStructures/Vector.h
		MeldableHeaps/MeldableHeap.h
		MeldableHeaps/BinomialHeap.h
		Other/PtrOwner.h
		StandardDataStructures/SingleLinkedList.h
		MeldableHeaps/LeftistHeap.h
		MeldableHeaps/SkewHeap.h
		Other/XMLParser.h
		BinarySearchTrees/BinarySearchTree.h
		BinarySearchTrees/CartesianTree.h
		BinarySearchTrees/AVLTree.h
		BinarySearchTrees/ITree.h
		BinarySearchTrees/SplayTree.h
		StructuresForMassModification/SegmentTree.h
		StructuresForMassModification/SparseTable.h
		ImplicitTrees/ImplicitSplayTree.h
		ImplicitTrees/ImplicitTreeTests.h
		ImplicitTrees/ImplicitTree.h
		ImplicitTrees/ImplicitSplayTree.cpp
		ImplicitTrees/ImplicitCartesianTree.cpp
		ImplicitTrees/ImplicitCartesianTree.h
		Graph/Graph.h
		Graph/GraphTraversal.h
		Graph/GraphTests.h
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
