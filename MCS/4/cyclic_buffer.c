#include "cyclic_buffer.h"

void cb_init(CyclicBuffer* cb, int capacity, int elem_size) {
    cb->size = cb->i1 = cb->i2 = 0;
    cb->capacity = capacity;
    cb->elem_size = elem_size;
    cb->buf = malloc(capacity*elem_size);
}

void cb_push(CyclicBuffer* cb, void* value) {
    assert(cb->size < cb->capacity);
    memcpy(cb->buf+cb->size*cb->elem_size, value, cb->elem_size);
    cb->i2 = (cb->i2+1)%cb->capacity;
    ++cb->size;
}

void cb_pop(CyclicBuffer* cb, void* value) {
    assert(cb->capacity >= 0);
    --cb->size;
    memcpy(value, cb->buf+cb->size*cb->elem_size, cb->elem_size);
    cb->i1 = (cb->size+1)%cb->capacity;
}

void cb_destroy(CyclicBuffer* cb) {
    free(cb->buf);
}
