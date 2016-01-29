#include "../stdafx.h"
#include "../Calc.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( CalculatorTest )
{
	std::stringstream ss;
	ToReversePolish( std::cin, ss );
	double res = CalculateReversePolish( ss );
}