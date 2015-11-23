#include "VectorTests.h"
#include "Vector.h"

void TestVector()
{
	const int N = 200;
	srand( static_cast<unsigned int>(time( NULL )) );
	my::CVector<int> vec;
	for( int i = 0; i < N; ++i ) {
		vec.push_back( rand() );
	}
	for( int i = 0; i < N; ++i ) {
		vec.pop_back();
		std::cout << vec.max_size() << ' ';
	}
}