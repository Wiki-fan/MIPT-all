#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "../examples/openmp_dot_product/graph/graph.h"
//#include "BlockingQueue.h"
#include "BlockingQueue_univ.h"
#include "Population.h"

typedef struct {
    int t, N, S;
} context;




void* number_generator_work(void* arg) {
    blocking_queue* q = (blocking_queue*) arg;
    while (blocking_queue_put(q, (long long) rand() % 1000)) {}
}

int main(int args, char* argv[]) {
    srand(time(NULL));
    context ctx;
    ctx.t = atoi(argv[1]);
    ctx.N = atoi(argv[2]);
    ctx.S = atoi(argv[3]);

    graph_t* graph;
    if (!strcmp(argv[4], "--generate")) {
        int node_number = atoi(argv[5]);
        graph = graph_generate(node_number, 1000);
    } else {
        graph = graph_read_file(argv[5]);
    }

    // graph_dump(graph, stdout);

    blocking_queue random_numbers;
    blocking_queue_init(&random_numbers, 1000);
    pthread_t number_generator;
    pthread_create(&number_generator, NULL, number_generator_work, &random_numbers);

    Population pop;
    Population_init(&pop, ctx.N, graph);
    Population_fill_random(&pop);

    for (int i = 0; i < 10; ++i) {
        printf("%d ", Tour_weight(Population_get_best(&pop)));
        pop = Population_evolve(&pop);
    }

    blocking_queue_shutdown(&random_numbers);
    pthread_join(number_generator, NULL);
    blocking_queue_destroy(&random_numbers);

    return 0;
}
