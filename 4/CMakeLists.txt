set(TP_FILES BlockingQueue.h BlockingQueue.c ThreadPool.h ThreadPool.c packaged_task.h packaged_task.c queue_impl.h queue_impl.c)
set(TSP_FILES ../examples/openmp_dot_product/graph/graph.c ../examples/openmp_dot_product/graph/graph.h Tour.h Tour.c Population.h Population.c)

add_executable(OpenMP4 main.c utils.h ${TP_FILES} ${TSP_FILES})
add_executable(OpenMP4_test unittest.c utils.h ${TP_FILES} ${TSP_FILES})
