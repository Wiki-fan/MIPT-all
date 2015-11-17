#pragma once
#include "stdafx.h"
#include "MeldableHeap.h"
#include "BinomialHeap.h"

template<typename T>
struct COperationDescr {

public:
	enum Operation {
		AddHeap, Insert, ExtractTop, Meld
	};

	COperationDescr( Operation _oper, mysize heap1, mysize heap2, T key )
	{
	}

	Operation oper;
	mysize heap1, heap2;
	T key;
};

std::vector<COperationDescr<type>> GenerateTestSequence()
{
	const int N = 1000; // Число тестов
	std::vector<COperationDescr<type>> ret;
	ret.reserve( N );
	mysize heap1, heap2;
	type key;
	for( int i = 0; i < N; ++i ) {
		int oper = rand() % 4;
		switch( oper ) {
			case COperationDescr<type>::Operation::AddHeap:
				key = rand();
				ret.push_back( COperationDescr<type>( COperationDescr<type>::Operation( oper ), 0, 0, key ));
				break;
			case COperationDescr<type>::Operation::Insert:
				key = rand();
				heap1 = rand() % ret.size();
				ret.push_back( COperationDescr<type>( COperationDescr<type>::Operation( oper ), heap1, 0, key ));
				break;
			case COperationDescr<type>::Operation::ExtractTop:
				heap2 = rand() % ret.size();
				ret.push_back( COperationDescr<type>( COperationDescr<type>::Operation( oper ), heap2, 0, 0 ));
				break;
			case COperationDescr<type>::Operation::Meld:
				key = rand();
				heap1 = rand() % ret.size();
				heap2 = rand() % ret.size();
				ret.push_back( COperationDescr<type>( COperationDescr<type>::Operation( oper ), heap1, heap2, key ));
				break;
			default:
				massert( false );
		}

	}
}

void TestMyHeaps()
{
	const int N = 1000; // Число куч
	std::vector<IMeldableHeap<type, std::greater<type>>*> LeftHeaps, BinHeaps;
	LeftHeaps.reserve( N );
	BinHeaps.reserve( N );
	std::vector<COperationDescr<type>> TestSequence( GenerateTestSequence());

	for( auto i = TestSequence.begin(); i != TestSequence.end(); ++i ) {
		switch( i->oper ) {
			case COperationDescr<type>::Operation::AddHeap:
				BinHeaps.push_back( new CBinomialHeap<type, std::greater<type>>( i->key ));
				break;
			case COperationDescr<type>::Operation::Insert:
				BinHeaps[i->heap1]->Add( i->key );
				break;
			case COperationDescr<type>::Operation::ExtractTop:
				BinHeaps[i->heap1]->ExtractTop();
				break;
			case COperationDescr<type>::Operation::Meld:
				BinHeaps[i->heap1] = Meld( *BinHeaps[i->heap1], *BinHeaps[i->heap2] );
				BinHeaps[i->heap2] = BinHeaps.back();
				BinHeaps.pop_back();
				break;
			default:
				massert( false );
		}
	}

	for( int i = 0; i < N; ++i ) {

	}
}