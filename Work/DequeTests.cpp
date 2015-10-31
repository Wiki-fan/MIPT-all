#include "stdafx.h"
#include "Deque.h"

void TestDeque()
{
	srand( static_cast<unsigned int>(time( NULL )) );
	clock_t t;
	my::CDeque<int> deque;
	printf( "N       push_back pop_back push_front pop_front operator[]\n" );
	double push_back_t, pop_back_t, push_front_t, pop_front_t, operator_br_t;
	for( int i = 0; i < 1000; ++i ) {
		for( int j = 0; j < i; ++j ) {
			t = clock();
			deque.push_back( rand() );
			push_back_t = (double)(clock() - t);

			t = clock();
			deque.push_front( rand() );
			push_front_t = (double)(clock() - t) / CLOCKS_PER_SEC;

			t = clock();
			int temp = deque[rand() % deque.size()];
			operator_br_t = (double)(clock() - t) / CLOCKS_PER_SEC;

			t = clock();
			deque.pop_back();
			pop_back_t = (double)(clock() - t) / CLOCKS_PER_SEC;

			t = clock();
			deque.pop_front();
			pop_front_t = (double)(clock() - t) / CLOCKS_PER_SEC;
			
			printf( "%7d %10.10f %f %f %f %f\n", i, push_back_t, push_front_t, operator_br_t, pop_back_t, pop_front_t );
		}

		
	}
}