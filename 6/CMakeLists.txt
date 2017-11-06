add_executable(MPI6 main.c vector.h walking.h walking.c communicating.h communicating.c utils.h)
target_link_libraries(MPI6 ${MPI_C_LIBRARIES})
