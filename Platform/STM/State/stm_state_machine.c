#include "stm_state_machine.h"
#include "FreeRTOS.h"

void stm_state_machine_init(StmStateMachine *sm) {
    if (sm == NULL) {
        return;
    }

    sm->current_state = NULL;
    sm->next_state = NULL;
    sm->runtime = NULL;
    sm->active_workers = 0;
    sm->transitioning = false;
    sm->mutex = osMutexNew(NULL);
}

void stm_state_machine_request_transition(StmStateMachine *sm, State *next) {
    if (sm == NULL || next == NULL) {
        return;
    }

    osMutexAcquire(sm->mutex, osWaitForever);
    sm->next_state = next;
    osMutexRelease(sm->mutex);
}

void stm_state_machine_master_loop(StmStateMachine *sm) {
    State *next = NULL;

    if (sm == NULL) {
        return;
    }

    osMutexAcquire(sm->mutex, osWaitForever);
    next = sm->next_state;

    if (next == NULL) {
        osMutexRelease(sm->mutex);
        return;
    }

    sm->next_state = NULL;
    sm->transitioning = true;
    osMutexRelease(sm->mutex);

    while (1) {
        osMutexAcquire(sm->mutex, osWaitForever);
        bool done = (sm->active_workers == 0);
        osMutexRelease(sm->mutex);

        if (done) {
            break;
        }

        osThreadYield();
    }

    if (sm->current_state != NULL) {
        state_stop(sm->runtime, sm->current_state);
    }

    sm->current_state = next;

    if (sm->current_state != NULL) {
        state_start(sm->runtime, sm->current_state);
    }

    osMutexAcquire(sm->mutex, osWaitForever);
    sm->transitioning = false;
    osMutexRelease(sm->mutex);
}

void stm_state_machine_worker_loop(StmStateMachine *sm, uint8_t thread_idx) {
    State *state = NULL;

    if (sm == NULL) {
        return;
    }

    osMutexAcquire(sm->mutex, osWaitForever);

    if (!sm->transitioning) {
        state = sm->current_state;

        if (state != NULL) {
            sm->active_workers++;
        }
    }

    osMutexRelease(sm->mutex);

    if (state != NULL) {
        state_loop(sm->runtime, thread_idx, state);

        osMutexAcquire(sm->mutex, osWaitForever);
        sm->active_workers--;
        osMutexRelease(sm->mutex);
    }
}


void stm_state_machine_thread(void *argument)
{
    StmThreadConfig *config = (StmThreadConfig *)argument;

    TickType_t period = pdMS_TO_TICKS(config->period_ms);
    TickType_t last_wake = osKernelGetTickCount();

    while (1) {
        stm_state_machine_worker_loop(config->ctx, config->thread_idx);
        osDelayUntil(last_wake + period);
    }
}

void stm_state_machine_master_thread(void *argument)
{
    StmStateMachine *sm = (StmStateMachine *)argument;

    const TickType_t period = pdMS_TO_TICKS(10);
    TickType_t last_wake = osKernelGetTickCount();

    while (1) {
        stm_state_machine_master_loop(sm);
        osDelayUntil(last_wake + period);
    }
}