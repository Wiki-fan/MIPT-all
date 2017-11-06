#pragma once

#include <malloc.h>
#include <string.h>
#include <assert.h>

typedef struct {
    void* buf;
    int size;
    int i1, i2;
    int capacity;
    int elem_size;
} CyclicBuffer;

void cb_init(CyclicBuffer* cb, int capacity, int elem_size);

void cb_push(CyclicBuffer* cb, void* value);

void cb_pop(CyclicBuffer* cb, void* value);

void cb_destroy(CyclicBuffer* cb);
