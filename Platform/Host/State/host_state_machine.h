#include <state.h>
#ifndef __HOST_STATE_MACHINE_H__
#define __HOST_STATE_MACHINE_H__


typedef struct {
    State* cur_state; 
    State* next_state; 

    PlatformRuntime* runtime; 
} HostStateMachine; 

void loop_host_state_machine(HostStateMachine* context); 

void host_state_machine_request_transition(HostStateMachine* context, State* new_state); 

#endif