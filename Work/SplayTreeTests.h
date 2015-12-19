#pragma once
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
			}
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
	/*template<>
	void GetRandKey<int>( int& key )
	{
		key = rand();
	}*/

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
	treeTester.PrepareTests( 1000 );
	CSplayTree SplayTree;
	CAVLTree AvlTree;
	CStdSetWrapper<int> StdSet;

	std::vector<bool> SplayRes( treeTester.TestTree( SplayTree ) );
	std::vector<bool> AvlRes( treeTester.TestTree( AvlTree ) );
	std::vector<bool> StdSetRes( treeTester.TestTree( StdSet ) );
	massert( SplayRes == StdSetRes /*&& AvlRes == StdSetRes */);
	std::cout << "Testing completed." << std::endl;
}
void Task3Old()
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
		std::cout << command << ' ' << key << std::endl;
		switch( command ) {
			case '?':
				massert( StdSet.count( key ) == AvlTree.Search( key ) && StdSet.count( key ) == SplayTree.Search( key ) );
				break;
			case '+':
				massert( StdSet.insert( key ).second == AvlTree.Insert( key ) && StdSet.insert( key ).second == SplayTree.Insert( key ) );
				break;
			case '-':
				massert( StdSet.erase( key ) == AvlTree.Remove( key ) && StdSet.erase( key ) == SplayTree.Remove( key ) );
				break;
			default:
				massert( false );
		}

	}
}