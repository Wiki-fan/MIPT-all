#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*
 * Function that reads string from the f stream before term characters are found and stores the result in array pointed by str.
 * Returns the number s of characters copied to str and 0, if EOF is reached. Newline character is stored in str although it terminates reading.
 * End-of-string character \0 is stored at s+1 position of *str.
 */
extern int gets_safe_ext(FILE* f, char** str, const char* term);

/*
 * Variation of gets_safe_ext, where term is always "\n".
 */
extern int gets_safe(FILE* f, char** str);