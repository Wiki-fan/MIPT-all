#pragma once


#define swap(x,y) do \
   { unsigned char swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1]; \
     memcpy(swap_temp,&y,sizeof(x)); \
     memcpy(&y,&x,       sizeof(x)); \
     memcpy(&x,swap_temp,sizeof(x)); \
    } while(0)

#define MAX(A, B) ((A)>(B)? (A):(B))

#ifdef DEBUG
#define IF_DBG(A) A
#else
#define IF_DBG(A) do {} while(0)
#endif
