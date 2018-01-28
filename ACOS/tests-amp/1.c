#include <string.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    unsigned long int x;
    size_t i;
    size_t count = 0;
    char *nptr, *endptr;
    if (argc != 2) {
        err(1, "wrong num of params");
    }

    nptr = argv[1];
    endptr = NULL;
    x = strtoul(nptr, &endptr, 10);
    if (*endptr != '\0') {
        err(1, "invalid number");
    }
    /*printf("%d\n", x);*/

    for (i = 0; i<sizeof(x)*8-1; ++i) {
        unsigned long int bit = (x>>i)&1;
        /*printf("%d", bit);*/
        if (bit != 0) {
            ++count;
        }
    }

    printf("%d\n", count);
    return 0;
}