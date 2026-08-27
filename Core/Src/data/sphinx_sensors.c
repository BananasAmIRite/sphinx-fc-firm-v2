#include "sphinx_sensors.h"

SphinxSensors sphinx_sensors; 

void sphinx_sensors_struct_init() {
    sphinx_sensors.data_rw_lock = osMutexNew(NULL);
}