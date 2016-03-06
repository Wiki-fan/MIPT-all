#include "Tests.h"
#include "SafeString.h"

void TestS2()
{
	FILE* f;
	char* str;
	if (!(f = fopen("/dev/urandom", "r")) ) {
		printf("Unable to open file\n");
		exit(1);
	};
	str = malloc(2000*sizeof(char));
	gets_safe(f, &str);
	printf("%s", str);
}