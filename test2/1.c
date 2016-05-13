#include <pthread.h>
#include <stdlib.h>
#include <err.h>
#include "../common/utils.h"

#define CHN1(COMM, CODE, MESSAGE)\
if ((COMM) == -1) \
err(CODE, MESSAGE);
#define CHN0(COMM, CODE, MESSAGE)\
if ((COMM) != 0) \
err(CODE, MESSAGE);

pthread_t pthreads[5];
pthread_mutex_t master_mutex;
pthread_cond_t ready_thread;

#define ISOCOMM(PTR) (*PTR == '/' && *(PTR+1)=='*')
#define ISCLCOMM(PTR) (*PTR == '*' && *(PTR+1)=='/')

typedef struct {
	int numofcoms;
	int numopencoms;
	int start, end;
	int state;
} Work;

volatile Work* works;
volatile int threads_running = 0;
volatile char** globaltext;

void* work_body(void* args)
{
	Work* work = (Work*)args;
	int i;
	int ocomms = 0;
	int numofcoms = 0;
	for (i = work->start; i<work->end; ++i) {
		char* iter = globaltext[i];
		printf("Processing %d\n", i);
		while( *( iter + 1 ) != '\0' ) {
			if( ISOCOMM( iter )) {
				++ocomms;
			}
			if( ISCLCOMM( iter )) {
				numofcoms += ocomms;
				ocomms = 0;
			}
			++iter;
		}
		printf("processed %d\n", i);
	}
	CHN0(pthread_mutex_lock( &master_mutex ), 7, "Lock");
	work->numopencoms = ocomms;
	work->numofcoms = numofcoms;
	work->state = 1;
	CHN0(pthread_cond_signal( &ready_thread ), 8, "cond signal");
	CHN0(pthread_mutex_unlock( &master_mutex ), 9, "unlock");
}

void calc(char** text, int num)
{
	int i = 0;
	int numofstrings;
	works = malloc(sizeof(Work)*num);
	if (works == NULL) {
		err(3, "malloc failed");
	}

	globaltext = text;
	while (text[i] != NULL) {
		++i;
	}
	numofstrings = i;

	CHN0(pthread_mutex_init( &master_mutex, NULL), 4, "Mutex init failed");
	CHN0(pthread_cond_init( &ready_thread, NULL), 5, "Cond init failed");

	if (num > numofstrings)
		num = numofstrings-1;
	printf("%d", numofstrings);
	for( i = 0; i < num; ++i ) {
		works[i].start = ( numofstrings * i ) / num;
		works[i].end = ( numofstrings * ( i + 1 )) / num;
		works[i].state = 0;
		++threads_running;
		printf( "init thread %d, start = %d, end = %d\n", i, works[i].start, works[i].end );
		CHN0(pthread_create( &( pthreads[i] ), NULL, work_body, &( works[i] )), 6, "Pthread create failed");
	}

	while( threads_running > 0 ) {
		CHN0(pthread_mutex_lock( &master_mutex ), 7, "lock");

		for( i = 0; i < num; ++i ) {
			if( works[i].state == 1 ) {
				--threads_running;
				works[i].state = -1;
				printf( "stop thread %d, open %d comms, inside %d comms\n", i, works[i].numopencoms, works[i].numofcoms );
			}
		}

		if( threads_running ) {
			CHN0(pthread_cond_wait( &ready_thread, &master_mutex ), 11, "wait");
		}
		CHN0(pthread_mutex_unlock( &master_mutex ), 9, "unlock");
	}

	pthread_cond_destroy( &ready_thread );
	pthread_mutex_destroy( &master_mutex );

	for( i = 0; i < num; ++i ) {
		CHN0(pthread_join( pthreads[i], NULL), 12, "join");
	}

	int curnumopencoms = 0;
	int totalcomms = 0;
	for (i = 0; i<num; ++i) {
		totalcomms += works[i].numofcoms;
		curnumopencoms += works[i].numopencoms;
		if (i != num-1 && works[i+1].numofcoms > 0) {
			totalcomms += curnumopencoms;
			curnumopencoms = 0;
		}
	}
	printf("%d", totalcomms);

	free(works);
}

int main()
{
	char* text[] = {"/* */ /* /* ",
			"/**/ /* /*",
			"/**/",
			"/**/",
			"/**/ /**/",
			"/**/ /* /*",
	NULL};
	calc(text,17);
	return 0;
}