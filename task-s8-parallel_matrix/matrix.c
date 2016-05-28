#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <err.h>
#include <string.h>

#define NUMOFTHREADS 5

/** Check thread error */
#define CHTE(val, msg) \
{\
	int code;\
	if ((code = (val)) != 0) {\
    printf("%s: %s",msg, strerror(code));\
    exit(code);\
	}\
}

/* Struct describing work of each thread */
struct work
{
    int start, finish;
    int state;
    double max;
};

/* Matrix struct. Data is stored by rows. */
typedef struct
{
    double** m;
    int h;
    int w;
} Matrix;

Matrix matrix;
struct work works[NUMOFTHREADS];
pthread_t pthreads[NUMOFTHREADS];

pthread_mutex_t master_mutex;
/* Each thread will tell us that it's ready using this condvar */
pthread_cond_t ready_thread;

/** Read matrix from standard input stream */
void read_matrix( Matrix* m )
{
    int i, j;
    scanf( "%d%d", &m->w, &m->h );
    m->m = malloc( sizeof( double* ) * ( m->h ));

    for( i = 0; i < ( m->h ); ++i )
    {
        m->m[i] = malloc( sizeof( double ) * (m->w ));
        for( j = 0; j < ( m->w ); ++j )
        {
            scanf( "%lf", &(m->m[i][j] ));
        }
    }
}

/** Body of thread, performs work according to its parameter */
void* body( void* arg_raw )
{
    struct work* arg = (void*) arg_raw;
    int i, j;
    double max = matrix.m[arg->start][0];
    for( i = arg->start; i < arg->finish; ++i )
    {
        for( j = 0; j < matrix.w; ++j )
        {
            if( max < matrix.m[i][j] )
            {
                max = matrix.m[i][j];
            }
        }
    }
    CHTE(pthread_mutex_lock( &master_mutex ), "Lock failed");
    arg->max = max;
    arg->state = 1;
    CHTE(pthread_cond_signal( &ready_thread ), "Cond signal failed");
    CHTE(pthread_mutex_unlock( &master_mutex ), "Unlock failed");
    return 0;
}

int main( int argc, char** argv )
{
    int i;
    double max;
    int threads_running = 0;

    if (argc == 1)
    {
        printf("Reading from stdin\n");
    }
    else if( argc == 2 )
    {
        if( freopen( argv[1], "rt", stdin ) == NULL)
        {
            errx( 2, "Freopen error" );
        }
    }
    else
    {
        errx(1, "Too many parameters");
    }

    read_matrix( &matrix );

    CHTE(pthread_mutex_init( &master_mutex, NULL), "Mutex init failed");
    CHTE(pthread_cond_init( &ready_thread, NULL), "Cond init failed");

    for( i = 0; i < NUMOFTHREADS; ++i )
    {
        works[i].start = ( matrix.h * i ) / NUMOFTHREADS;
        works[i].finish = ( matrix.h * ( i + 1 )) / NUMOFTHREADS;
        works[i].state = 0;

        if( works[i].start != works[i].finish )
        {
            ++threads_running;
            printf( "Create thread %d: start = %d, finish = %d\n", i, works[i].start, works[i].finish );
            CHTE(pthread_create( &( pthreads[i] ), NULL, body, &( works[i] )), "Pthread_create failed");
        }
        else
        {
            printf( "Thread %d don't being created: no work for it\n", i );
        }
    }

    while( threads_running > 0 )
    {
        int j;
        CHTE(pthread_mutex_lock( &master_mutex ), "Lock failed");

        /* Check which thread ended execution */
        for( j = 0; j < NUMOFTHREADS; ++j )
        {
            if( works[j].state == 1 )
            {
                --threads_running;
                works[j].state = -1;
                printf( "stop thread %d: start = %d, finish = %d, max = %f\n", j, works[j].start, works[j].finish, works[j].max );
            }
        }

        /* Wait for another thread */
        if( threads_running )
        {
            CHTE(pthread_cond_wait( &ready_thread, &master_mutex ), "Cond wait failed");
        }
        CHTE(pthread_mutex_unlock( &master_mutex ), "Unlock failed");
    }

    CHTE(pthread_cond_destroy( &ready_thread ), "Cond destroy failed");
    CHTE(pthread_mutex_destroy( &master_mutex ), "Mutex destroy failed");

    max = works[0].max;
    for( i = 0; i < NUMOFTHREADS; ++i )
    {
        CHTE(pthread_join( pthreads[i], NULL), "Join error");
        if( max < works[i].max )
        {
            max = works[i].max;
        }
    }

    for( i = 0; i < matrix.h; ++i )
    {
        free( matrix.m[i] );
    }
    free( matrix.m );

    printf( "max = %f\n", max );
    return 0;
}
