#pragma once
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

	std::cout << "Testing operator++" << std::endl;
	for( iter = deque.begin(), siter = sdeque.begin();
	iter != deque.end(), siter != sdeque.end(); ++iter, ++siter ) {
		// std::cout <<i++ <<' ' << *iter <<' ' <<*siter <<std::endl;
		massert( *iter == *siter );
	}

	std::deque<int>::reverse_iterator rsiter;
	std::reverse_iterator<my::CDeque<int>::CIterator> riter;
	std::cout << "Testing reverse operator++" << std::endl;
	for( riter = deque.rbegin(), rsiter = sdeque.rbegin();
	riter != deque.rend(), rsiter != sdeque.rend(); ++riter, ++rsiter ) {
		// std::cout <<i++ <<' ' << *riter <<' ' <<*rsiter <<std::endl;
		massert( *riter == *rsiter );
	}

	std::cout << "Testing operator--" << std::endl;
	for( iter = deque.end()-1, siter = sdeque.end()-1;
	iter != deque.begin(), siter != sdeque.begin(); --iter, --siter ) {
		// std::cout <<i++ <<' ' << *iter <<' ' <<*siter <<std::endl;
		massert( *iter == *siter );
	}

	std::cout << "Testing reverse operator--" << std::endl;
	for( riter = deque.rend(), rsiter = sdeque.rend();
	riter != deque.rbegin(), rsiter != sdeque.rbegin(); --riter, --rsiter ) {
		// std::cout <<i++ <<' ' << *riter <<' ' <<*rsiter <<std::endl;
		massert( *riter == *rsiter );
	}

	std::cout << "Testing operators >, >=, <, <=" << std::endl;
	my::CDeque<int>::CIterator iter2;
	for( iter = deque.begin(); iter != deque.end(); ++iter ) {
		for( iter2 = iter; iter2 != deque.end(); ++iter2 ) {
			//std::cout <<(iter == iter2) << (iter <= iter2) << (iter>iter2)<<(iter2 < iter) << (iter2 >= iter) <<std::endl;
			massert( (iter <= iter2) && !(iter > iter2) && !(iter2 < iter) && (iter2 >= iter) );
		}
	}

	std::cout << "Testing operators +, -" << std::endl;
	for( int j = 0; j < 100; j++ ) {
		int origin = rand();
		int offset = rand();
		my::CDeque<int>::CIterator test = deque.begin() + origin % deque.size() - offset%((origin) % deque.size()+1);
		std::deque<int>::iterator stest = sdeque.begin() + origin % sdeque.size() - offset%((origin) % deque.size()+1);
		massert( *test == *stest );
		//test = origin % deque.size() - offset%deque.size() + deque.begin();
		//stest = sdeque.begin() + origin % deque.size() - offset%deque.size();
		//massert( *test == *stest );
	}
	printf( "N           push_back_t   push_front_t  operator_br_t pop_back_t    pop_front_t  \n" );
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

		printf( "%9d %13f %13f %13f %13f %13f\n", i, push_back_t, push_front_t, operator_br_t, pop_back_t, pop_front_t );
	}
}
