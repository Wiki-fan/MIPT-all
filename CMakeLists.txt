cmake_minimum_required(VERSION 3.3)
project(didactic_woof)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

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
        Work/HeapTests.cpp
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
        Work/HeapTests.h
        Work/HeapTests.h
        Work/TestBracketSequence.cpp
        Work/TestBracketSequence.h
        Work/MeldableHeap.h Work/BinomialHeap.h Work/SingleLinkedList.cpp Work/CalcTests.h Work/PtrOwner.h Work/VectorTests.h)

add_executable(didactic_woof ${SOURCE_FILES})