#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "../examples/openmp_dot_product/scalar/runner.h"
#include "walking.h"
#include "communicating.h"
#include "utils.h"

// Проверить, нужно ли отправить точку в другой узел, и если да, положить в нужный массив и обнулить.
int check_if_leaved(context* ctx, point* pnt, Vector_point* vec_send) {
    if (pnt->x < 0 - OVERLAP) {
        // вниз
        Vector_point_push(&vec_send[0], *pnt);
        pnt->i = 0;
    } else if (pnt->x > ctx->l + OVERLAP) {
        // вверх
        Vector_point_push(&vec_send[1], *pnt);
        pnt->i = 0;
    } else if (pnt->y < 0 - OVERLAP) {
        //влево
        Vector_point_push(&vec_send[2], *pnt);
        pnt->i = 0;
    } else if (pnt->y > ctx->l + OVERLAP) {
        //вправо
        Vector_point_push(&vec_send[3], *pnt);
        pnt->i = 0;
    }
}

void print_vector_points(Vector_point* v) {
    for (int i = 0; i < v->size; ++i) {
        printf("%d %d %d| ", v->arr[i].x, v->arr[i].y, v->arr[i].i);
    }
    printf("\n");
}

void merge_with_vector(Vector_point* big, Vector_point* small) {
    if (small->size != 0) {
        debug(
            printf("Merging %d\n", small->max_size);
            print_vector_points(big);
            print_vector_points(small);
        );
        int last_free_in_big = 0;
        int i = 0;
        for (; i < small->size && last_free_in_big < big->size;) {
            while (big->arr[last_free_in_big].i != 0 && last_free_in_big < big->size) {
                ++last_free_in_big;
            }
            if (last_free_in_big < big->size) {
                big->arr[last_free_in_big] = small->arr[i];
                ++i;
            }
        }

        for (; i < small->size; ++i) {
            Vector_point_push(big, small->arr[i]);
        }
        debug(
            print_vector_points(big);
            printf("\n");
        );
    }
}

void communicate_for(int prev, int next, context* ctx, Vector_point* vec_send, Vector_point* my_points) {
    debug(printf("Me is %d, next=%d, prev=%d\n", ctx->rank, next, prev));
    int coef = ctx->rank % 2 != next % 2 ? 1 : ctx->l;

    if ((ctx->rank / coef) % 2 == 0) {
        send_array_point(next, vec_send);

        Vector_point buf;
        buf = recv_array_point(prev);
        merge_with_vector(my_points, &buf);
        Vector_point_destroy(&buf);
    } else {
        Vector_point buf;
        buf = recv_array_point(prev);
        merge_with_vector(my_points, &buf);
        Vector_point_destroy(&buf);

        send_array_point(next, vec_send);
    }
    debug(printf("%d communicated\n", ctx->rank));
}

int mod(int x, int y) {
    return (x % y) >= 0 ? x % y : y + x % y;
}

void custom(void* arg) {
    context* ctx = (context*) arg;

    int seed = scatter_seeds(ctx->rank, ctx->size);
    debug(printf("%d: seed = %d\n", ctx->rank, seed));
    srand(seed);

    thread_data td;
    Vector_point_init(&td.points, ctx->N);
    td.points.size = ctx->N;
    for (int j = 0; j < ctx->N; ++j) {
        // Точка живёт от (n+1) до 1. Не 0, ибо так проверяется занятость клетки массива.
        td.points.arr[j].i = ctx->n + 1;
        td.points.arr[j].x = rand() % ctx->l;
        td.points.arr[j].y = rand() % ctx->l;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // Делаем ITERS_PER_EXCHANGE итераций, обмениваемся точками и синхронизируемся барьером. При этом в синхронизации все точки прошли одинаково итераций. Можно просто определить количество нужных повторений внешнего цикла.
    for (int i = 0; i < ctx->n / ITERS_PER_EXCHANGE; ++i) {
        for (int j = 0; j < ITERS_PER_EXCHANGE; ++j) {
            if (!make_step(&td, ctx)) {
                break;
            }
        }

        printf("Process %d has: ", ctx->rank);
        for (int j = 0; j < td.points.size; ++j) {
            printf("%d %d %d | ", td.points.arr[j].x, td.points.arr[j].y, td.points.arr[j].i);
        }
        printf("\n");

        // Формируем массивы и отправляем.
        Vector_point vec_send[4];
        for (int j = 0; j < 4; ++j) {
            Vector_point_init(&vec_send[j], 10);
        }

        for (int j = 0; j < td.points.size; ++j) {
            pointp pnt = &td.points.arr[j];
            check_if_leaved(ctx, pnt, vec_send);
        }

        printf("Process %d wants to exchange %d %d %d %d\n",
               ctx->rank,
               vec_send[0].size,
               vec_send[1].size,
               vec_send[2].size,
               vec_send[3].size);

        MPI_Barrier(MPI_COMM_WORLD);
        int up = mod(ctx->rank - ctx->a, ctx->size);
        int down = mod(ctx->rank + ctx->a, ctx->size);
        int left = (abs(ctx->rank - 1)) % ctx->a + (ctx->rank / ctx->a) * ctx->a;
        int right = (abs(ctx->rank + 1)) % ctx->a + (ctx->rank / ctx->a) * ctx->a;
        communicate_for(left, right, ctx, &vec_send[0], &td.points);
        communicate_for(right, left, ctx, &vec_send[1], &td.points);
        communicate_for(up, down, ctx, &vec_send[2], &td.points);
        communicate_for(down, up, ctx, &vec_send[3], &td.points);
        MPI_Barrier(MPI_COMM_WORLD);

        printf("Iteration completed\n");
    }

    MPI_Barrier(MPI_COMM_WORLD);

    int count = 0;
    for (int i = 0; i < td.points.size; ++i) {
        if (td.points.arr[i].i != 0) {
            ++count;
        }
    }

    printf("Process rank %d ended with count=%d\n", ctx->rank, count);

    int* counts = gather_counts(ctx->rank, ctx->size, count);
    if (ctx->rank == MASTER) {
        printf("Results:\n");
        for (int i = 0; i < ctx->size; ++i) {
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
