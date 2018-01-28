#define DEBUG
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "../examples/openmp_dot_product/scalar/runner.h"
#include "walking.h"
#include "communicating.h"
#include "utils.h"

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    context ctx;
    ctx.l = atoi(argv[1]);
    ctx.a = atoi(argv[2]);
    ctx.b = atoi(argv[3]);
    ctx.n = atoi(argv[4]);
    ctx.N = atoi(argv[5]);
    ctx.pl = atof(argv[6]);
    ctx.pr = atof(argv[7]);
    ctx.pu = atof(argv[8]);
    ctx.pd = atof(argv[9]);
    ctx.cdf[0] = ctx.pl;
    ctx.cdf[1] = ctx.cdf[0] + ctx.pr;
    ctx.cdf[2] = ctx.cdf[1] + ctx.pu;
    ctx.cdf[3] = ctx.cdf[2] + ctx.pd;
    ctx.rank = rank;
    ctx.size = size;
    assert(size == ctx.a*ctx.b);
    double time;

    time = runner_run(custom, &ctx, "walk");

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == MASTER) {
        FILE* out = fopen("stats.txt", "a+");
        fprintf(out,
                "%d %d %d %d %d %f %f %f %f %fs\n",
                ctx.l,
                ctx.a,
                ctx.b,
                ctx.n,
                ctx.N,
                ctx.pl,
                ctx.pr,
                ctx.pu,
                ctx.pd,
                time);
        for (int i = 0; i < ctx.size; ++i) {
            fprintf(out, "%d: %d\n", i, ctx.counts[i]);
        }
        fclose(out);

        int sum = 0;
        for (int i = 0; i<ctx.size; ++i) {
            sum += ctx.counts[i];
        }
        assert(sum == ctx.N*ctx.a*ctx.b);
        free(ctx.counts);
    }

    MPI_Finalize();
    return 0;
}
