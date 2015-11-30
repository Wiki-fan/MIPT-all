#include "SingleLinkedListTests.h"
#include "SingleLinkedList.h"

int TestSingleLinkedList( int argc, const char* argv[] )
{
	CSingleLinkedListTester tester;

	// Тестируем all and list
	if( tester.TestAddAndList()) {
		std::cout << "add and list successful" << std::endl;
	} else {
		std::cout << "add and list failed" << std::endl;
	}

	// Тестируем reverse
	if( tester.TestReverse()) {
		std::cout << "reverse successful" << std::endl;
	} else {
		std::cout << "reverse failed" << std::endl;
	}
	return 0;
}

bool CSingleLinkedListTester::TestReverse()
{
	std::vector<int> testElem( N, rand());
	CSingleLinkedList lst;
	for( int i = 0; i < N; ++i ) {
		lst.AddFirst( testElem[i] );
	}
	lst.Reverse();
	CSingleLinkedList::CNode* node = lst.First();
	for( int i = N - 1; i >= 0; --i ) {
		if( node->Value != testElem[i] ) {
			return false;
		}
		node = node->Next();
	}
	return true;
}

bool CSingleLinkedListTester::TestAddAndList()
{
	std::vector<int> testElem( N, rand());
	CSingleLinkedList lst;
	for( int i = 0; i < N; ++i ) {
		lst.AddFirst( testElem[i] );
	}
	CSingleLinkedList::CNode* node = lst.First();
	for( int i = 0; i < N; ++i ) {
		if( node->Value != testElem[i] ) {
			return false;
		}
		node = node->Next();
	}
	return true;
}
