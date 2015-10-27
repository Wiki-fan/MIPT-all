#pragma once
#include "stdafx.h"
#include "Deque.h"

void TestDeque()
{
	srand( static_cast<unsigned int>(time( NULL )) );
	clock_t t;
	my::CDeque<int> deque;
	printf( "N       push_back pop_back push_front pop_front operator[]\n" );
	double push_back_t, pop_back_t, push_front_t, pop_front_t, operator_br_t;
	for( int i = 1000; i < 1000000; i *= 10 ) {
		t = clock();
		for( int j = 0; j < i; ++j ) {
			deque.push_back( rand() );
		}
		push_back_t = (float)(clock() - t) / CLOCKS_PER_SEC;

		t = clock();
		for( int j = 0; j < i; ++j ) {
			deque.push_front( rand() );
		}
		push_front_t = (float)(clock() - t) / CLOCKS_PER_SEC;

		t = clock();
		for( int j = 0; j < i; ++j ) {
			int temp = deque[rand() % deque.size()];
		}
		operator_br_t = (float)(clock() - t) / CLOCKS_PER_SEC;

		t = clock();
		for( int j = 0; j < i; ++j ) {
			deque.pop_back();
		}
		pop_back_t = (float)(clock() - t) / CLOCKS_PER_SEC;

		t = clock();
		for( int j = 0; j < i; ++j ) {
			deque.pop_front();
		}
		pop_front_t = (float)(clock() - t) / CLOCKS_PER_SEC;

		printf( "%7d %10.5lf %10.5lf %10.5lf %10.5lf %10.5lf\n", i, push_back_t, push_front_t, operator_br_t, pop_back_t, pop_front_t );
	}
}