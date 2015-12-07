#pragma once
#include "stdafx.h"
#include "MeldableHeap.h"
#include "BinomialHeap.h"
#include "LeftistHeap.h"
#include "SkewHeap.h"

template<typename T>
struct COperationDescr {

public:
	enum Operation {
		AddHeap, Insert, ExtractTop, Meld
	};

	COperationDescr( Operation _oper, mysize _heap1, mysize _heap2, T _key )
			: oper( _oper ), heap1( _heap1 ), heap2( _heap2 ), key( _key )
	{
	}

	Operation oper;
	mysize heap1, heap2;
	T key;
};

std::vector<COperationDescr<type>> GenerateTestSequence()
{
	const int N = 10000000; // Число тестов
	const int AddN = 100000;
	std::vector<COperationDescr<type>> ret;
	std::vector<int> numberOfElementsInHeaps;
	//ret.reserve( N );
	mysize heap1, heap2;
	type key;
	int heapCount = 0;
	for( int i = 0; i < AddN; ++i ) {
		key = rand();
		ret.push_back( COperationDescr<type>( COperationDescr<type>::Operation( COperationDescr<type>::Operation::AddHeap ), 0, 0, key ));
		numberOfElementsInHeaps.push_back( 0 );
		++heapCount;
	}
	for( int i = AddN; i < N - AddN; ++i ) {
		int oper = rand() % 4;
		switch( oper ) {
			case COperationDescr<type>::Operation::AddHeap:
				key = rand();
				ret.push_back( COperationDescr<type>( COperationDescr<type>::Operation( oper ), 0, 0, key ));
				numberOfElementsInHeaps.push_back( 0 );
				++heapCount;
				break;
			case COperationDescr<type>::Operation::Insert:
				key = rand();
				heap1 = rand() % heapCount;
				ret.push_back( COperationDescr<type>( COperationDescr<type>::Operation( oper ), heap1, 0, key ));
				numberOfElementsInHeaps[heap1]++;
				break;
			case COperationDescr<type>::Operation::ExtractTop:
				heap1 = rand() % heapCount;
				if( numberOfElementsInHeaps[heap1] == 0 ) {
					continue;
				}
				ret.push_back( COperationDescr<type>( COperationDescr<type>::Operation( oper ), heap1, 0, 0 ));
				--numberOfElementsInHeaps[heap1];
				break;
			case COperationDescr<type>::Operation::Meld:
				key = rand();
				heap1 = rand() % heapCount;
				heap2 = rand() % heapCount;
				numberOfElementsInHeaps[heap1] += numberOfElementsInHeaps[heap2];
				numberOfElementsInHeaps[heap2] = numberOfElementsInHeaps.back();
				ret.push_back( COperationDescr<type>( COperationDescr<type>::Operation( oper ), heap1, heap2, key ));
				numberOfElementsInHeaps.pop_back();
				--heapCount;
				break;
			default:
				massert( false );
		}

	}
	return ret;
}

typedef IMeldableHeap<type, std::greater<type>> TestableHeap;
typedef std::vector<TestableHeap*> TestableHeaps;

void Insert( TestableHeaps& heaps, const COperationDescr<type>& op )
{
	heaps[op.heap1]->Add( op.key );
}

type ExtractTop( TestableHeaps& heaps, const COperationDescr<type>& op )
{
	return heaps[op.heap1]->ExtractTop();
}

void Meld( std::vector<IMeldableHeap<type, std::greater<type>>*>& heaps, const COperationDescr<type>& op )
{
	heaps[op.heap1]->Meld( *heaps[op.heap2] );
	heaps[op.heap2] = heaps.back();
	heaps.pop_back();
}

enum HeapType {
	Binomial, Leftist, Skew
};

std::vector<type> TestMyHeap( TestableHeaps& heaps, int heapType, std::vector<COperationDescr<type>>& TestSequence )
{
	int cnt = 0;
	std::vector<type> ret;
	for( auto i = TestSequence.begin(); i != TestSequence.end(); ++i, ++cnt ) {
		switch( i->oper ) {
			case COperationDescr<type>::Operation::AddHeap:
				switch( heapType ) {
					case HeapType::Binomial:
						heaps.push_back( new CBinomialHeap<type, std::greater<type>>( i->key ));
						break;
					case HeapType::Leftist:
						heaps.push_back( new CLeftistHeap<type, std::greater<type>>( i->key ));
						break;
					case HeapType::Skew:
						heaps.push_back( new CSkewHeap<type, std::greater<type>>( i->key ));
						break;
					default:
						massert( false );
				}
				break;
			case COperationDescr<type>::Operation::Insert:
				Insert( heaps, *i );
				break;
			case COperationDescr<type>::Operation::ExtractTop:
				ret.push_back( ExtractTop( heaps, *i ));
				//std::cout << ret << std::endl;
				//massert( left == skew && left == bin );
				break;
			case COperationDescr<type>::Operation::Meld:
				Meld( heaps, *i );
				break;
			default:
				massert( false );
		}
	}
	for( auto i = heaps.begin(); i != heaps.end(); ++i ) {
		delete *i;
	}
	return ret;
}
void TestMyHeaps()
{
	TestableHeaps LeftHeaps, SkewHeaps, BinHeaps;
	std::vector<COperationDescr<type>> TestSequence( GenerateTestSequence());

	//type left, skew, bin;
	clock_t t;
	t = clock();
	std::vector<type> BinomialRet = TestMyHeap( BinHeaps, HeapType::Binomial, TestSequence );
	double BinTime = double( clock() - t ) / CLOCKS_PER_SEC;

	t = clock();
	std::vector<type> LeftistRet = TestMyHeap( LeftHeaps, HeapType::Leftist, TestSequence );
	double LeftTime = double( clock() - t ) / CLOCKS_PER_SEC;

	t = clock();
	std::vector<type> SkewRet = TestMyHeap( SkewHeaps, HeapType::Skew, TestSequence );
	double SkewTime = double( clock() - t ) / CLOCKS_PER_SEC;

	massert( BinomialRet == SkewRet && BinomialRet == LeftistRet );
	printf( "%13f %13f %13f\n", BinTime, LeftTime, SkewTime );

	std::cout << "Testing completed." << std::endl;
}

void ManualTest()
{
	CLeftistHeap<int, std::greater<int>> lh;
	CSkewHeap<int, std::greater<int>> sh;
	CBinomialHeap<int, std::greater<int>> bh;
	for( int i = 10; i >= 1; --i ) {
		int x = rand() % 32;
		lh.Add( x );
		sh.Add( x );
		bh.Add( x );
	}
	for( int i = 0; i < 10; ++i ) {
		std::cout << lh.ExtractTop() << ' ' << sh.ExtractTop() << ' ' << bh.ExtractTop() << std::endl;
	}

}