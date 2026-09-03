#ifndef __STATE_RUNTIME_H__
#define __STATE_RUNTIME_H__
#include "state.h"
#include "sphinx_io.h"

// platform runtime; defines what APIs are available per platform 

typedef void (*StateTransitionRequestFn)(void *state_machine_ctx, State *next);

// each hosting platform will have to specify: a request_transition func, a state machine context to pass into request_transition, an IO layer, and whether to enable threads
struct PlatformRuntime {
    void *state_machine_ctx;
    StateTransitionRequestFn request_transition;
    SphinxIO *io; 

    bool enable_threads; 
};

#endif