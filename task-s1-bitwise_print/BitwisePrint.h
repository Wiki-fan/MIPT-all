#ifndef TASK_S1__BITWISE_PRINT
#define TASK_S1__BITWISE_PRINT

#include <stdio.h>
#include "../common/BitManipulations.h"

/*
 * Macro that prints all the bits of X to stdout.
 * Format like that:
 * 0001 1111 | ....
 *    1    F
 */

#define BITWISE_FORMATTED_PRINT( X ) \
{ \
	int i##X, k##X; \
	for( i##X = sizeof(X)-1; i##X >= 0; --i##X ) {\
        for( k##X = 7; k##X >= 4; --k##X ) \
        { \
            putchar( \
                ( GET_BIT(k##X, i##X, X) \
                ?'1':'0')); \
        } \
        putchar(' '); \
        for(; k##X >= 0; --k##X ) \
        { \
            putchar( \
                ( GET_BIT(k##X, i##X, X) \
                ?'1':'0')); \
        } \
        if (i##X != 0) \
        { \
            printf(" | "); \
        } \
	} \
	putchar('\n'); \
	for ( i##X = (int)sizeof(X)*2-1; i##X >= 0 ; --i##X ) \
	{ \
		printf("%4x", i##X*4); \
		if (i##X%2 ==0 ) \
		{ \
			printf("   "); \
		} \
		else \
		{ \
			putchar(' '); \
		} \
	} \
	putchar('\n'); \
}

/*
 * Macro that prints all the bits of X to stdout.
 */

#define BITWISE_PRINT( X ) \
{ \
	int i##X, k##X; \
	for( i##X = sizeof(X)-1; i##X >= 0; --i##X ) {\
        for( k##X = 7; k##X >= 0; --k##X ) \
        { \
            putchar( \
                ( GET_BIT(k##X, i##X, X) \
                ?'1':'0')); \
        } \
	} \
	putchar('\n'); \
}

#endif /* TASK_S1__BITWISE_PRINT */
