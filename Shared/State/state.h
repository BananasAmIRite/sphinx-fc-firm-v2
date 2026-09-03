#ifndef __STATE_H__
#define __STATE_H__

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "algorithm.h"

#define MAX_ALGORITHMS 16

typedef struct {
    Algorithm *algorithms[MAX_ALGORITHMS];
    size_t algorithm_count;
    uint16_t alg_enabled;  // bitfield of algorithm enables (based on MAX_ALGORITHMS)
} State;

// state helpers
// TODO: note that these are currently not very safe to use and aren't incorporated into algorithm lifecycle (ie. disabling an Algorithm doesn't run its stop())
void state_alg_set_enabled(State* state, uint8_t idx, bool enabled);
bool state_alg_get_enabled(State* state, uint8_t idx);

// state lifecycle
void state_start(PlatformRuntime *runtime, State *state);
void state_loop(PlatformRuntime *runtime, uint8_t thread_idx, State *state);
void state_stop(PlatformRuntime *runtime, State *state);

#endif