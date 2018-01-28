#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <assert.h>


#include "runner.h"

typedef struct scalar_ctx_t {
	double *a;
	double *b;
	size_t len;
	double res;
} scalar_ctx_t;


double random_double()
{
	return (double)rand() / RAND_MAX * 2.0 - 1.0;
}

void ctor(void *context)
{
	scalar_ctx_t *ctx = context;
	ctx->a = calloc(ctx->len, sizeof(double));
	assert(ctx->a);
	
	ctx->b = calloc(ctx->len, sizeof(double));
	assert(ctx->b);

	srand(time(NULL));

	for (uint i = 0; i < ctx->len; i++) {
		ctx->a[i] = random_double();
		ctx->b[i] = random_double();
	}
}

void dtor(void *context)
{
	scalar_ctx_t *ctx = context;
	free(ctx->a);
	free(ctx->b);
}

void scalar_serial(void *context)
{
	scalar_ctx_t *ctx = context;

	double sum = 0;
	for (uint i = 0; i < ctx->len; i++) {
		sum += ctx->a[i] * ctx->b[i];
	}
	printf("sum = %lf\n", sum);
	ctx->res = sum;
}


void scalar_mutex(void *context)
{
	scalar_ctx_t *ctx = context;

	omp_lock_t lock;
	omp_init_lock(&lock);
	double sum = 0;
	
	#pragma omp parallel for
	for (uint i = 0; i < ctx->len; i++) {
		double t = ctx->a[i] * ctx->b[i];
		omp_set_lock(&lock);
		sum += t;
		omp_unset_lock(&lock);
	}
	omp_destroy_lock(&lock);
	printf("sum = %lf\n", sum);
	ctx->res = sum;
}


void scalar_critical(void *context)
{
	scalar_ctx_t *ctx = context;

	double sum = 0;
	#pragma omp parallel for reduction(+ : sum)
	for (uint i = 0; i < ctx->len; i++) {
		double t = ctx->a[i] * ctx->b[i];
		#pragma omp critical
		{
			sum += t;
		}
	}
	printf("sum = %lf\n", sum);
	ctx->res = sum;
}

void scalar_atomic(void *context)
{
	scalar_ctx_t *ctx = context;

	double sum = 0;
	#pragma omp parallel for
	for (uint i = 0; i < ctx->len; i++) {
		#pragma omp atomic
		sum += ctx->a[i] * ctx->b[i];
	}
	printf("sum = %lf\n", sum);
	ctx->res = sum;
}

void scalar_reduction(void *context)
{
	scalar_ctx_t *ctx = context;

	double sum = 0;
	#pragma omp parallel for reduction(+ : sum)
	for (uint i = 0; i < ctx->len; i++) {
		sum += ctx->a[i] * ctx->b[i];
	}
	printf("sum = %lf\n", sum);
	ctx->res = sum;
}

double dot_product(double *a, double *b, int len)
{
	if (len == 0) {
		return 0;
	}
	if (len == 1) {
		return *a * *b;
	}

	int half = len / 2;
	double x, y;
	x = dot_product(a, b, half);
	y = dot_product(a + half, b + half, len - half);
	return x + y;
}

void scalar_recursive(void *context)
{
	scalar_ctx_t *ctx = context;

	double sum = dot_product(ctx->a, ctx->b, ctx->len);
	printf("sum = %lf\n", sum);
}

double dot_product_omp(double *a, double *b, int len)
{
	if (len < 1000) {
		double sum = 0;
		for (int i = 0; i < len;i++) {
			sum += a[i] * b[i];
		}
		return sum;
	}
	
	int half = len / 2;
	
	double x, y;
	#pragma omp parallel
	{
		#pragma omp single nowait
		{
			#pragma omp task
			x = dot_product_omp(a, b, half);
			#pragma omp task
			y = dot_product_omp(a + half, b + half, len - half);
		}
	}
	return x + y;
}

void scalar_recursive_omp(void *context)
{
	scalar_ctx_t *ctx = context;

	double sum = dot_product_omp(ctx->a, ctx->b, ctx->len);
	printf("sum = %lf\n", sum);
}


int main(int argc, char **argv)
{
	size_t n = atoi(argv[1]);
	int nthreads = atoi(argv[2]);

	omp_set_num_threads(nthreads);
	scalar_ctx_t ctx = {
	    .len = n,
	};

	runner_pre(ctor, &ctx);

	runner_run(scalar_serial, &ctx, "serial");
	runner_run(scalar_mutex, &ctx, "mutex");
	runner_run(scalar_critical, &ctx, "critical");
	runner_run(scalar_atomic, &ctx, "atomic");
	runner_run(scalar_reduction, &ctx, "reduction");
	runner_run(scalar_recursive, &ctx, "recursive");
	runner_run(scalar_recursive_omp, &ctx, "recursive_omp");

	runner_post(dtor, &ctx);
	return 0;
}
