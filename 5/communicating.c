#include "communicating.h"

void send_array_point(int to, Vector_point* points) {
    //MPI_Send(&points->size, 1, MPI_INT, to, 0, MPI_COMM_WORLD);
    MPI_Send(points->arr, points->size*sizeof(point), MPI_BYTE, to, 0, MPI_COMM_WORLD);
}

Vector_point recv_array_point(int from) {
    int n;
    MPI_Status status;
    MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_INT, &n);
    //MPI_Recv(&n, 1, MPI_INT, from, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    point* buf = NULL;
    buf = malloc(n * sizeof(point));
    MPI_Recv(buf, n*sizeof(point), MPI_BYTE, from, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    Vector_point v;
    v.arr = buf;
    v.size = v.max_size = n;
    return v;
}

int scatter_seeds(int rank, int size) {
    int* buf = NULL;
    if (rank == MASTER) {
        srand(time(NULL));
        buf = calloc(size, sizeof(int));
        for (int i = 0; i < size; i++) {
            buf[i] = rand();
        }
    }
    int seed;
    MPI_Scatter(buf, 1, MPI_INT, &seed, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
    if (buf != NULL) {
        free(buf);
    }
    return seed;
}

int* gather_counts(int rank, int size, int count) {
    int* buf = NULL;
    if (rank == MASTER) {
        buf = calloc(size, sizeof(int));
    }
    MPI_Gather(&count, 1, MPI_INT, buf, 1, MPI_INT, MASTER, MPI_COMM_WORLD);

    return buf;
}
