#include "alg_altimeter_read.h"
#include "sphinx_sensors.h"
#include "sphinx_io.h"
#include "FreeRTOS.h"

AltReadContext alt_read_ctx = {}; 
Algorithm alt_read_algorithm = {
    .start = alt_read_start, 
    .loop = alt_read_loop, 
    .stop = alt_read_stop, 
    .context = &alt_read_ctx, 
    .thread_idx = 1
}; 

void alt_read_start(Algorithm *self) {}

void alt_read_loop(Algorithm *self) {
    // don't wait longer than the thread loop time
    osMutexAcquire(sphinx_sensors.data_rw_lock, pdMS_TO_TICKS(10)); 
    // TODO: may need to separate if they have separate timings
    sphinx_io_inst->altimeter_read(&sphinx_sensors.data.altimeter_readings); 
    sphinx_io_inst->altimeter_tof_read(&sphinx_sensors.data.altimeter_readings); 
    osMutexRelease(sphinx_sensors.data_rw_lock); 
}

void alt_read_stop(Algorithm *self) {}