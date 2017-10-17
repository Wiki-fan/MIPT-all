#include <malloc.h>
#include "Population.h"
#include "utils.h"

void Population_init(Population* pop, int n, graph_t* g) {
    pop->size = n;
    pop->g = g;
    pop->tours = malloc(pop->size * sizeof(Tour));
}

void Population_fill_random(Population* pop) {
    for (int i = 0; i<pop->size; ++i) {
        Tour_init(&pop->tours[i], pop->g);
        Tour_generate(&pop->tours[i]);
    }
}

void Population_destroy(Population* pop) {
    for (int i = 0; i < pop->size; ++i) {
        Tour_destroy(&pop->tours[i]);
    }
    free(pop->tours);
}

Tour* Population_get_best(Population* pop) {
    float best_fitness = Tour_fitness(&pop->tours[0]);
    Tour* best_tour = &pop->tours[0];
    for (int i = 0; i<pop->size; ++i) {
        float fitness = Tour_fitness(&pop->tours[i]);
        if (fitness > best_fitness) {
            best_fitness = fitness;
            best_tour = &pop->tours[i];
        }

    }
    return best_tour;
}

Population Population_evolve(Population* pop) {
    Population new_pop;
    Population_init(&new_pop, pop->size, pop->g);

    for (int i = 0; i < new_pop.size; ++i) {
        int t1 = Population_get_crossover_candidate(pop);
        int t2 = Population_get_crossover_candidate(pop);
        Tour child = Tour_crossover(&pop->tours[t1], &pop->tours[t2]);
        new_pop.tours[i] = child;
    }

    for (int i = 0; i < new_pop.size; ++i) {
        Tour_mutate(&new_pop.tours[i]);
    }

    Population_destroy(pop);

    return new_pop;
}

int Population_get_crossover_candidate(Population* pop) {
    return rand_mt() % pop->size;
}

