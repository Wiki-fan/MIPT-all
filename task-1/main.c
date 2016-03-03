#include "BitCoding.h"
#include <malloc.h>

int
main()
{
    int             i;
    const char     *alphabet = "atgcu";
    const char     *str = "atgcuagtucga";
    char           *ans;
    char           *ans2;
    int             count,
                    count2;
    for (i = 0; i < 5000; ++i) {
	printf("%4d ", i);
	BITWISE_PRINT(i);
	putchar(' ');
	BITWISE_PRINT2(i);

	putchar('\n');
    }

    count = BitEncode(str, alphabet, &ans);
    printf("%d\n", count);

    count2 = BitDecode(ans, alphabet, &ans2, count);
    printf("%d: %s\n", count2, ans2);

    free(ans);
    free(ans2);

    return 0;
}
