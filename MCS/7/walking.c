#include <malloc.h>
#include "walking.h"
#include "communicating.h"

implement_vector(point)

int mod(int x, int y) {
    return (x % y) >= 0 ? x % y : y + x % y;
}

int make_step(thread_data* td, context* ctx) {
    int some_node_alive = 0;
    for (int i = 0; i < td->points.size; ++i) {
        point* pnt = &td->points.arr[i];

        // Если точки на самом деле нет, сразу пропускаем.
        if (pnt->i == 0) {
            continue;
        }

        if (pnt->i != 1) {
            --pnt->i;
            some_node_alive = 1;
        } else {
            continue;
        }
        float p = (float) rand() / RAND_MAX;

        if (p <= ctx->cdf[0]) {
            pnt->x -= 1;
        } else if (p <= ctx->cdf[1]) {
            pnt->x += 1;
        } else if (p <= ctx->cdf[2]) {
            pnt->y += 1;
        } else if (p <= ctx->cdf[3]) {
            pnt->y -= 1;
        } else {
            printf("p=%f", p);
            assert(0);
        }
    }
    return some_node_alive;
}

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
            while (last_free_in_big < big->size && big->arr[last_free_in_big].i != 0) {
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
    int coef = ctx->rank % 2 != next % 2 ? 1 : ctx->a;// TODO: или b?
    //printf("coef %d\n", coef);

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
        td.points.arr[j].r = ctx->rank;
    }

    // Делаем ITERS_PER_EXCHANGE итераций, обмениваемся точками и синхронизируемся барьером. При этом в синхронизации все точки прошли одинаково итераций. Можно просто определить количество нужных повторений внешнего цикла.
    for (int i = 0; i < ctx->n / ITERS_PER_EXCHANGE + 1; ++i) {
        for (int j = 0; j < ITERS_PER_EXCHANGE; ++j) {
            if (!make_step(&td, ctx)) {
                break;
            }
        }

        debug(
            printf("Process %d has: ", ctx->rank);
            for (int j = 0; j < td.points.size; ++j) {
                printf("%d %d %d | ", td.points.arr[j].x, td.points.arr[j].y, td.points.arr[j].i);
            }
            printf("\n");
        );

        // Формируем массивы и отправляем.
        Vector_point vec_send[4];
        for (int j = 0; j < 4; ++j) {
            Vector_point_init(&vec_send[j], 10);
        }

        for (int j = 0; j < td.points.size; ++j) {
            pointp pnt = &td.points.arr[j];
            check_if_leaved(ctx, pnt, vec_send);
        }

        debug(printf("Process %d wants to exchange %d %d %d %d\n",
                     ctx->rank,
                     vec_send[0].size,
                     vec_send[1].size,
                     vec_send[2].size,
                     vec_send[3].size));

        int up = mod(ctx->rank - ctx->a, ctx->size);
        int down = mod(ctx->rank + ctx->a, ctx->size);
        int left = mod(ctx->rank - 1, ctx->a) + (ctx->rank / ctx->a) * ctx->a;
        int right = mod(ctx->rank + 1, ctx->a) + (ctx->rank / ctx->a) * ctx->a;
        debug(printf("For %d %d:%d:%d:%d %d\n", ctx->rank, left, right, up, down, ctx->size));
        communicate_for(left, right, ctx, &vec_send[0], &td.points);
        communicate_for(right, left, ctx, &vec_send[1], &td.points);
        communicate_for(up, down, ctx, &vec_send[2], &td.points);
        communicate_for(down, up, ctx, &vec_send[3], &td.points);

        debug(printf("Iteration completed\n"));
    }

    MPI_Barrier(MPI_COMM_WORLD);

    int count = 0;
    for (int i = 0; i < td.points.size; ++i) {
        if (td.points.arr[i].i != 0) {
            assert(td.points.arr[i].i == 1);
            ++count;
        }
    }

    debug(printf("Process rank %d ended with count=%d\n", ctx->rank, count));

    int* counts = gather_counts(ctx->rank, ctx->size, count);

    debug(
        if (ctx->rank == MASTER) {
            printf("Results:\n");
            for (int i = 0; i < ctx->size; ++i) {
                printf("%d: %d\n", i, counts[i]);
            }
        }
    );

    int* buf = calloc(ctx->size, sizeof(int));
    for (int i = 0; i < td.points.size; ++i) {
        if (td.points.arr[i].i == 1) {
            ++buf[td.points.arr[i].r];
        }
    }

    debug(
        printf("for rank %d\n", ctx->rank);
        for (int i = 0; i < ctx->size; ++i) {
            printf("%d ", buf[i]);
        }
        printf("\n");
    );

    int offset = (ctx->rank * ctx->size);
    MPI_Aint intex, lbound;
    MPI_Type_get_extent(MPI_INT, &lbound, &intex);

    MPI_File fh;
    MPI_File_open(MPI_COMM_WORLD, "data.bin", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
    MPI_File_write_at(fh, offset * intex, buf, ctx->size, MPI_INT, MPI_STATUS_IGNORE);
    MPI_File_close(&fh);
    ctx->counts = counts;

    Vector_point_destroy(&td.points);
}
