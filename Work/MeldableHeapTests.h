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
	const int N = 1000; // Число тестов
	const int AddN = 100;
	std::vector<COperationDescr<type>> ret;
	std::vector<int> numberOfElementsInHeaps;
	//ret.reserve( N );
	mysize heap1, heap2;
	type key;
	int heapCount = 0;
	for( int i = 0; i < AddN; ++i ) {
		key = rand() % 30000;
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
				numberOfElementsInHeaps[heap1] += numberOfElementsInHeaps[heap2];
				heap2 = rand() % heapCount;
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

void AddHeaps( TestableHeaps& binHeaps,
               TestableHeaps& leftHeaps,
               TestableHeaps& skewHeaps,
               const COperationDescr<type>& op )
{
	binHeaps.push_back( new CBinomialHeap<type, std::greater<type>>( op.key ));
	leftHeaps.push_back( new CLeftistHeap<type, std::greater<type>>( op.key ));
	skewHeaps.push_back( new CSkewHeap<type, std::greater<type>>( op.key ));
	/*leftHeaps.push_back( new CLeftistHeap<type, std::greater<type>>( op.key ));
	SkewHeaps.push_back( new CSkewHeap<type, std::greater<type>>( i->key ));*/
}

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

void TestMyHeaps()
{
	const int N = 1000; // Число куч
	TestableHeaps LeftHeaps, SkewHeaps, BinHeaps;
	LeftHeaps.reserve( N );
	BinHeaps.reserve( N );
	SkewHeaps.reserve( N );
	std::vector<COperationDescr<type>> TestSequence( GenerateTestSequence());

	type left, skew, bin;
	int cnt = 0;
	for( auto i = TestSequence.begin(); i != TestSequence.end(); ++i, ++cnt ) {
		switch( i->oper ) {
			case COperationDescr<type>::Operation::AddHeap:
				//AddHeaps( LeftHeaps, SkewHeaps, BinHeaps, *i );
				LeftHeaps.push_back( new CLeftistHeap<type, std::greater<type>>( i->key ));
				SkewHeaps.push_back( new CSkewHeap<type, std::greater<type>>( i->key ));
				BinHeaps.push_back( new CBinomialHeap<type, std::greater<type>>( i->key ));
				break;
			case COperationDescr<type>::Operation::Insert:
				Insert( BinHeaps, *i );
				Insert( LeftHeaps, *i );
				Insert( SkewHeaps, *i );
				break;
			case COperationDescr<type>::Operation::ExtractTop:
				left = ExtractTop( LeftHeaps, *i );
				skew = ExtractTop( SkewHeaps, *i );
				bin = ExtractTop( BinHeaps, *i );
				std::cout << left << ' ' << skew << ' ' << bin << std::endl;
				//massert( left == skew == bin);
				massert( left == skew && left == bin );
				break;
			case COperationDescr<type>::Operation::Meld:
				Meld( BinHeaps, *i );
				Meld( LeftHeaps, *i );
				Meld( SkewHeaps, *i );
				break;
			default:
				massert( false );
		}
	}
	for( auto i = LeftHeaps.begin(); i != LeftHeaps.end(); ++i ) {
		delete *i;
	}
	for( auto i = SkewHeaps.begin(); i != SkewHeaps.end(); ++i ) {
		delete *i;
	}
	for( auto i = BinHeaps.begin(); i != BinHeaps.end(); ++i ) {
		delete *i;
	}
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