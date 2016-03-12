#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*Function that reads string from the f stream before \n or \0 characters are found and stores the result in array pointed by str.
 * Returns the number s of characters copied to str and 0, if EOF is reached. Newline character is stored in str although it terminates reading.
 * End-of-string character \0 is stored at s+1 position of *str.
 */
extern int gets_safe(FILE* f, char** str);
