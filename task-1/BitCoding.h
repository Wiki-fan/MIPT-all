#include "stdio.h"

/*
 * Macro that prints all the bits of X to stdout. 
 */
#define BITWISE_PRINT( X ) \
{ \
int _i; \
for (_i = sizeof(X)*8-1; _i>=0; --_i) { \
putchar( ( (X >> _i) & 1) ? '1':'0'); \
} \
}

/*
 * Macro that prints all the bits of X to stdout. 
 */
#define BITWISE_PRINT2( X ) \
{ \
int _i,_k; \
for(_i=sizeof(X)-1;_i>=0;--_i) \
    for(_k = 7;_k>=0; --_k) { \
        putchar( \
            ( (((unsigned char*)&X)[_i] >> _k) & 1) \
            ?'1':'0'); \
} \
}

/*
 * Function that encodes symbols in alphabet with cardinality lesser than
 * 256. Returns number of bits written to answer. Both toEncode and
 * alphabet are c-strings. answer will be pointing to resulting c-string.
 * Memory for answer is allocated inside the function.
 */
extern int      BitEncode(const char *toEncode, const char *alphabet,
			  char **answer);

/*
 * Function that decodes symbols in alphabet with cardinality lesser than
 * 256. Returns number of bytes written to answer. alphabet is c-string.
 * answer will be pointing to resulting c-string. toDecode is pointing to
 * bits encoded bits. Memory for answer is allocated inside the function.
 */
extern int      BitDecode(const char *toDecode, const char *alphabet,
			  char **answer, int bits);
