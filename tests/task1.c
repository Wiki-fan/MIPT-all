/* Variant 1. */

#include <stdlib.h>
#include <err.h>
#include "../task-s1-bitwise_print/bitwise_print.h"

/* assuming numeration from 1 and closed segments [a,b] */
/* this program performs simple test */
#define POS1 2
#define POS2 19
#define BLOCK 0x7ff

int main( int argc, char* argv[] )
{
    int temp1, temp2;
    unsigned int a;
    if( argc != 2 )
    {
        errx( 10, "Wrong number of parameters" );
    }
    a = atoi(argv[1])/*0b10000000000010101010111111111110*//*0b00111111111110000000000000000000*/;

    temp1 = ( a >> POS1 ) & BLOCK; /* from 3 to 13 */
    /*BITWISE_FORMATTED_PRINT(temp1);*/
    temp2 = ( a >> POS2 ) & BLOCK;
    /*BITWISE_FORMATTED_PRINT(temp2);*/
    BITWISE_FORMATTED_PRINT( a );
    a &= ~( BLOCK << POS1 ); /* set from 3 to 13 to 1 */
    a &= ~( BLOCK << POS2 );
    /*BITWISE_FORMATTED_PRINT(a);*/
    a |= ( temp2 << POS1 );
    a |= ( temp1 << POS2 );
    BITWISE_FORMATTED_PRINT( a );

    return 0;
}