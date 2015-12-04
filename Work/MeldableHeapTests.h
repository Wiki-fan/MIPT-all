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
	std::vector<COperationDescr<type>> ret;
	ret.reserve( N );
	mysize heap1, heap2;
	type key;
	for( int i = 10; i < N - 10; ++i ) {
		key = rand() % 30000;
		ret.push_back( COperationDescr<type>( COperationDescr<type>::Operation( COperationDescr<type>::Operation::AddHeap ), 0, 0, key ));
	}
	for( int i = 10; i < N - 10; ++i ) {
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
	return ret;
}

void AddHeap( std::vector<IMeldableHeap<type, std::greater<type>>*>& heaps, const COperationDescr<type>& op )
{
	heaps.push_back( new CBinomialHeap<type, std::greater<type>>( op.key ));
}

void Insert( std::vector<IMeldableHeap<type, std::greater<type>>*>& heaps, const COperationDescr<type>& op )
{
	heaps[op.heap1]->Add( op.key );
}

type ExtractTop( std::vector<IMeldableHeap<type, std::greater<type>>*>& heaps, const COperationDescr<type>& op )
{
	return heaps[op.heap1]->ExtractTop();
}

void Meld( std::vector<IMeldableHeap<type, std::greater<type>>*>& heaps, const COperationDescr<type>& op )
{
	dynamic_cast<CBinomialHeap<type, std::greater<type>>*>(heaps[op.heap1])->
			Meld( dynamic_cast<CBinomialHeap<type, std::greater<type>>&>(*heaps[op.heap2]));
	heaps[op.heap2] = heaps.back();
	heaps.pop_back();
}
void TestMyHeaps()
{
	const int N = 1000; // Число куч
	std::vector<IMeldableHeap<type, std::greater<type>>*> LeftHeaps/*, BinHeaps*/, SkewHeaps;
	LeftHeaps.reserve( N );
	//BinHeaps.reserve( N );
	SkewHeaps.reserve( N );
	std::vector<COperationDescr<type>> TestSequence( GenerateTestSequence());

	for( auto i = TestSequence.begin(); i != TestSequence.end(); ++i ) {
		switch( i->oper ) {
			case COperationDescr<type>::Operation::AddHeap:
				//AddHeap(BinHeaps, *i);
				AddHeap( LeftHeaps, *i );
				AddHeap( SkewHeaps, *i );
				break;
			case COperationDescr<type>::Operation::Insert:
				//Insert(BinHeaps, *i);
				Insert( LeftHeaps, *i );
				Insert( SkewHeaps, *i );
				break;
			case COperationDescr<type>::Operation::ExtractTop:
				massert(/*ExtractTop(BinHeaps, *i) == */ExtractTop( LeftHeaps, *i ) == ExtractTop( SkewHeaps, *i ));
				break;
			case COperationDescr<type>::Operation::Meld:
				//Meld(BinHeaps, *i);
				Meld( LeftHeaps, *i );
				Meld( SkewHeaps, *i );
				break;
			default:
				massert( false );
		}
	}
}

void ManualTest()
{
	CLeftistHeap<int, std::greater<int>> lh;
	CSkewHeap<int, std::greater<int>> sh;
	CBinomialHeap<int, std::greater<int>> bh;
	for( int i = 10; i >= 1; --i ) {
		lh.Add( i );
		sh.Add( i );
		bh.Add( i );
	}
	for( int i = 0; i < 10; ++i ) {
		std::cout << lh.ExtractTop() << ' ' << sh.ExtractTop() << ' ' << bh.ExtractTop() << std::endl;
	}

}