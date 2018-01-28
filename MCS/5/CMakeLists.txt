add_executable(MPI5 main.c vector.h walking.h walking.c communicating.h communicating.c utils.h)
target_link_libraries(MPI5 ${MPI_C_LIBRARIES})
