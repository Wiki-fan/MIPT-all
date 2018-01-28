#pragma once

/*
void send_array(int rank, MPI_Datatype* arr, int size, MPI_Datatype type) {
    if (rank >= 2) {
        return;
    }
    int type_size;
    MPI_Type_size(type, &type_size);

    int* buf = NULL;
    if (rank == MASTER) {
        int n = 10;
        buf = malloc(n * type_size);
        MPI_Send(&n, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(buf, n, type, 1, 0, MPI_COMM_WORLD);
    } else {
        int n;
        MPI_Recv(&n, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        buf = malloc(n * type_size);
        MPI_Recv(buf, n, type, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i = 0; i < n; i++) {
            printf("buf[%d] = %d\n", i, buf[i]);
        }
    }
    if (buf) {
        free(buf);
    }
}

void send_array_probe(int to, MPI_Datatype* arr, int size, MPI_Datatype type) {
    MPI_Send(arr, size, type, 1, 0, MPI_COMM_WORLD);
}

MPI_Datatype* recv_array_probe(int from, MPI_Datatype type) {
    int n;
    MPI_Status status;
    MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_INT, &n);

    int type_size;
    MPI_Type_size(type, &type_size);

    MPI_Datatype* buf = NULL;
    buf = malloc(n * type_size);
    MPI_Recv(buf, n, MPI_INT, from, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return buf;
}*/

#include <mpi.h>
#include <malloc.h>
#include <time.h>
#include "walking.h"

void send_array_point(int to, Vector_point* points);

Vector_point recv_array_point(int from);

int scatter_seeds(int rank, int size);

int* gather_counts(int rank, int size, int count);
