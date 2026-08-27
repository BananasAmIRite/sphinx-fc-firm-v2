#ifndef __STATE_H__
#define __STATE_H__

#include <stdint.h>
#include <stdbool.h>
#include "algorithm.h"
#include <cmsis_os2.h>

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
void state_start(State *state); 
void state_loop(uint8_t thread_idx, State *state);
void state_stop(State *state); 


// state machine
typedef struct {
    State* current_state; 

    // state transitions
    State* next_state; 
    
    // multithreading management
    osMutexId_t state_mutex; // mutex/lock for workers_done, active_workers, transitioning
    uint32_t active_workers;
    bool transitioning; 
} StateMachineContext; 


// state machine helpers
void state_machine_init(StateMachineContext *ctx);


void state_machine_request_transition(
    StateMachineContext *ctx,
    State *next
);

// loop function for the master thread
void loop_state_machine_master(StateMachineContext *ctx);

// loop function for each worker thread that runs the algorithms
void loop_state_machine_thread(
    uint8_t thread_idx,
    StateMachineContext *ctx
); 

#endif