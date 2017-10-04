#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "../examples/openmp_dot_product/runner.h"
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
    int tid;
    int src_to_dst;
    int chunk_size;
} thread_data_t;

typedef struct {
    int* src;
    int l1;
    int r1;
    int l2;
    int r2;
    int* dst;
    int l;
    int chunk_size;
} thread_merge_data_t;

int* gen_rand_ints(int n) {
    int* a = malloc(n * sizeof(int));
    for (int i = 0; i < n; ++i) {
        a[i] = rand() % 100;
    }
    return a;
}

int cmp_int(const void* a, const void* b) {
    int x = *(int*) a;
    int y = *(int*) b;

    return x - y;
}

long binsearch(int val, const int* a, int l, int r) {
    r = MAX(l, r + 1);
    while (l < r) {
        long mid = (l + r) / 2;
        if (val <= a[mid]) {
            r = mid;
        } else {
            l = mid + 1;
        }
    }
    return r;
}

void merge(int* a_start, int* a_end, int* b_start, int* b_end, int* dst) {
    while (a_start < a_end && b_start < b_end) {
        if (*a_start <= *b_start) {
            *dst++ = *a_start++;
        } else {
            *dst++ = *b_start++;
        }
    }

    while (a_start < a_end) {
        *dst++ = *a_start++;
    }
    while (b_start < b_end) {
        *dst++ = *b_start++;
    }

}

// Сливает фрагменты массива t [l1;r1] и [l2;r2], складывая результат в a[l;...]
void* parmerge(void* arg) {//int* src, int l1, int r1, int l2, int r2, int* dst, int l, int chunk_size) {
    thread_merge_data_t* data = (thread_merge_data_t*) arg;
    int* src = data->src;
    int l1 = data->l1;
    int r1 = data->r1;
    int l2 = data->l2;
    int r2 = data->r2;
    int* dst = data->dst;
    int l = data->l;
    int chunk_size = data->chunk_size;

    int len1 = r1 - l1 + 1;
    int len2 = r2 - l2 + 1;

    if (len1 < len2) {
        swap(l1, l2);
        swap(r1, r2);
        swap(len1, len2);
    }
    if (len1 == 0) {
        return NULL;
    }

    if (len1 + len2 <= chunk_size) {
        merge(&src[l1], &src[l1 + len1], &src[l2], &src[l2 + len2], &dst[l]);
    } else {
        int median = (l1 + r1) / 2;
        int target_median = binsearch(src[median], src, l2, r2);
        int size_of_first_half = l + (median - l1) + (target_median - l2);
        dst[size_of_first_half] = src[median];
        pthread_t t1;
        thread_merge_data_t data1 = {src, l1, median - 1, l2, target_median - 1, dst, l, chunk_size};
        pthread_create(&t1, NULL, &parmerge, &data1);
        pthread_t t2;
        thread_merge_data_t data2 = {src, median + 1, r1, target_median, r2, dst, size_of_first_half + 1, chunk_size};
        pthread_create(&t2, NULL, &parmerge, &data2);

    }
}

void parmergesort(void* arg) {//int* src, int l, int r, int* dst, int src_to_dst, int chunk_size) {
    thread_data_t* data = (thread_data_t*) arg;
    int* src = data->array;
    int l = data->l;
    int r = data->r;
    int* dst = data->dst;
    int chunk_size = data->chunk_size;
    int src_to_dst = data->src_to_dst;

    if (r == l) {
        if (src_to_dst) {
            dst[l] = src[l];
        }
        return;
    }
    if (r - l <= chunk_size && !src_to_dst) {
        qsort(src + l, r - l + 1, sizeof(int), cmp_int);
        return;
    }

    int m = (r + l) / 2;

    thread_data_t data1 = {src, l, m, dst, !src_to_dst, chunk_size};
    parmergesort(&data1);

    thread_data_t data2 = {src, m + 1, r, dst, !src_to_dst, chunk_size};
    parmergesort(&data2);

    if (src_to_dst) {
        thread_merge_data_t data_merge = {src, l, m, m + 1, r, dst, l, chunk_size};
        parmerge(&data_merge);
    } else {
        thread_merge_data_t data_merge = {dst, l, m, m + 1, r, src, l, chunk_size};
        parmerge(&data_merge);
    }
}

int* parallel_merge_sort(int* a, int n, int m, int P) {
    int* res = malloc(n * sizeof(int));
    thread_data_t data = {a, 0, n - 1, res, 0, m};
    parmergesort(&data);
    free(res);
}

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
