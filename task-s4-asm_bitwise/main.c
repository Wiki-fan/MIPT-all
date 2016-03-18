#include <stdio.h>
#include "../common/bit_manipulations.h"
#include "stdlib.h"

int print_bits(int value, char* str);

int main()
{
    int i;
    char* str = malloc( sizeof( char ) * 100 );
    for( i = 0; i < 5000; ++i )
    {
        print_bits( i, str );
        puts( str );
    }

    return 0;
}

