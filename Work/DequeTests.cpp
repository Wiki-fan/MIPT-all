#include "stdafx.h"
#include "Deque.h"
#include <deque>

void TestDeque()
{
	srand( static_cast<unsigned int>(time( NULL )) );
	clock_t t;
	my::CDeque<int> deque;
	std::deque<int> sdeque;
	for( int j = 0; j < 100; ++j ) {
		int val = rand() - rand();
		deque.push_back( val );
		sdeque.push_back( val );
	}
	int i = 0;
	std::deque<int>::iterator siter;
	my::CDeque<int>::CIterator iter;
	for( iter = deque.begin(), siter = sdeque.begin();
	iter != deque.end(), siter != sdeque.end(); ++iter, ++siter ) {
		// std::cout <<i++ <<' ' << *iter <<' ' <<*siter <<std::endl;
		massert( *iter == *siter );
	}
	std::cout << deque[4] << deque[6] << deque[23];
	/*
	my::CDeque<int>::CIterator iter2;
	for( iter = deque.begin(); iter != deque.end(); ++iter ) {
		for( iter2 = iter; iter2 != deque.end(); ++iter2 ) {
			//std::cout <<(iter == iter2) << (iter <= iter2) << (iter>iter2)<<(iter2 < iter) << (iter2 >= iter) <<std::endl;
			massert( (iter <= iter2) && !(iter>iter2) && !(iter2 < iter) && (iter2 >= iter) );
		}
	}*/

	printf( "N       push_back_t, push_front_t, operator_br_t, pop_back_t, pop_front_t\n" );
	double push_back_t, pop_back_t, push_front_t, pop_front_t, operator_br_t;
	for( int i = 10; i <= 1000000; i *= 10 ) {

		t = clock();
		for( int j = 0; j < i; ++j ) {
			int val = rand() - rand();
			deque.push_back( val );
			sdeque.push_back( val );
		}
		push_back_t = (double)(clock() - t) / CLOCKS_PER_SEC;

		t = clock();
		for( int j = 0; j < i; ++j ) {
			int val = rand() - rand();
			deque.push_front( val );
			sdeque.push_front( val );
		}
		push_front_t = (double)(clock() - t) / CLOCKS_PER_SEC;

		t = clock();
		for( int j = 0; j < i; ++j ) {
			int val = rand();
			int a1 = deque[val % deque.size()];
			int a2 = sdeque[val % sdeque.size()];
			massert( a1 == a2 );
		}
		operator_br_t = (double)(clock() - t) / CLOCKS_PER_SEC;

		t = clock();
		for( int j = 0; j < i; ++j ) {
			int a1 = deque.pop_back();
			int a2 = sdeque.back();
			sdeque.pop_back();
			massert( a1 == a2 );
		}
		pop_back_t = (double)(clock() - t) / CLOCKS_PER_SEC;

		t = clock();
		for( int j = 0; j < i; ++j ) {
			int a1 = deque.pop_front();
			int a2 = sdeque.front();
			sdeque.pop_front();
			massert( a1 == a2 );
		}
		pop_front_t = (double)(clock() - t) / CLOCKS_PER_SEC;

		printf( "%7d %f %f %f %f %f\n", i, push_back_t, push_front_t, operator_br_t, pop_back_t, pop_front_t );
	}
}
