#include <malloc.h>
#include <tkPort.h>
#include "Tour.h"
#include "Population.h"

#define MUTATION_RATE 0.015

/* Arrange the N elements of ARRAY in random order.
   Only effective if N is much smaller than RAND_MAX;
   if this may not be the case, use a better random
   number generator. */
void shuffle(int* array, size_t n) {
    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; i++) {
            size_t j = i + rand_mt() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

void Tour_init(Tour* t, graph_t* g) {
    t->vertices = malloc(g->n * sizeof(int));
    for (int i = 0; i < g->n; ++i) {
        t->vertices[i] = -1;
    }
    t->g = g;
}

void Tour_destroy(Tour* t) {
    free(t->vertices);
}

void Tour_print(Tour* t) {
    for (int i = 0; i < t->g->n; ++i) {
        printf("%d ", t->vertices[i]);
    }
    printf("\n");
}

int Tour_weight(Tour* t) {
    int sum = 0;
    for (int i = 0; i < t->g->n; ++i) {
        sum += graph_weight(t->g, t->vertices[i], t->vertices[i + 1]);
    }
    return sum;
}

float Tour_fitness(Tour* t) {
    return 1 / (float) Tour_weight(t);
}

Tour Tour_crossover(Tour* t1, Tour* t2) {
    Tour child;
    Tour_init(&child, t1->g);
    int n = t1->g->n;
    int start_pos = rand_mt() % n;
    int end_pos = rand_mt() % n;

    // Из зацикленного массива берём кусок от start_pos до end_pos и переносим в ребёнка (по тем же индексам).
    for (int i = 0; i < n; ++i) {
        if (start_pos < end_pos && i > start_pos && i < end_pos) {
            child.vertices[i] = t1->vertices[i];
        } else if (start_pos > end_pos) {
            if (!(i < start_pos && i > end_pos)) {
                child.vertices[i] = t1->vertices[i];
            }
        }
    }

    // Идём по вершинам второго пути и, если они не попали в ребёнка, вставляем их на ближайшее доступное место в ребёнке.
    // За квадрат.
    for (int i = 0; i < n; ++i) {
        if (!Tour_contains_node(&child, t2->vertices[i])) {
            for (int j = 0; j < n; ++j) {
                if (child.vertices[j] == -1) {
                    child.vertices[j] = t2->vertices[i];
                    break;
                }
            }
        }
    }

    IF_DBG(printf("Crossover for start=%d end=%d:\n", start_pos, end_pos);
               Tour_print(t1);
               Tour_print(t2);
               Tour_print(&child));

    return child;
}

void* Tour_crossover_tp(void* void_arg) {
    Tour_crossover_param* arg = (Tour_crossover_param*) void_arg;
    *arg->ans_ptr = Tour_crossover(arg->t1, arg->t2);
    //free(arg);
}

int Tour_contains_node(Tour* t, int v) {
    for (int i = 0; i < t->g->n; ++i) {
        if (t->vertices[i] == v) {
            return 1;
        }
    }
    return 0;
}

void Tour_mutate(Tour* tour) {
    int n = tour->g->n;
    for (int i = 0; i < n; ++i) {
        if ((double) rand_mt() / RAND_MAX < MUTATION_RATE) {
            int j = rand_mt() % n;
            swap(tour->vertices[i], tour->vertices[j]);
        }
    }
}

void* Tour_mutate_tp(void* void_arg) {
    Tour_crossover_param* arg = (Tour_crossover_param*) void_arg;
    Tour_mutate(arg->t1);
    //free(arg);
}

void Tour_generate(Tour* t) {
    for (int i = 0; i < t->g->n; ++i) {
        t->vertices[i] = i;
    }
    shuffle(t->vertices, t->g->n);
}
