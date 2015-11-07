cmake_minimum_required(VERSION 3.3)
project(didactic_woof)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

set(SOURCE_FILES
    Work/Brackets.cpp
    Work/Calc.cpp
    Work/Calc.h
    Work/CalcJudge.cpp
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
    Work/Heap.h
    Work/HeapForMerge.cpp
    Work/HeapForMerge.h
    Work/HeapTests.cpp
    Work/main.cpp
    Work/main_contest.cpp
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
    Work/VectorTests.cpp)

add_executable(didactic_woof ${SOURCE_FILES})