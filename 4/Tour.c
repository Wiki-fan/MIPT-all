#include <malloc.h>
#include <tkPort.h>
#include "Tour.h"
#include "Population.h"
#include "utils.h"

#define MUTATION_RATE 0.015

/* Arrange the N elements of ARRAY in random order.
   Only effective if N is much smaller than RAND_MAX;
   if this may not be the case, use a better random
   number generator. */
void shuffle(int* array, size_t n) {
    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

void Tour_init(Tour* t, graph_t* g) {
    t->vertices = calloc(g->n, sizeof(int));
    t->g = g;
}

void Tour_destroy(Tour* t) {
    free(t->vertices);
}

int Tour_weight(Tour* t) {
    int sum = 0;
    for (int i = 0; i < t->g->n; ++i) {
        sum += graph_weight(t->g, t->vertices[i], t->vertices[i + 1]);
    }
    return sum;
}

float Tour_fitness(Tour* t) {
    return 1 / Tour_weight(t);
}

Tour Tour_crossover(Tour* t1, Tour* t2) {
    Tour child;
    Tour_init(&child, t1->g);
    int n = t1->g->n;
    int start_pos = rand() % n;
    int end_pos = rand() % n;

    for (int i = 0; i < n; ++i) {
        if (start_pos < end_pos && i > start_pos && i < end_pos) {
            child.vertices[i] = t1->vertices[i];
        } else if (start_pos > end_pos) {
            if (!(i < start_pos && i > end_pos)) {
                child.vertices[i] = t1->vertices[i];
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        if (Tour_contains_node(&child, t2->vertices[i])) {
            for (int j = 0; j < n; ++j) {
                if (Tour_contains_node(&child, j) == 0) {
                    child.vertices[j] = t2->vertices[i];
                    break;
                }
            }
        }
    }

    return child;
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
        if ((double) rand() / RAND_MAX < MUTATION_RATE) {
            int j = rand() % n;
            swap(tour->vertices[i], tour->vertices[j]);
        }
    }
}

void Tour_generate(Tour* t) {
    for (int i = 0; i < t->g->n; ++i) {
        t->vertices[i] = i;
    }
    shuffle(t->vertices, t->g->n);
}
