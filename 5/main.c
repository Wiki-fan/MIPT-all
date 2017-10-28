#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../examples/openmp_dot_product/scalar/runner.h"
#include "walking.h"
#include "communicating.h"

// Проверить, нужно ли отправить точку в другой узел, и если да, положить в нужный массив и обнулить.
int check_if_leaved(context* ctx, point* pnt, Vector_point* vec_send) {
    if (pnt->x < 0 - OVERLAP) {
        // вниз
        Vector_point_push(&vec_send[0], *pnt);
        pnt->i = 0;
    }
    if (pnt->x > ctx->l + OVERLAP) {
        // вверх
        Vector_point_push(&vec_send[1], *pnt);
        pnt->i = 0;
    }
    if (pnt->y < 0 - OVERLAP) {
        //влево
        Vector_point_push(&vec_send[2], *pnt);
        pnt->i = 0;
    }
    if (pnt->y > ctx->l + OVERLAP) {
        //вправо
        Vector_point_push(&vec_send[3], *pnt);
        pnt->i = 0;
    }
}

void merge_with_vector(Vector_point* big, Vector_point* small) {
    int last_free_in_big = 0;
    int i = 0;
    for (; i<small->size && last_free_in_big < big->size; ++i) {
        while (big->arr[last_free_in_big].i != 0 && last_free_in_big < big->size) {
            ++last_free_in_big;
        }
        big->arr[last_free_in_big] = small->arr[i];
    }

    for (; i<small->size; ++i) {
        Vector_point_push(big, small->arr[i]);
    }
}

void communicate_for(int to, context* ctx, Vector_point* vec_send, Vector_point* my_points) {
    printf("Target is %d\n", to);
    if (ctx->rank % 2 == 0) {
        send_array_point(to, &vec_send[0]);

        Vector_point buf;
        buf = recv_array_point(to);
        merge_with_vector(my_points, &buf);
        Vector_point_destroy(&buf);
    } else {
        Vector_point buf;
        buf = recv_array_point(to);
        merge_with_vector(my_points, &buf);
        Vector_point_destroy(&buf);

        send_array_point(to, &vec_send[0]);
    }
}

void custom(void* arg) {
    context* ctx = (context*) arg;

    int seed = scatter_seeds(ctx->rank, ctx->size);
    printf("%d: seed = %d\n", ctx->rank, seed);
    srand(seed);

    thread_data td;
    Vector_point_init(&td.points, ctx->N);
    td.points.size = ctx->N;
    for (int j = 0; j < ctx->N; ++j) {
        // Точка живёт от (n+1) до 1. Не 0, ибо так проверяется занятость клетки массива.
        td.points.arr[j].i = ctx->n+1;
        td.points.arr[j].x = rand() % ctx->l;
        td.points.arr[j].y = rand() % ctx->l;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // Делаем ITERS_PER_EXCHANGE итераций, обмениваемся точками и синхронизируемся барьером. При этом в синхронизации все точки прошли одинаково итераций. Можно просто определить количество нужных повторений внешнего цикла.
    for (int i = 0; i<ctx->n/ITERS_PER_EXCHANGE; ++i) {
        for (int j = 0; j < ITERS_PER_EXCHANGE; ++j) {
            if (!make_step(&td, ctx)) {
                break;
            }
        }

        printf("Process %d has: ", ctx->rank);
        for (int j = 0; j<td.points.size; ++j) {
            printf("%d %d %d | ", td.points.arr[j].x, td.points.arr[j].y, td.points.arr[j].i);
        }
        printf("\n");

        // Формируем массивы и отправляем.
        Vector_point vec_send[4];
        for (int j = 0; j<4; ++j) {
            Vector_point_init(&vec_send[j], 10);
        }

        for (int j = 0; j < td.points.size; ++j) {
            pointp pnt = &td.points.arr[j];
            check_if_leaved(ctx, pnt, vec_send);
        }

        printf("Process %d wants to exchange %d %d %d %d\n", ctx->rank, vec_send[0].size, vec_send[1].size, vec_send[2].size,vec_send[3].size);

        MPI_Barrier(MPI_COMM_WORLD);
        communicate_for((abs(ctx->rank - 1)) % ctx->a + (ctx->rank / ctx->a)*ctx->a, ctx, &vec_send[0], &td.points);
        communicate_for((abs(ctx->rank + 1)) % ctx->a + (ctx->rank / ctx->a)*ctx->a, ctx, &vec_send[1], &td.points);
        communicate_for((abs(ctx->rank - ctx->a)) % ctx->size, ctx, &vec_send[2], &td.points);
        communicate_for((abs(ctx->rank + ctx->a)) % ctx->size, ctx, &vec_send[3], &td.points);
        MPI_Barrier(MPI_COMM_WORLD);

        printf("Iteration completed\n");
    }


    MPI_Barrier(MPI_COMM_WORLD);

    int count = 0;
    for (int i = 0; i<td.points.size; ++i) {
        if (td.points.arr[i].i != 0) {
            ++count;
        }
    }

    printf("Process rank %d ended with count=%d\n", ctx->rank, count);

    int* counts = gather_counts(ctx->rank, ctx->size, count);
    if (ctx->rank == MASTER) {
        printf("Results:\n");
        for (int i = 0; i<ctx->size; ++i) {
            printf("%d: %d\n", i, counts[i]);
        }
    }
}

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
    double time;
    for (int i = 0; i<4; ++i) {
        printf("%f ", ctx.cdf[i]);
    }

    time = runner_run(custom, &ctx, "walk");

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

        fclose(out);
    }
    MPI_Finalize();
    return 0;
}
