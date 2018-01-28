#include <malloc.h>
#include "Population.h"
#include <unistd.h>
#include "utils.h"
#include "ThreadPool.h"
#include "Tour.h"
#include "packaged_task.h"

void Population_init(Population* pop, int n, graph_t* g, thread_pool* tp) {
    pop->size = n;
    pop->g = g;
    pop->tours = malloc(pop->size * sizeof(Tour));
    pop->tp = tp;
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
    Population_init(&new_pop, pop->size, pop->g, pop->tp);

    for (int i = 0; i < new_pop.size; ++i) {
        int t1 = Population_get_crossover_candidate(pop);
        int t2 = Population_get_crossover_candidate(pop);
        Tour_crossover_param* arg = malloc(sizeof(Tour_crossover_param));
        arg->t1 = &pop->tours[t1];
        arg->t2 = &pop->tours[t2];
        arg->ans_ptr = &new_pop.tours[i];
        //Tour_crossover_tp(&arg);
        packaged_task* pt = malloc(sizeof(packaged_task));
        pt->func_ptr = Tour_crossover_tp;
        pt->args = arg;
        ThreadPool_Submit(pop->tp, pt);
    }
    ThreadPool_Wait_until_free(pop->tp);

    for (int i = 0; i < new_pop.size; ++i) {
        Tour_crossover_param* arg = malloc(sizeof(Tour_crossover_param));
        arg->t1 = &pop->tours[i];
        packaged_task* pt = malloc(sizeof(packaged_task));
        pt->func_ptr = Tour_mutate_tp;
        pt->args = arg;
        ThreadPool_Submit(pop->tp, pt);
        //Tour_mutate(&new_pop.tours[i]);
    }
    ThreadPool_Wait_until_free(pop->tp);

    Population_destroy(pop);

    return new_pop;
}

int Population_get_crossover_candidate(Population* pop) {
    return rand_mt() % pop->size;
}
