#ifndef COMMON__MANIPULATIONS_H
#define COMMON__MANIPULATIONS_H

#include <stdio.h>

int strcnt (char *s, char c);
void read_file( FILE* f, char*** strs, int** lengths, int* lines_number, int* columns_number);

#endif /* COMMON__MANIPULATIONS_H */