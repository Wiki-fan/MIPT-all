#pragma once
#include "../stdafx.h"
#include "../ImplicitTree.h"
#include "../ImplicitCartesianTree.h"
#include "../Smth.h"

// Наивная реализация дерева по неявному ключу на std::vector.
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

	// Сумма на полуинтервале [l, r).
	int SumSubSegment( int l, int r )
	{
		return std::accumulate( v.begin() + l, v.begin() + r, 0, std::plus<int>());
	}

	// Лексикографически следующая перестановка на полуинтервале [l, r).
	bool NextPermutation( int l, int r )
	{
		return std::next_permutation( v.begin() + l, v.begin() + r );
	}

	void Print()
	{
		printf( "Naive: " );
		for( int i = 0; i < v.size(); ++i ) {
			printf( "%d ", v[i] );
		}
		printf( "\n" );
	}

	std::vector<int>& Inorder()
	{
		return v;
	}
};

// Тестировщик для неявных деревьев с next_permutation.
class CImplicitTreeTester {
public:
	// Генерирует тестовую последовательность из N тестов, диапазон значений [rangeL, rangeR].
	void PrepareTests( int N, int rangeL, int rangeR )
	{
		opers.clear();
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
				x1 = randBetween( 0, size - 1 );
				x2 = randBetween( x1, size - 1 );
			} else {
				massert( false );
			}
			opers.push_back( COperationDescr( oper, x1, x2 ));
		}

	}

	// Прогоняет тесты без всяких проверок, показывает чистое время работы.
	void PerformTimeTests( IImplicitTree& tree )
	{
		for( auto i = opers.begin(); i != opers.end(); ++i ) {
			switch( i->oper ) {
				case COperationDescr::Operation::Insert:
					tree.Insert( i->x1, i->x2 );
					break;
				case COperationDescr::Operation::Assign:
					tree.Assign( i->x1, i->x2 );
					break;
				case COperationDescr::Operation::SumSubSegment:
					tree.SumSubSegment( i->x1, i->x2 );
					break;
				case COperationDescr::Operation::NextPermutation:
					tree.NextPermutation( i->x1, i->x2 );
					break;
				default:
					massert( false );
			}
		}
	}

	// Прогоняет тесты и сравнивает результаты.
	void PerformProofOfWorkTests()
	{
		CNaiveImplicitTree naive;
		CImplicitCartesianTree cartes;
		for( auto i = opers.begin(); i != opers.end(); ++i ) {
			switch( i->oper ) {
				case COperationDescr::Operation::Insert:
					cartes.Insert( i->x1, i->x2 );
					naive.Insert( i->x1, i->x2 );
					break;
				case COperationDescr::Operation::Assign:
					cartes.Assign( i->x1, i->x2 );
					naive.Assign( i->x1, i->x2 );
					break;
				case COperationDescr::Operation::SumSubSegment:
					massert( cartes.SumSubSegment( i->x1, i->x2 ) == naive.SumSubSegment( i->x1, i->x2 ));
					break;
				case COperationDescr::Operation::NextPermutation:
					massert( cartes.NextPermutation( i->x1, i->x2 ) == naive.NextPermutation( i->x1, i->x2 ));
					break;
				default:
					massert( false );
			}
			//cartes.Print();
			//naive.Print();
			massert( cartes.Inorder() == naive.Inorder());
		}
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
	int n, rangeL, rangeR;
	std::cin >> n >>rangeR >> rangeR;
	tester.PrepareTests( n, rangeL, rangeR );
	// ProofOfWorkTests сравнивают весь получившийся массив, принудительно пушат все отложенные модификации, работают долго.
	// Не стоит включать их на большом наборе тестов.
	if (n <=100000) {
		tester.PerformProofOfWorkTests();
		printf( "ProofOfWorkTests successful\n" );
	}

	CImplicitCartesianTree cartes;
	CNaiveImplicitTree naive;
	clock_t t;
	t = clock();
	tester.PerformTimeTests( cartes );
	double timeCartesian = double( clock() - t ) / CLOCKS_PER_SEC;
	printf( "Cartesian %10f\n", timeCartesian );
	t = clock();
	tester.PerformTimeTests( naive );
	double timeNaive = double( clock() - t ) / CLOCKS_PER_SEC;
	printf( "Naive     %10f\n", timeNaive);

}

void ManualTest()
{
	CImplicitCartesianTree cartes;
	CNaiveImplicitTree naive;
	const int N = 10;
	for( int i = 0; i < N; ++i ) {
		naive.Insert( 0, N - i - 1 );
		cartes.Insert( 0, N - i - 1 );
	}
	const int l = 1, r = 9;
	while( naive.NextPermutation( l, r )) {
		cartes.NextPermutation( l, r );
		naive.Print();
		cartes.Print();
		if( naive.Inorder() != cartes.Inorder()) {
			printf( "===========\n" );
			massert( false );
		};

		printf( "\n" );
	}

}