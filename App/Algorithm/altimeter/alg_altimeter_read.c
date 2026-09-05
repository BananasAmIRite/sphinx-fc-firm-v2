#include "alg_altimeter_read.h"
#include "sphinx_io.h"
#include <platform_runtime.h>
#include "sphinx_data.h"


#ifdef FREERTOS_ENABLED
#include "FreeRTOS.h"
#endif

AltReadContext alt_read_ctx = {}; 
Algorithm alt_read_algorithm = {
    .start = alt_read_start, 
    .loop = alt_read_loop, 
    .stop = alt_read_stop, 
    .context = &alt_read_ctx, 
    .thread_idx = 1
}; 

void alt_read_start(Algorithm *self, PlatformRuntime *runtime) {}

void alt_read_loop(Algorithm *self, PlatformRuntime *runtime) {
    // #ifdef FREERTOS_ENABLED
    //     // don't wait longer than the thread loop time
    //     // osMutexAcquire(sphinx_sensors.data_rw_lock, pdMS_TO_TICKS(10)); 
    // #endif
    // TODO: may need to separate if they have separate timings
    runtime->io->altimeter_read(sphinx_data.raw_alti_readings); 
    runtime->io->altimeter_tof_read(sphinx_data.raw_alti_readings); 
    
    // #ifdef FREERTOS_ENABLED
    //     // osMutexRelease(sphinx_sensors.data_rw_lock); 
    // #endif
}

void alt_read_stop(Algorithm *self, PlatformRuntime *runtime) {}