add_executable(MPI5 main.c vector_impl.h vector_impl.c walking.h walking.c communicating.h communicating.c)
target_link_libraries(MPI5 ${MPI_C_LIBRARIES})
