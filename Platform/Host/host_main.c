#include "host_main.h"
#include "platform_runtime.h"
#include "host_state_machine.h"
#include "states_config.h"
#include "host_io.h"
#include <stdio.h>

int main() {

    #ifdef WIN
        printf("Running sim on windows\r\n"); 
    #endif
    #ifdef UNIX
        printf("Runnign sim on unix\r\n");
    #endif 
    
    PlatformRuntime host_runtime = {0}; 

    // context for host state machine operation (without RTOS)
    HostStateMachine host_sm;
    host_sm.runtime = &host_runtime; 
    host_sm.cur_state = NULL;
    host_sm.next_state = NULL; 


    // some other methods we have to implement for IO
    SphinxIO io = {
        .altimeter_init = altimeter_init,
        .altimeter_read = altimeter_read, 
        
        .altimeter_tof_init = altimeter_tof_init,
        .altimeter_tof_read = altimeter_tof_read
    }; 
    // calls all the init() methods in io
    sphinx_io_init(&io); 
    
    // init host runtime
    host_runtime.state_machine_ctx = &host_sm;
    host_runtime.io = &io;
    host_runtime.request_transition = (StateTransitionRequestFn)host_state_machine_request_transition; 
    host_runtime.enable_threads = false; // this is kinda stupid rn 

    host_state_machine_request_transition(&host_sm, &idle_state); 

    // TODO: instead of ticking independently using a while loop, you could also loop once every "tick" from an external program if you'd like
    while (1) {
        loop_host_state_machine(&host_sm); 
    }

}