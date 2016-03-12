#include "safe_string.h"

int main()
{
	FILE* f;
	char* str;
	if (!(f = fopen("/dev/urandom", "r")) ) {
		printf("Unable to open file\n");
		exit(1);
	};
	gets_safe(f, &str);
	printf("%s", str);

	free(str);
	return 0;
}