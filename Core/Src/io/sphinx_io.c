#include "sphinx_io.h"

#include "bmp5_drv.h"
#include "vl53l1x_drv.h"

// TODO: fill in once we get implementations
SphinxIO sphinx_io_real = {
    .altimeter_init = bmp5_config,
    .altimeter_read = bmp5_read_altitude, 
    
    .altimeter_tof_init = tof_config,
    .altimeter_tof_read = tof_read_altitude
}; 

SphinxIO sphinx_io_sim = {}; 

SphinxIO* sphinx_io_inst = &sphinx_io_real; 

void sphinx_io_init() {
    // TODO: add hot swapping based on which io to use


    sphinx_io_inst->altimeter_init(); 
    sphinx_io_inst->altimeter_tof_init(); 
}