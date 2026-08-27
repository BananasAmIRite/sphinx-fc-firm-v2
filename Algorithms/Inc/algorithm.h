#ifndef __ALGORITHMS_H__
#define __ALGORITHMS_H__

#include <stdint.h>

typedef struct Algorithm Algorithm;

struct Algorithm {
    void (*start)(Algorithm *self);
    void (*loop)(Algorithm *self);
    void (*stop)(Algorithm *self);

    void *context;
    uint8_t thread_idx;
};

#endif