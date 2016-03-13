#include <stdio.h>

struct Data
{
char mode;
	int dimensions[3];
};

/* Warning: due to aligning struct will be larger than sum of all its fields. We can write fields simultaneously. */
/* Warning: there is no counter of written elements or error field. */
int main()
{
	struct Data *a;
	int num = 4;
	FILE* f = fopen("input", "w");
	fwrite(a, sizeof(struct Data), num, f );
	fclose(f);
	return 0;
}