#include "Tests.h"
#include <stdlib.h>
#include "BitwisePrint.h"

struct TestStruct {
	int i;
	float f;
};

void
TestS1()
{
    struct TestStruct           a = {0,0};
    for (; a.i < 1000; ++a.i, ++a.f) {
	BITWISE_PRINT(a);
	BITWISE_FORMATTED_PRINT(a);
    }
}
