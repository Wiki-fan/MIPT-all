#include "../stdafx.h"
#include "../Vector.h"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE( VectorTest )
{
	const int N = 200;
	srand( static_cast<unsigned int>(time( NULL )));
	my::CVector<int> vec;
	for( int i = 0; i < N; ++i ) {
		vec.push_back( rand());
	}
	for( int i = 0; i < N; ++i ) {
		vec.pop_back();
		std::cout << vec.max_size() << ' ';
	}
}