#pragma once
#include "../examples/openmp_dot_product/graph/graph.h"

typedef struct {
    int* vertices;
    graph_t* g;
} Tour;

void Tour_init(Tour* t, graph_t* g);
void Tour_destroy(Tour* t);
int Tour_weight(Tour* t);
float Tour_fitness(Tour* t);
int Tour_contains_node(Tour* t, int v);
Tour Tour_crossover(Tour* t1, Tour* t2);
void Tour_mutate(Tour* tour);
void Tour_generate(Tour* t);

