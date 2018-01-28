#include "safe_string.h"

int main()
{
    FILE* f;
    char* str;
    f = fopen_s("/dev/urandom", "r");

    gets_safe(f, &str);
    printf("%s", str);

    free(str);
    return 0;
}