#include "state.h"


// state helpers
void state_alg_set_enabled(State* state, uint8_t idx, bool enabled) {
    if (enabled) {
        state->alg_enabled |= ((uint16_t) 0x01) << idx; 
    } else {
        state->alg_enabled &= ~(((uint16_t) 0x01) << idx); 
    }
}

bool state_alg_get_enabled(State* state, uint8_t idx) {
    return (state->alg_enabled >> idx) & 0x01; 
}

// state machine helpers
void state_machine_init(StateMachineContext *ctx) {
    ctx->current_state = NULL;
    ctx->next_state = NULL;

    ctx->active_workers = 0;
    ctx->transitioning = false;

    ctx->state_mutex = osMutexNew(NULL);
}

// state lifecycles
void state_start(State *state) {
    for (size_t i = 0; i < state->algorithm_count; i++) {
        Algorithm *alg = state->algorithms[i];

        // NOTE: these are a couple wasted cycles :/
        // if (alg->thread_idx != thread_idx) continue; // make sure algs only run on their designated threads
        if (!state_alg_get_enabled(state, i)) continue; // make sure only enabled algs run

        if (alg->start) alg->start(alg);
    }
}

void state_loop(uint8_t thread_idx, State *state) {
    for (size_t i = 0; i < state->algorithm_count; i++) {
        Algorithm *alg = state->algorithms[i];

        if (alg->thread_idx != thread_idx) continue; // make sure algs only run on their designated threads
        if (!state_alg_get_enabled(state, i)) continue; 

        if (alg->loop) alg->loop(alg);
    }
}

void state_stop(State *state) {
    for (size_t i = 0; i < state->algorithm_count; i++) {
        Algorithm *alg = state->algorithms[i];

        // if (alg->thread_idx != thread_idx) continue; 
        if (!state_alg_get_enabled(state, i)) continue; 


        if (alg->stop) alg->stop(alg);
    }
}




// state machine lifecycle

// for any thread to set the next state transition
void state_machine_request_transition(
    StateMachineContext *ctx,
    State *next
)
{
    // TODO: see if this is fine :/
    osKernelLock(); 
    ctx->next_state = next;
    osKernelUnlock(); 
}


// loop function for the master thread
void loop_state_machine_master(StateMachineContext *ctx)
{
    State *next;

    osMutexAcquire(ctx->state_mutex, osWaitForever);

    next = ctx->next_state;

    if (next == NULL) {
        osMutexRelease(ctx->state_mutex);
        return;
    }

    ctx->next_state = NULL;
    ctx->transitioning = true;

    osMutexRelease(ctx->state_mutex);

    // Wait for existing workers to finish
    while (1) {

        osMutexAcquire(ctx->state_mutex, osWaitForever);

        bool done = (ctx->active_workers == 0);

        osMutexRelease(ctx->state_mutex);

        if (done)
            break;

        osThreadYield();
    }

    // Nobody can be inside state_loop() now
    if (ctx->current_state)
        state_stop(ctx->current_state);

    ctx->current_state = next;

    if (ctx->current_state)
        state_start(ctx->current_state);

    osMutexAcquire(ctx->state_mutex, osWaitForever);

    ctx->transitioning = false;

    osMutexRelease(ctx->state_mutex);
}

// loop function for each worker thread that runs the algorithms
void loop_state_machine_thread(
    uint8_t thread_idx,
    StateMachineContext *ctx
)
{
    State *state = NULL;

    // acquire state to increment active workers
    osMutexAcquire(ctx->state_mutex, osWaitForever);

    if (!ctx->transitioning) { // only enter when not transitioning (otherwise state is NULL and we don't loop our state)
        state = ctx->current_state;

        if (state != NULL)
            ctx->active_workers++;
    }

    osMutexRelease(ctx->state_mutex);

    if (state != NULL) {

        // acquire state to decrement active workers
        state_loop(thread_idx, state);

        osMutexAcquire(ctx->state_mutex, osWaitForever);

        ctx->active_workers--;

        osMutexRelease(ctx->state_mutex);
    }
}