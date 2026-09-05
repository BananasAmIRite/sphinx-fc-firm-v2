#include "host_state_machine.h"

void loop_host_state_machine(HostStateMachine* context) {

    if (context->next_state != NULL) {
        // we have a state to transition to
        if (context->cur_state != NULL) state_stop(context->runtime, context->cur_state);
        if (context->next_state != NULL) state_start(context->runtime, context->next_state); // redundant but meh
        // set the new state
        context->cur_state = context->next_state; 
        context->next_state = NULL; 
    }
    
    // do the loop step for the current states (thread 0 since we don't have threads enabled here)
    // runs all the algs basically
    if (context->cur_state != NULL) state_loop(context->runtime, 0, context->cur_state); 
}

void host_state_machine_request_transition(HostStateMachine* context, State* new_state) {
    context->next_state = new_state; 
}