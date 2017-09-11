#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#include "examples/openmp_dot_product/runner.h"

int getMove(double p, unsigned int* seed) {
    return (rand_r(seed) / (double) RAND_MAX) < p ? -1 : 1;
}

struct context {
    int a, b, x, N, P;
    double p;
    int result, lifetime;
};

void not_parallel_walk(void* ctx_void) {
    struct context* ctx = (struct context*) ctx_void;

    int result = 0;
    int lifetime = 0;

    unsigned int seed = (unsigned int) omp_get_thread_num() + (unsigned int) time(NULL);
    printf("seed: %u\n", seed);
    for (int j = 0; j < ctx->N; ++j) {
        int S = ctx->x;
        for (int i = 0;; ++i) {
            S += getMove(ctx->p, &seed);
            //printf("%d ", S);
            if (S == ctx->a || S == ctx->b) {
                lifetime += i;
                if (S == ctx->a) {
                    result -= 1;
                } else if (S == ctx->b) {
                    result += 1;
                }
                break;
            }
        }
    }

    ctx->result = result;
    ctx->lifetime = lifetime;
}

void parallel_walk(void* ctx_void) {
    struct context* ctx = (struct context*) ctx_void;

    int result = 0;
    int lifetime = 0;

    #pragma omp parallel num_threads(ctx->P)
    {
        //omp_set_num_threads(ctx->P);
        unsigned int seed = (unsigned int) omp_get_thread_num() + (unsigned int) time(NULL);
        printf("seed: %u\n", seed);
        #pragma omp for reduction(+:result) reduction(+:lifetime)
        for (int j = 0; j < ctx->N; ++j) {
            int S = ctx->x;
            for (int i = 0;; ++i) {
                S += getMove(ctx->p, &seed);
                if (S == ctx->a || S == ctx->b) {
                    lifetime += i;
                    if (S == ctx->a) {
                        result -= 1;
                    } else if (S == ctx->b) {
                        result += 1;
                    }
                    break;
                }
            }
        }
    }

    ctx->result = result;
    ctx->lifetime = lifetime;
}

int main(int args, char* argv[]) {
    struct context ctx;
    ctx.a = atoi(argv[1]);
    ctx.b = atoi(argv[2]);
    ctx.x = atoi(argv[3]);
    ctx.N = atoi(argv[4]);
    ctx.p = atof(argv[5]);
    ctx.P = atoi(argv[6]);

    double time;
    if (ctx.P != 0) {
        time = runner_run(parallel_walk, &ctx, "parallel walk");
    } else {
        time = runner_run(not_parallel_walk, &ctx, "not parallel walk");
    }

    FILE* out = fopen("stats.txt", "a+");
    fprintf(out,
            "%f %f %lfs %d %d %d %d %lf %d\n",
            (ctx.result + ctx.N) / (double) (2 * ctx.N),
            ctx.lifetime / (double) ctx.N,
            time,
            ctx.a,
            ctx.b,
            ctx.x,
            ctx.N,
            ctx.p,
            ctx.P);
    fclose(out);
    return 0;
}
