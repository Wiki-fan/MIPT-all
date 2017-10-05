#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "../examples/openmp_dot_product/scalar/runner.h"
#include "parmergesort.h"

void parallel_merge_sort_run(void* ctx_void) {
    struct context* ctx = (struct context*) ctx_void;
    parallel_merge_sort(ctx->a, ctx->n, ctx->m, ctx->P);
}

void std_sort_run(void* ctx_void) {
    struct context* ctx = (struct context*) ctx_void;
    qsort(ctx->a, ctx->n, sizeof(int), cmp_int);
}

int main(int args, char* argv[]) {
    srand(time(NULL));
    struct context ctx;
    ctx.n = atoi(argv[1]);
    ctx.m = atoi(argv[2]);
    ctx.P = atoi(argv[3]);
    int* a = gen_rand_ints(ctx.n);

    FILE* outf_data = fopen("data.txt", "w");
    for (int i = 0; i < ctx.n; ++i) {
        fprintf(outf_data, "%d ", a[i]);
    }
    fprintf(outf_data, "\n");

    int* a_copy = malloc(ctx.n * sizeof(int));
    memcpy(a_copy, a, ctx.n * sizeof(int));

    IF_DBG(
        {
            for (int i = 0; i < ctx.n; ++i) {
                printf("%d %d; ", a[i], a_copy[i]);
            }
            printf("\n");
        });

    double time_parallel, time_unparallel;

    ctx.a = a;
    if (ctx.P != 0) {
        time_parallel = runner_run(parallel_merge_sort_run, &ctx, "parallel sort");
    }
    IF_DBG(
        {
            for (int i = 0; i < ctx.n; ++i) {
                printf("%d %d; ", a[i], a_copy[i]);
            }
            printf("\n");
        });

    for (int i = 0; i < ctx.n; ++i) {
        fprintf(outf_data, "%d ", a[i]);
    }
    fprintf(outf_data, "\n");
    fclose(outf_data);

    ctx.a = a_copy;
    time_unparallel = runner_run(std_sort_run, &ctx, "not parallel sort");
    IF_DBG(
        {
            for (int i = 0; i < ctx.n; ++i) {
                printf("%d %d; ", a[i], a_copy[i]);
            }
            printf("\n");
        });

    if (memcmp(a, a_copy, ctx.n * sizeof(int))) {
        printf("Incorrect sorting\n");
    }

    double time = ctx.P != 0 ? time_parallel : time_unparallel;

    FILE* outf_stats = fopen("stats.txt", "a+");
    fprintf(outf_stats,
            "%fs %d %d %d\n",
            time,
            ctx.n,
            ctx.m,
            ctx.P);
    fclose(outf_stats);

    free(a);
    free(a_copy);
    return 0;
}
