cmake_minimum_required(VERSION 3.8)
project(raspreds)

set(CMAKE_C_STANDARD 11)
find_package(OpenMP REQUIRED)
find_package(MPI REQUIRED)

add_subdirectory(1)
add_subdirectory(2)
add_subdirectory(3)
add_subdirectory(4)
add_subdirectory(5)

if (MPI_FOUND)
    include_directories(${MPI_INCLUDE_PATH})
    if(MPI_COMPILE_FLAGS)
        set_target_properties(MPI5 PROPERTIES
                COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
    endif()

    if(MPI_LINK_FLAGS)
        set_target_properties(MPI5 PROPERTIES
                LINK_FLAGS "${MPI_LINK_FLAGS}")
    endif()
endif()

if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} -pthread -fsanitize=thread -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS} ")
endif()


