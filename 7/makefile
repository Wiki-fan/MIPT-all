
MPI5:
	mpicc -g -o run main.c walking.c communicating.c

launch:
	mpirun -n 4 ./run 10 2 2 100 10 0.25 0.25 0.25 0.25

launch3:
	mpirun -n 9 ./run 10 3 3 100 10 0.25 0.25 0.25 0.25

runvg:
	LD_PRELOAD=$prefix/lib/valgrind/libmpiwrap-amd64-linux.so mpirun -n 4 $(prefix)/bin/valgrind --leak-check=full ./run 10 2 2 100 10 0.25 0.25 0.25 0.25
