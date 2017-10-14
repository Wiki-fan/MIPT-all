#pragma once
#include "Tour.h"
#include "../examples/openmp_dot_product/graph/graph.h"

typedef struct {
    Tour* tours;
    int size;
    graph_t* g;
} Population;

void Population_init(Population* pop, int n, graph_t* g);
void Population_fill_random(Population* pop);
void Population_destroy(Population* pop);
Tour* Population_get_best(Population* pop);
Population Population_evolve(Population* pop);
int Population_get_crossover_candidate(Population* pop);