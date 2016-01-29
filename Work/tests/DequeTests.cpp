#include "../stdafx.h"
#include "../Deque.h"
#include <deque>
#include <boost/test/unit_test.hpp>

struct Fixture {
	Fixture()
	{
		srand( static_cast<unsigned int>(time( NULL )));
		for( int j = 0; j < 100; ++j ) {
			int val = rand() - rand();
			deque.push_back( val );
			sdeque.push_back( val );
		}
	}
	~Fixture()
	{
	}

	my::CDeque<int> deque;
	std::deque<int> sdeque;
	std::deque<int>::iterator siter;
	my::CDeque<int>::CIterator iter;
	my::CDeque<int>::CIterator iter2;
	std::deque<int>::reverse_iterator rsiter;
	std::reverse_iterator<my::CDeque<int>::CIterator> riter;
	int i;
};

BOOST_FIXTURE_TEST_SUITE( DequeTestSuite, Fixture )

BOOST_AUTO_TEST_CASE( PopulateTest )
{
	for( int j = 0; j < 100; ++j ) {
		BOOST_CHECK( deque.size() == sdeque.size());
		int val = rand() - rand();
		deque.push_back( val );
		sdeque.push_back( val );
	}
	BOOST_CHECK( deque.size() == sdeque.size());
}

BOOST_AUTO_TEST_CASE( OperatorPlusPlusTest )
{
	for( iter = deque.begin(), siter = sdeque.begin();
	     iter != deque.end(), siter != sdeque.end(); ++iter, ++siter ) {
		// std::cout <<i++ <<' ' << *iter <<' ' <<*siter <<std::endl;
		BOOST_CHECK( *iter == *siter );
	}
}

BOOST_AUTO_TEST_CASE( ReverseOperatorPlusPlusTest )
{
	for( riter = deque.rbegin(), rsiter = sdeque.rbegin();
	     riter != deque.rend(), rsiter != sdeque.rend();
	     ++riter, ++rsiter ) {
		// std::cout <<i++ <<' ' << *riter <<' ' <<*rsiter <<std::endl;
		BOOST_CHECK( *riter == *rsiter );
	}
}

BOOST_AUTO_TEST_CASE( OperatorMinusMinusTest )
{
	for( iter = deque.end() - 1, siter = sdeque.end() - 1;
	     iter != deque.begin(), siter != sdeque.begin(); --iter, --siter ) {
		// std::cout <<i++ <<' ' << *iter <<' ' <<*siter <<std::endl;
		BOOST_CHECK( *iter == *siter );
	}
}

BOOST_AUTO_TEST_CASE( ReverseOperatorMinusMinusTest )
{
	for( riter = deque.rend() - 1, rsiter = sdeque.rend() - 1;
	     riter != deque.rbegin(), rsiter != sdeque.rbegin(); --riter, --rsiter ) {
		// std::cout <<i++ <<' ' << *riter <<' ' <<*rsiter <<std::endl;
		BOOST_CHECK( *riter == *rsiter );
	}
}

BOOST_AUTO_TEST_CASE( CompareOperatorsTest )
{
	for( iter = deque.begin(); iter != deque.end(); ++iter ) {
		for( iter2 = iter; iter2 != deque.end(); ++iter2 ) {
			//std::cout <<(iter == iter2) << (iter <= iter2) << (iter>iter2)<<(iter2 < iter) << (iter2 >= iter) <<std::endl;
			BOOST_CHECK(( iter <= iter2 ) && !( iter > iter2 ) && !( iter2 < iter ) && ( iter2 >= iter ));
		}
	}
}

BOOST_AUTO_TEST_CASE( BinaryMinusOperatorTest )
{
	iter2 = deque.begin();
	for( i = 0, iter = deque.begin(); iter != deque.end(); ++iter, ++i ) {
		BOOST_CHECK(( iter - iter2 ) == i && -( iter2 - iter ) == i );
		//std::cout << iter2 - iter <<' ' << iter - iter2 <<std::endl;
	}
}

BOOST_AUTO_TEST_CASE( PlusMinusOperatorsTest )
{
	for( int j = 0; j < 1000; j++ ) {
		int origin = rand();
		int offset = rand();
		my::CDeque<int>::CIterator test = deque.begin() + origin % deque.size() - offset % (( origin ) % deque.size() + 1 );
		std::deque<int>::iterator stest = sdeque.begin() + origin % sdeque.size() - offset % (( origin ) % deque.size() + 1 );
		BOOST_CHECK( *test == *stest );
		//test = origin % deque.size() - offset%deque.size() + deque.begin();
		//stest = sdeque.begin() + origin % deque.size() - offset%deque.size();
		//BOOST_CHECK( *test == *stest );
	}
}

BOOST_AUTO_TEST_CASE( DequeLinearityTest )
{
	clock_t t;
	std::cout << "Testing linearity..." << std::endl;
	printf( "N           push_back_t   push_front_t  operator_br_t pop_back_t    pop_front_t  \n" );
	double push_back_t, pop_back_t, push_front_t, pop_front_t, operator_br_t;
	for( i = 10; i <= 10000; i *= 10 ) {

		t = clock();
		for( int j = 0; j < i; ++j ) {
			int val = rand() - rand();
			deque.push_back( val );
			sdeque.push_back( val );
		}
		push_back_t = double( clock() - t ) / CLOCKS_PER_SEC;

		t = clock();
		for( int j = 0; j < i; ++j ) {
			int val = rand() - rand();
			deque.push_front( val );
			sdeque.push_front( val );
		}
		push_front_t = (double) ( clock() - t ) / CLOCKS_PER_SEC;

		t = clock();
		for( int j = 0; j < i; ++j ) {
			int val = rand();
			int a1 = deque[val % deque.size()];
			int a2 = sdeque[val % sdeque.size()];
			BOOST_CHECK( a1 == a2 );
		}
		operator_br_t = (double) ( clock() - t ) / CLOCKS_PER_SEC;

		t = clock();
		for( int j = 0; j < i; ++j ) {
			int a1 = deque.pop_back();
			int a2 = sdeque.back();
			sdeque.pop_back();
			BOOST_CHECK( a1 == a2 );
		}
		pop_back_t = (double) ( clock() - t ) / CLOCKS_PER_SEC;

		t = clock();
		for( int j = 0; j < i; ++j ) {
			int a1 = deque.pop_front();
			int a2 = sdeque.front();
			sdeque.pop_front();
			BOOST_CHECK( a1 == a2 );
		}
		pop_front_t = (double) ( clock() - t ) / CLOCKS_PER_SEC;

		printf( "%9d %13f %13f %13f %13f %13f\n", i, push_back_t, push_front_t, operator_br_t, pop_back_t, pop_front_t );
	}
}
BOOST_AUTO_TEST_SUITE_END()

void TestDeque()
{

	// �������� ��������������� CConstIterator.
	//const my::CDeque<int> cdeque = deque;
	//my::CDeque<int>::CConstIterator citer;
	//citer = (deque.cbegin());
	//citer++; 
	// ������ � �� ����� ��������� ���������. ��������!
	// (*citer) = 13;


}
