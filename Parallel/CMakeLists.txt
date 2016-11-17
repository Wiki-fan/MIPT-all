
set(TREEMUTEX
		tree_mutex.h
		tree_mutex.cpp)
set(BARRIER
		barrier.h
		barrier.cpp)
set(STEP
		StepRobot.cpp
		StepRobot.h)

add_executable(tree_mutex ${TREEMUTEX})
add_executable(barrier ${BARRIER})
add_executable(step ${STEP})
add_executable(mutex_with_ID ID.h ID.cpp)
