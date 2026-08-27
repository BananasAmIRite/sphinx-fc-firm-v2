#include "state_machine.h"
#include "FreeRTOS.h"

StateMachineContext state_machine_ctx; 

// state machine threads config
StateMachineThreadConfig thread_20hz = {
    .ctx = &state_machine_ctx,
    .thread_idx = 0,
    .period_ms = 50
};

StateMachineThreadConfig thread_30hz = {
    .ctx = &state_machine_ctx,
    .thread_idx = 1,
    .period_ms = 33
};

StateMachineThreadConfig thread_100hz = {
    .ctx = &state_machine_ctx,
    .thread_idx = 2,
    .period_ms = 10
};

void state_machine_thread(void *argument)
{
    StateMachineThreadConfig *config =
        (StateMachineThreadConfig *)argument;

    TickType_t period = pdMS_TO_TICKS(config->period_ms);
    TickType_t last_wake = osKernelGetTickCount();

    while (1) {

        loop_state_machine_thread(
            config->thread_idx,
            config->ctx
        );

        osDelayUntil(last_wake + period);
    }
}

void state_machine_master_thread(void *argument)
{
    StateMachineContext *ctx = argument;

    const TickType_t period = pdMS_TO_TICKS(10); // 100 Hz
    TickType_t last_wake = osKernelGetTickCount();

    while (1) {

        loop_state_machine_master(ctx);

        osDelayUntil(last_wake + period);
    }
}

void init_state_machine(State* starting_state) {
    state_machine_init(&state_machine_ctx); 

    state_machine_request_transition(
        &state_machine_ctx,
        starting_state
    );
    
    // master
    osThreadNew(
        state_machine_master_thread, 
        &state_machine_ctx, 
        NULL
    );

    // workers
    osThreadNew(
        state_machine_thread,
        &thread_20hz,
        NULL
    );

    osThreadNew(
        state_machine_thread,
        &thread_30hz,
        NULL
    );

    osThreadNew(
        state_machine_thread,
        &thread_100hz,
        NULL
    );
}