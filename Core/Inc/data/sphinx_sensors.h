#ifndef __SPHINX_SENSORS_H__
#define __SPHINX_SENSORS_H__
#include <cmsis_os2.h>
#include "sphinx_io.h"

typedef struct {
    AltimeterReadings altimeter_readings; 
} SphinxSensorsData; 


// TODO: may want to explicitly initialize these structs? 
typedef struct {
    SphinxSensorsData data; 
    osMutexId_t data_rw_lock; 
} SphinxSensors; 

extern SphinxSensors sphinx_sensors; 

void sphinx_sensors_struct_init(); 

#endif