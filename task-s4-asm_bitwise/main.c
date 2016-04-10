#include <stdio.h>
#include "../task-s1-bitwise_print/bitwise_print.h"
#include <stdlib.h>

void print_bits(int value, char* str);

int main()
{
    int i;
    /* Guaranteed to be safe. */
    char* str = malloc( sizeof( char ) * 100 );
    for( i = 0; i < 5000; ++i )
    {
        print_bits( i, str );
        puts( str );
        BITWISE_FORMATTED_PRINT(i);
        putchar('\n');
    }
    free(str);
    return 0;
}

