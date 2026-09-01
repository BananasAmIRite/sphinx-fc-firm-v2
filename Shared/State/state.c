#include "state.h"
#include "platform_runtime.h"

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

// state lifecycle
void state_start(PlatformRuntime *runtime, State *state) {
    for (size_t i = 0; i < state->algorithm_count; i++) {
        Algorithm *alg = state->algorithms[i];

        if (!state_alg_get_enabled(state, i)) {
            continue;
        }

        if (alg != NULL && alg->start != NULL) {
            alg->start(alg, runtime);
        }
    }
}

void state_loop(PlatformRuntime *runtime, uint8_t thread_idx, State *state) {

    for (size_t i = 0; i < state->algorithm_count; i++) {
        Algorithm *alg = state->algorithms[i];

        if (alg == NULL) {
            continue;
        }

        if (runtime->enable_threads && alg->thread_idx != thread_idx) {
            continue;
        }

        if (!state_alg_get_enabled(state, i)) {
            continue;
        }

        if (alg->loop != NULL) {
            alg->loop(alg, runtime);
        }
    }
}

void state_stop(PlatformRuntime *runtime, State *state) {
    for (size_t i = 0; i < state->algorithm_count; i++) {
        Algorithm *alg = state->algorithms[i];

        if (!state_alg_get_enabled(state, i)) {
            continue;
        }

        if (alg != NULL && alg->stop != NULL) {
            alg->stop(alg, runtime);
        }
    }
}