#ifndef TASK_L1__BIT_CODING
#define TASK_L1__BIT_CODING
#include <stdlib.h>

/* Function that encodes symbols in alphabet with cardinality lesser than
 * 256. Returns number of successfully coded symbols (excluding end-of-string). Both toEncode and
 * alphabet are c-strings. answer will be pointing to resulting c-string.
 * Memory for answer is allocated inside the function.
 */
extern int BitEncode(const char *toEncode, const char *alphabet,
                     char **answer );

/*
 * Function that decodes symbols in alphabet with cardinality lesser than
 * 256. Returns number of bytes written to answer. alphabet is c-string.
 * answer will be pointing to resulting c-string. toDecode is pointing to
 * bits encoded bits. Memory for answer is allocated inside the function.
 */
/* returns number of decoded. It will be lesser than size, if end character found. */
extern int BitDecode(const char *toDecode, const char *alphabet,
                     char **answer, int size );

#endif /* TASK_L1__BIT_CODING */