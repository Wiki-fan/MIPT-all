/*
 * multi-threaded matrix maximum search
 * use 5 threads
 */
#include <pthread.h>

#define N 5
struct Work
{
    int start, finish;
    int state;
    double max;
} works[N];

pthread_mutex_t master_mutex;
pthread_cond_t ready_thread;
double max;
void* body(void* param)
{

}

void read_matrix(double **mtr, int* x, int *y)
{
    int i, j;
    double max = mtr[0][0];
    for (i=0; i<*x; ++i)
        for (j = 0; j<*y; ++j)
            if (max < mtr[i][j])
                max = mtr[i][j];
}

void read_matrix_threaded(double **mtr, int* x, int *y)
{
    int i, j;
    pthread_t threads[N];
    double max = mtr[0][0];

    for (i = 0; i<N; ++i)
    {
        works[i].start = *x/5*i;
        works[i].finish = *x/5*(i+1)+(i == 4?*x%5:0);
        works[i].state = 0;
    }

    pthread_cond_init(&ready_thread, NULL);
    pthread_mutex_init(&master_mutex, NULL);

    for (i = 0; i<N; ++i)
    {
        pthread_create(&(threads[i]), NULL, body, &(works[i]));
    }

    for (int i = 0; i<N;)
    {
        pthread_mutex_lock(&master_mutex);
        pthread_cond_wait(&ready_thread, &master_mutex);
        i = 0;
        for (j = 0; j<5; ++j)
        {
            if (works[i].state == 1)
            {
                ++i;
                if (max <works[i].max)
                    max = works[j].max;
            }
        }
    }
    for (i=0; i<*x; ++i)
        for (j = 0; j<*y; ++j)
            if (max < mtr[i][j])
                max = mtr[i][j];
}

/* короткое задаание -- написать body */
