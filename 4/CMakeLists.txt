add_executable(OpenMP4 main.c utils.h BlockingQueue.h ThreadPool.h packaged_task.h packaged_task.c queue_impl.h queue_impl.c)
add_executable(OpenMP4_test unittest.c utils.h BlockingQueue.h ThreadPool.h packaged_task.h packaged_task.c queue_impl.h queue_impl.c)
