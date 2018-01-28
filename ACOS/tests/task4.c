/* Variant 1. */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "../common/manipulations.h"

#define N 6

void filltest(char* filename)
{
    FILE* f;
    float arr[N];
    int i;
    for (i = 0; i<N; ++i)
        arr[i] = i;

    f = fopen_s(filename, "rb+");

    fwrite(arr, sizeof(float), N, f);
    fclose(f);
}

void out(FILE* f)
{
    float arr[N];
    int i;
    fseek(f, 0,SEEK_SET);
    fread(arr, sizeof(float), N, f);
    for (i = 0; i<N; ++i)
        printf("%f ", arr[i]);
}

int main(int argc, char* argv[])
{
    FILE* f;
    float a,c;
    int count,i;
    if (argc != 2)
        errx(10, "Wrong number of parameters");
    /*filltest(argv[1]);*/

    f = fopen_s(argv[1], "rb+");

    fseek(f, 0, SEEK_END);
    count = ftell(f)/sizeof(float);
    if (count %3 != 0)
    {
        errx(20, "Can't divide");
    }
    count /= 3;
    for (i = 0; i<count; ++i)
    {
        fseek(f, i*sizeof(float),SEEK_SET );
        fread(&a, sizeof(float), 1, f);
        fseek(f, (2*count+i)*sizeof(float),SEEK_SET);
        fread(&c, sizeof(float), 1, f);

        fseek(f,  i*sizeof(float),SEEK_SET );
        fwrite(&c, sizeof(float), 1, f);
        fseek(f, (2*count+i)*sizeof(float),SEEK_SET);
        fwrite(&a, sizeof(float), 1, f);
    }

    /*out(f);*/
    fclose(f);

    return 0;
}