#pragma once
#include "../stdafx.h"
#include "../ImplicitTree.h"
#include "../ImplicitSplayTree.h"

int randBetween( const int l, const int r )
{
	return ( rand() % ( r - l + 1 )) + l;
}

class CNaiveImplicitTree : public IImplicitTree {
public:
	std::vector<int> v;
	// Вставка элемента со значением k на позицию i.
	void Insert( int i, int k )
	{
		v.insert( v.begin() + i, k );
	}

	// Присвоение элементу с индексом i значения k.
	void Assign( int i, int k )
	{
		v[i] = k;
	}

	// [l, r)
	int SumSubSegment( int l, int r )
	{
		return std::accumulate( v.begin() + l, v.begin() + r, 0, std::plus<int>());
	}

	bool NextPermutation( int l, int r )
	{
		return std::next_permutation( v.begin() + l, v.begin() + r );
	}
};

class CImplicitTreeTester {
public:
	void PrepareTests( int N, int rangeL, int rangeR )
	{
		int x1, x2;
		const int StartCapacity = int( N * 0.1 );
		// Начальное заполнение
		for( int i = 0; i < StartCapacity; ++i ) {
			opers.push_back( COperationDescr( COperationDescr::Operation::Insert,
			                                  randBetween( 0, std::max( 0, i - 1 )),
			                                  randBetween( rangeL, rangeR )));
		}
		int size = StartCapacity, sums = 0;
		for( int i = StartCapacity; i < N; ++i ) {
			COperationDescr::Operation oper = static_cast<typename COperationDescr::Operation>(rand() % 4);
			if( oper == COperationDescr::Operation::Insert ) {
				x1 = randBetween( 0, size++ - 1 );
				x2 = randBetween( rangeL, rangeR );
			} else if( oper == COperationDescr::Operation::Assign ) {
				x1 = randBetween( 0, size - 1 );
				x2 = randBetween( rangeL, rangeR );
			} else if( oper == COperationDescr::Operation::SumSubSegment
					|| oper == COperationDescr::Operation::NextPermutation ) {
				x1 = randBetween( 0, size - 2 );
				x2 = randBetween( x1 + 1, size - 1 );
			}
			opers.push_back( COperationDescr( oper, x1, x2 ));
		}

	}

	std::vector<int> PerformTests( IImplicitTree& tree )
	{
		std::vector<int> res;
		for( auto i = opers.begin(); i != opers.end(); ++i ) {
			switch( i->oper ) {
				case COperationDescr::Operation::Insert:
					tree.Insert( i->x1, i->x2 );
					break;
				case COperationDescr::Operation::Assign:
					tree.Assign( i->x1, i->x2 );
					break;
				case COperationDescr::Operation::SumSubSegment:
					res.push_back(
							tree.SumSubSegment( i->x1, i->x2 )
					);
					break;
				case COperationDescr::Operation::NextPermutation:
					tree.NextPermutation( i->x1, i->x2 );
					break;
				default:
					massert( false );
			}
		}
		return res;
	}

public:
	struct COperationDescr {

	public:
		enum Operation {
			Insert, Assign, SumSubSegment, NextPermutation
		};

		Operation oper;
		int x1, x2;

		COperationDescr( Operation _oper, int _x1, int _x2 ) : oper( _oper ), x1( _x1 ), x2( _x2 )
		{
		}
	};

	std::vector<COperationDescr> opers;
};

void TestImplicitTree()
{
	CImplicitTreeTester tester;
	CNaiveImplicitTree naive;
	CImplicitSplayTree splay;
	tester.PrepareTests( 100000, -100, 100 );
	clock_t t;
	t = clock();
	auto resNaive = tester.PerformTests( naive );
	double timeNaive = double( clock() - t ) / CLOCKS_PER_SEC;
	t = clock();
	auto resSplay = tester.PerformTests( splay );
	double timeSplay = double( clock() - t ) / CLOCKS_PER_SEC;
	printf( "Naive Splay\n%10f %10f\n", timeNaive, timeSplay );
	for( int i = 0; i < resNaive.size(); ++i ) {
		if( resNaive[i] != resSplay[i] ) {
			printf( "===>" );

			printf( "%d %d\n", resNaive[i], resSplay[i] );
		}
	}
	massert( resNaive == resSplay );

}

void ManualTest()
{
	std::vector<int> vec;
	CImplicitSplayTree splay;
	for( int i = 0; i < 10; ++i ) {
		vec.push_back( i );
		splay.Insert( 0, 9 - i );
		splay.print( splay.head );
		printf( "\n" );
	}
	while( std::next_permutation( &vec[1], &vec[7] )) {

		splay.NextPermutation( 1, 7 );
		printf( "\nans:" );
		splay.print( splay.head );
		printf( "\nvec:" );

		for( int i = 0; i < vec.size(); ++i ) {
			printf( "%d ", vec[i] );
		}
	}

}