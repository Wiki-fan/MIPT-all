#include "stdafx.h"
#include "Vector.h"

void TestVector()
{
	srand( static_cast<unsigned int>(time( NULL )) );
	my::CVector<int> vec;
	for( int i = 0; i < 200; ++i ) {
		vec.push_back( rand() );
	}
	for( int i = 0; i < 200; ++i ) {
		vec.pop_back();
		std::cout << vec.max_size() << ' ';
	}
}