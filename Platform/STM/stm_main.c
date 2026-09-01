#include "stm_main.h"
#include "platform_runtime.h"
#include "stm_state_machine.h"
#include "states_config.h"

#include "bmp5_drv.h"
#include "vl53l1x_drv.h"
void stm_main(void) {
    // highest-level platform runtime context, providing bindings for everything we need
    PlatformRuntime stm_runtime = {0}; 

    // context for STM state machine operation with RTOS
    StmStateMachine stm_sm;
    stm_sm.runtime = &stm_runtime; 
    stm_state_machine_init(&stm_sm);
    
    // configure IO layer to be fed into runtime
    #ifdef HIL_SIM
        // TODO: update for sim
        SphinxIO io = {}; 
    #else
        SphinxIO io = {
            .altimeter_init = bmp5_config,
            .altimeter_read = bmp5_read_altitude, 
            
            .altimeter_tof_init = tof_config,
            .altimeter_tof_read = tof_read_altitude
        }; 
    #endif
    sphinx_io_init(&io); 

    // configure runtime with state machine context
    stm_runtime.state_machine_ctx = &stm_sm;
    stm_runtime.io = &io; 
    stm_runtime.request_transition = (StateTransitionRequestFn)stm_state_machine_request_transition;
    stm_runtime.enable_threads = true; 


    StmThreadConfig thread_20hz = {0};
    StmThreadConfig thread_30hz = {0};
    StmThreadConfig thread_100hz = {0};

    thread_20hz.ctx = &stm_sm;
    thread_20hz.thread_idx = 0;
    thread_20hz.period_ms = 50;

    thread_30hz.ctx = &stm_sm;
    thread_30hz.thread_idx = 1;
    thread_30hz.period_ms = 33;

    thread_100hz.ctx = &stm_sm;
    thread_100hz.thread_idx = 2;
    thread_100hz.period_ms = 10;

    stm_state_machine_request_transition(&stm_sm, &idle_state); 

    // create threads
    // master
    osThreadNew(stm_state_machine_master_thread, &stm_sm, NULL);

    // workers
    osThreadNew(stm_state_machine_thread, &thread_20hz, NULL);
    osThreadNew(stm_state_machine_thread, &thread_30hz, NULL);
    osThreadNew(stm_state_machine_thread, &thread_100hz, NULL);

    // yield control to rtos scheduler
}