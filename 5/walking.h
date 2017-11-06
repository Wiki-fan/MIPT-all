#pragma once

/*
point point;
MPI_Datatype types[3] = {MPI_INT, MPI_INT, MPI_INT};
int blocks[3] = {1, 1, 1};
MPI_Aint displacements[3];
MPI_Get_address(&point, displacements);
MPI_Get_address(&point.y, displacements + 1);
MPI_Get_address(&point.i, displacements + 2);

long base = displacements[0];
for (int i = 0; i < 3; i++) {
    displacements[i] -= base;
}

MPI_Datatype datatype;
MPI_Type_create_struct(3, blocks, displacements, types, &datatype);
MPI_Type_commit(&datatype);
*/

#include <assert.h>
#include "stdlib.h"
#include "vector.h"
#include "utils.h"

#define OVERLAP 5
#define MASTER 0
#define ITERS_PER_EXCHANGE 1000

typedef struct {
    int l, a, b, n, N;
    float pl, pr, pu, pd;
    float cdf[4];
    int rank, size;
    int* counts;
} context;

typedef struct {
    int x, y, i;
} point;
typedef point* pointp;

define_vector(point)

typedef struct {
    Vector_point points;
    int n;
} thread_data;

int make_step(thread_data* td, context* ctx);

void print_vector_points(Vector_point* v);

void merge_with_vector(Vector_point* big, Vector_point* small);

void communicate_for(int prev, int next, context* ctx, Vector_point* vec_send, Vector_point* my_points);

void custom(void* arg);
