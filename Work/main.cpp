#include "stdafx.h"

#include "Sorts.h"
#include "Smth.h"
#include "Vector.h"
#include "Vector.cpp"
#include "Heap.h"
#include "Heap.cpp"
#include "ExternalSort.h"
#include "ChainHashTable.h"
#include "OpenAddressHashTable.h"
#include "Stack.h"
#include "Stack.cpp"
#include "Deque.h"
#include "Deque.cpp"

#include "OpenAddressHashTableTests.cpp"

/*int main()
{
	my::TestDeque();
	return 0;
}*/

void TestVector()
{
	srand( static_cast<unsigned int>(time( NULL )) );
	my::CVector<int> vec;
	for( int i = 0; i < 200; ++i ) {
		vec.push_back( rand() );
	}
	for( int i = 0; i < 200; ++i ) {
		vec.pop_back();
		std::cout << vec.max_size() <<' ';
	}
}

int main()
{
	TestOpenAddressHashTable();
	std::cin.get();
	return 0;
}