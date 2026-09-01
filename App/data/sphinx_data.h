#ifndef __SPHINX_DATA_H__
#define __SPHINX_DATA_H__

#include <stdbool.h>
#include <stdint.h>

// sensor structs
typedef struct {
    double altitude_meters; 
    double altitude_tof_meters; 
    bool altitude_available; 
    bool altitude_tof_available; // TODO: make sure to check this before incorporating into kalman filter
} AltimeterReadings; 

typedef struct {
    AltimeterReadings *raw_alti_readings; 
} SphinxData; 

extern SphinxData sphinx_data; 

#endif