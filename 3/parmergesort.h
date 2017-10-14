#include <stdatomic.h>
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

void* parmergesort(void* arg) {
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
        return NULL;
    }
    if (r - l <= chunk_size && !src_to_dst) {
        qsort(src + l, r - l + 1, sizeof(int), cmp_int);
        return NULL;
    }

    int m = (r + l) / 2;

    // parallel
    thread_data_t data1 = {src, l, m, dst, !src_to_dst, chunk_size, data->free_threads};
    pthread_t t1;
    int t1_in_new_thread = 0;
    int old_free_threads1 = atomic_fetch_sub(data->free_threads, 1);
    if (old_free_threads1 > 0) {
        pthread_create(&t1, NULL, &parmergesort, &data1);
        t1_in_new_thread = 1;
    }

    if (!t1_in_new_thread) {
        parmergesort(&data1);
    }

    // parallel
    thread_data_t data2 = {src, m + 1, r, dst, !src_to_dst, chunk_size, data->free_threads};
    pthread_t t2;
    int t2_in_new_thread = 0;

    int old_free_threads2 = atomic_fetch_sub(data->free_threads, 1);
    if (old_free_threads2 > 0) {
        pthread_create(&t2, NULL, &parmergesort, &data2);
        t2_in_new_thread = 1;
    }

    if (!t2_in_new_thread) {
        parmergesort(&data2);
    }

    // joining
    if (t1_in_new_thread) {
        void* ret;
        pthread_join(t1, &ret);
    }
    if (t2_in_new_thread) {
        void* ret;
        pthread_join(t2, &ret);
    }

    int l1 = l;
    int r1 = m;
    int l2 = m + 1;
    int r2 = r;

    int len1 = r1 - l1 + 1;
    int len2 = r2 - l2 + 1;
    if (src_to_dst) {
        merge(&src[l1], &src[l1 + len1], &src[l2], &src[l2 + len2], &dst[l]);
    } else {
        merge(&dst[l1], &dst[l1 + len1], &dst[l2], &dst[l2 + len2], &src[l]);
    }

}

int* parallel_merge_sort(int* a, int n, int m, int P) {
    int* res = malloc(n * sizeof(int));
    atomic_int free_threads = P - 1;
    thread_data_t data = {a, 0, n - 1, res, 0, m, &free_threads};
    parmergesort(&data);
    free(res);
}
