#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "queue.h"

#define NTHREADS 4
#define THRESHOLD 20

struct pipeline_ctx_t {
	pthread_mutex_t *lock_q, *lock_ev;
	pthread_cond_t *cond_q, *cond_ev;
	queue_t *q;
};

enum event_type {
	SQUARE,
	DONE,
};

struct event_t {
	enum event_type type;
	int value;
};

struct event_t *event_init(const int value, const enum event_type type)
{
	struct event_t *e = calloc(1, sizeof(struct event_t));
	assert(e);
	e->value = value;
	e->type = type;

	return e;
}

void event_destroy(struct event_t *e)
{
	free(e);
}

void *gen(void *context)
{
	struct pipeline_ctx_t *ctx = context;
	for (int i = 0; i < 15; i++) {
		struct event_t *e = event_init(i, SQUARE);

		pthread_mutex_lock(ctx->lock_ev);

		queue_push(ctx->q, e);

		pthread_cond_broadcast(ctx->cond_ev);
		pthread_mutex_unlock(ctx->lock_ev);

		pthread_mutex_lock(ctx->lock_q);
		while (queue_len(ctx->q) >= THRESHOLD) {
			pthread_cond_wait(ctx->cond_q, ctx->lock_q);
		}
		pthread_mutex_unlock(ctx->lock_q);
	}
	
	for (int i = 0; i < NTHREADS - 1; i++) {
		struct event_t *e = event_init(0, DONE);
		pthread_mutex_lock(ctx->lock_ev);
		queue_push(ctx->q, e);
		pthread_cond_broadcast(ctx->cond_ev);
		pthread_mutex_unlock(ctx->lock_ev);
	}

	return NULL;
}

void *work(void *context)
{
	struct pipeline_ctx_t *ctx = context;
	for (;;) {
		sleep(1);

		pthread_mutex_lock(ctx->lock_ev);
		while (queue_len(ctx->q) == 0) {
			pthread_cond_wait(ctx->cond_ev, ctx->lock_ev);
		}
		struct event_t *e = queue_pop(ctx->q);
		pthread_mutex_unlock(ctx->lock_ev);

		pthread_mutex_lock(ctx->lock_q);
		if (queue_len(ctx->q) < THRESHOLD) {
			pthread_cond_signal(ctx->cond_q);
		}
		pthread_mutex_unlock(ctx->lock_q);

		if (!e) {
			continue;
		}

		switch (e->type) {
		case SQUARE:
			;
			int v = e->value * e->value;
			event_destroy(e);
			printf("%d\n", v);
			break;
		case DONE:
			event_destroy(e);
			printf("done\n");
			return NULL;
		default:
			printf("wrong event type\n");
			exit(1);
		}
	}
	return NULL;
}

int main()
{
	pthread_t threads[NTHREADS];
	queue_t *q = queue_init();

	pthread_mutex_t lock_q, lock_ev;
	pthread_cond_t cond_q, cond_ev;
	
	pthread_mutex_init(&lock_q, NULL);
	pthread_cond_init(&cond_q, NULL);
	pthread_mutex_init(&lock_ev, NULL);
	pthread_cond_init(&cond_ev, NULL);
	
	struct pipeline_ctx_t ctx = {
	    .lock_q = &lock_q,
	    .cond_q = &cond_q,
	    .lock_ev = &lock_ev,
	    .cond_ev = &cond_ev,
	    .q = q,
	};
	
	pthread_create(threads, NULL, gen, &ctx);
	for (int i = 1; i < NTHREADS; i++) {
		pthread_create(threads + i, NULL, work, &ctx);
	}

	for (int i = 0; i < NTHREADS; i++) {
		pthread_join(threads[i], NULL);
	}
	pthread_mutex_destroy(&lock_q);
	pthread_cond_destroy(&cond_q);
	pthread_mutex_destroy(&lock_ev);
	pthread_cond_destroy(&cond_ev);
	queue_destroy(q);

	return 0;
}
