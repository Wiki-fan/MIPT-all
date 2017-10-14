#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

typedef struct {
    int t, N, S;
} context;

#include "../examples/openmp_dot_product/scalar/runner.h"

int main(int args, char* argv[]) {
    srand(time(NULL));
    context ctx;
    ctx.t = atoi(argv[1]);
    ctx.N = atoi(argv[2]);
    ctx.S = atoi(argv[3]);

    double time_parallel, time_unparallel;

    return 0;
}
