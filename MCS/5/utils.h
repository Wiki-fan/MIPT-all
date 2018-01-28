#pragma once

#include <assert.h>
#include <stdio.h>

#define swap(x,y) do \
   { unsigned char swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1]; \
     memcpy(swap_temp,&y,sizeof(x)); \
     memcpy(&y,&x,       sizeof(x)); \
     memcpy(&x,swap_temp,sizeof(x)); \
    } while(0)

#define MAX(A, B) ((A)>(B)? (A):(B))

#ifdef DEBUG
#define debug(A) A
#else
#define debug(A) do {} while(0)
#endif

#define PRERR(VAL) do { int err; if ((err = (VAL))) { printf("%s", strerror(err)); assert(0);} } while(0)

int rand_mt();
