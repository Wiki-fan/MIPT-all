#include <string.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFSIZE 1024

int main(int argc, char* argv[]) {
    char* filename;
    char buf[BUFSIZE];
    FILE* file;
    size_t nread;
    size_t count = 0;
    if (argc != 2) {
        err(1, "wrong num of params");
    }
    filename = argv[1];

    file = fopen(filename, "r");
    if (file) {
        size_t i;
        while ((nread = fread(buf, 1, sizeof(buf), file)) > 0) {
            for(i = 0; i<nread; ++i) {
                if (isalpha(buf[i])) {
                    ++count;
                }
            }
        }
        if (ferror(file)) {
            err(1, "error reading file");
        }
        fclose(file);
    } else {
        err(1, "error opening file");
    }

    printf("%d\n", count);
    return 0;
}