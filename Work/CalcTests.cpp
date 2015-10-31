#include "stdafx.h"
#include "Calc.h"

void TestCalc()
{
	std::stringstream ss;
	ToReversePolish( std::cin, ss );
	double res = CalculateReversePolish( ss );
	std::cout << res;
}