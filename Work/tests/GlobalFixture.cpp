#define BOOST_TEST_MODULE tests
#include <boost/test/unit_test.hpp>
#include <iostream>

struct GlobalFixture {
	GlobalFixture() { std::cout << "global setup\n"; }
	~GlobalFixture() { std::cout << "global teardown\n"; }
};

BOOST_GLOBAL_FIXTURE( GlobalFixture );