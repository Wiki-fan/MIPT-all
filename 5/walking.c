#include <malloc.h>
#include "walking.h"

implement_vector(point)

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
