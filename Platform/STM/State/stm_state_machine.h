#ifndef __STM_STATE_MACHINE_H__
#define __STM_STATE_MACHINE_H__

#include <stdbool.h>
#include <stdint.h>
#include "state.h"
#include "platform_runtime.h"
#include "cmsis_os2.h"

typedef struct {
    State *current_state;
    State *next_state;
    PlatformRuntime *runtime;

    osMutexId_t mutex;
    uint32_t active_workers;
    bool transitioning;
} StmStateMachine;

typedef struct {
    StmStateMachine *ctx;
    uint8_t thread_idx;
    uint32_t period_ms;
} StmThreadConfig;

// initializes StmStateMachine object
void stm_state_machine_init(StmStateMachine *sm);

// request transition layer for state machine interface
void stm_state_machine_request_transition(StmStateMachine *sm, State *next);

// void stm_state_machine_master_loop(StmStateMachine *sm);
// void stm_state_machine_worker_loop(StmStateMachine *sm, uint8_t thread_idx);
void stm_state_machine_master_thread(void *argument);
void stm_state_machine_thread(void *argument);

#endif