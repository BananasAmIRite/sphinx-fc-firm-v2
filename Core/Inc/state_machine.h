#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#include "state.h"

typedef struct {
    StateMachineContext *ctx;
    uint8_t thread_idx;
    uint32_t period_ms;
} StateMachineThreadConfig;

void init_state_machine(State* starting_state); 

#endif