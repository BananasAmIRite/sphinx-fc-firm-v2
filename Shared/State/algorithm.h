#ifndef __ALGORITHMS_H__
#define __ALGORITHMS_H__

#include <stdint.h>

typedef struct Algorithm Algorithm;
typedef struct PlatformRuntime PlatformRuntime; 

struct Algorithm {
    void (*start)(Algorithm *self, PlatformRuntime *runtime);
    void (*loop)(Algorithm *self, PlatformRuntime *runtime);
    void (*stop)(Algorithm *self, PlatformRuntime *runtime);

    void *context;
    uint8_t thread_idx;
};

#endif