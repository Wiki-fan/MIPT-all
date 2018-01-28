/* Variant 1. */

#include <stdio.h>
#include <string.h>
#include <err.h>
#include "../common/manipulations.h"
#include "../task-s2-gets_safe/safe_string.h"

void process_string(char* str, char* substr)
{
    char* temp = strstr(str, substr);
    if (temp != NULL)
    {
        int a_count, i;
        char* iter = temp;
        a_count = strcnt(substr, 'a');

        /* writing # */
        for (i = 0; i<a_count; ++i)
        {
            *(iter++) = '#';
        }

        /* copying the rest of the string */
        temp += strlen(substr);
        while(*iter++ = *temp++);
    }
}

int main( int argc, char* argv[])
{
    FILE* f;
    char* str;

    if (argc != 3)
        errx(10, "Wrong number of parameters");

    f = fopen_s(argv[1], "r+");

    while (gets_safe(f, &str))
    {
        process_string(str, argv[2]);
        printf("%s",str);
        free(str);
    }

    return 0;

}