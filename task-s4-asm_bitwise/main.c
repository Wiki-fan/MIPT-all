#include <stdio.h>
#include "../common/bit_manipulations.h"

int print(long int X, char* str) {
	int i, k;
	for( i = sizeof(X)-1; i >= 0; --i ) {
        for( k = 7; k >= 4; --k )
        {
            *str++ = GET_BIT(k, i, X);
        }
		*str++ = ' ';
        for(; k >= 0; --k )
        {
	        *str++ = GET_BIT(k, i, X);
        }
        if (i != 0)
        {
	        sprintf(str, " | ");
        }
	}
	*str++ = '\n';
	for ( i = (int)sizeof(X)*2-1; i >= 0 ; --i )
	{
		sprintf(str, "%4x", (GET_BYTE(X, i/2) >> (i%2==0?0:4) & 0xf));
		if (i%2 ==0 )
		{
			sprintf(str, "   ");
		}
		else
		{
			putchar(' ');
		}
	}
	*str++ = '\n';
}