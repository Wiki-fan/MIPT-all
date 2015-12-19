﻿#pragma once
#include "SplayTree.h"
#include "AVLTree.h"

template<typename T>
class TreeTester {
public:
	void PrepareTests( int N )
	{
		T key;
		for( int i = 0; i < N; ++i ) {
			GetRandKey<T>( key );
			COperationDescr<T>::Operation oper = static_cast<typename COperationDescr<T>::Operation>(rand() % 2);
			opers.push_back( COperationDescr<T>( oper, key ) );
		}
	}

	std::vector<bool> TestTree( ITree<T>& tree )
	{
		std::vector<bool> res;
		int cnt = 0;
		for( auto i = opers.begin(); i != opers.end(); ++i ) {
			switch( i->oper ) {
				case COperationDescr<T>::Insert:
					res.push_back( tree.Insert( i->key ) );
					break;
				case COperationDescr<T>::Search:
					res.push_back( tree.Search( i->key ) );
					break;
				case COperationDescr<T>::Remove:
					res.push_back( tree.Remove( i->key ) );
					break;
				default:
					massert( false );
			}
			++cnt;
		}
		return res;
	}


private:
	template<typename T>
	struct COperationDescr {

	public:
		enum Operation {
			Insert, Search, Remove
		};
		COperationDescr() = delete;
		COperationDescr( Operation& _oper, T& _key )
			: oper( _oper ), key( _key )
		{
		}

		Operation oper;
		T key;
	};

	template<typename T>
	void GetRandKey( T& key ) {}
	template<>
	void GetRandKey<int>( int& key )
	{
		key = abs( rand() );
	}

	std::vector<COperationDescr<T>> opers;
};

template<typename T>
class CStdSetWrapper : public ITree<T> {
public:
	std::set<int> StdSet;

	// Вставка элемента с ключом key.
	virtual bool Insert( const T& key ) override
	{
		return StdSet.insert( key ).second;
	}
	// Поиск элемента с ключом key.
	virtual bool Search( const T& key ) const override
	{
		return 	StdSet.count( key );
	}
	// Удаление элемента с ключом key.
	virtual bool Remove( const T& key ) override
	{
		return StdSet.erase( key );
	}
};

void Task3()
{
	TreeTester<int> treeTester;
	treeTester.PrepareTests( 1000000 );

	CSplayTree SplayTree;
	CAVLTree AvlTree;
	CStdSetWrapper<int> StdSet;
	clock_t t;

	t = clock();
	std::vector<bool> SplayRes( treeTester.TestTree( SplayTree ) );
	double SplayTime = double( clock() - t ) / CLOCKS_PER_SEC;

	t = clock();
	std::vector<bool> AvlRes( treeTester.TestTree( AvlTree ) );
	double AvlTime = double( clock() - t ) / CLOCKS_PER_SEC;

	t = clock();
	std::vector<bool> StdSetRes( treeTester.TestTree( StdSet ) );
	double StdSetTime = double( clock() - t ) / CLOCKS_PER_SEC;

	massert( SplayRes == StdSetRes && AvlRes == StdSetRes );
	std::cout << "Testing completed." << std::endl;
	std::cout << "Splay, AVL, std::set" << std::endl;
	printf( "%13f %13f %13f\n", SplayTime, AvlTime, StdSetTime );
}

void AutoTestSplayTree()
{
	CSplayTree SplayTree;
	CAVLTree AvlTree;
	std::set<int> StdSet;

	while( true ) {
		char command = rand() % 3;
		switch( command ) {
			case 0:
				command = '+';
				break;
			case 1:
				command = '-';
				break;
			case 2:
				command = '?';
				break;
			default:
				massert( false );
		}

		int key = rand();
		bool SetRes, SplayRes, AvlRes;
		//std::cout << command << ' ' << key << std::endl;
		switch( command ) {
			case '?':
				SetRes = StdSet.count( key );
				SplayRes = SplayTree.Search( key );
				AvlRes = AvlTree.Search( key );
				massert( /*SetRes == AvlRes  &&*/ SetRes == SplayRes );
				break;
			case '+':
				SetRes = StdSet.insert( key ).second;
				SplayRes = SplayTree.Insert( key );
				AvlRes = AvlTree.Insert( key );
				massert(/* SetRes == AvlRes  &&*/ SetRes == SplayRes );
				break;
			case '-':
				SetRes = StdSet.erase( key );
				SplayRes = SplayTree.Remove( key );
				AvlRes = AvlTree.Remove( key );
				massert( /*SetRes == AvlRes  && */SetRes == SplayRes );
				break;
			default:
				massert( false );
		}

	}
}

void ManualTestSplayTree()
{
	CSplayTree SplayTree;
	char command;
	int key;
	while( std::cin >> command >> key ) {
		//std::cout << command << ' ' << key << std::endl;
		switch( command ) {
			case '?':
				std::cout << (SplayTree.Search( key ) ? "OK" : "FAIL");
				break;
			case '+':
				std::cout << (SplayTree.Insert( key ) ? "OK" : "FAIL");
				break;
			case '-':
				std::cout << (SplayTree.Remove( key ) ? "OK" : "FAIL");
				break;
			default:
				massert( false );
		}
		std::cout << std::endl;
	}
}