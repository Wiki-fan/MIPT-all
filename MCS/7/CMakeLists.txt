add_executable(MPI7 main.c vector.h walking.h walking.c communicating.h communicating.c utils.h)
target_link_libraries(MPI7 ${MPI_C_LIBRARIES})
