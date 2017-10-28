#include <stdatomic.h>
#include <malloc.h>
#include <stdlib.h>
#include "utils.h"

struct context {
    int m, n, P;
    int* a;
};

typedef struct {
    int* array;
    int l;
    int r;
    int* dst;
    int src_to_dst;
    int chunk_size;
    atomic_int* free_threads;
} thread_data_t;

int* gen_rand_ints(int n);

int cmp_int(const void* a, const void* b);

void merge(int* a_start, int* a_end, int* b_start, int* b_end, int* dst);

void* parmergesort(void* arg);

int* parallel_merge_sort(int* a, int n, int m, int P);
