#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_THREADS 30
#define THRESHOLD 1000

static int count = 0;

pthread_mutex_t lock;
pthread_cond_t cond;

void *inc(void *params)
{
	for (;;) {
		pthread_mutex_lock(&lock);
		if (count >= THRESHOLD) {
			pthread_mutex_unlock(&lock);
			break;
		}
		count++;
		if (count >= THRESHOLD) {
			pthread_cond_signal(&cond);
			pthread_mutex_unlock(&lock);
			break;
		}
		pthread_mutex_unlock(&lock);
	}
	return NULL;
}

void *waiter(void *params)
{
	pthread_mutex_lock(&lock);
	while (count < THRESHOLD) {
		pthread_cond_wait(&cond, &lock);
	}
	printf("count = %d\n", count);
	pthread_mutex_unlock(&lock);
	return NULL;
}

int main()
{
	pthread_mutex_init(&lock, NULL);
	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
	size_t stack_size;

	pthread_attr_getstacksize(&attr, &stack_size);
	printf("stack size = %zu\n", stack_size);
	pthread_attr_setstacksize(&attr, stack_size * 2);
	pthread_attr_getstacksize(&attr, &stack_size);
	printf("stack size = %zu\n", stack_size);

	for (long i = 0; i < NUM_THREADS - 1; i++) {
		pthread_create(threads + i, NULL, inc, (void *)i);
	}
	pthread_create(threads + NUM_THREADS - 1, NULL, waiter, NULL);


	for (long i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	return 0;
}
