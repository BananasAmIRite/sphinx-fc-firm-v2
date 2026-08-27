#ifndef __SPHINX_IO_H__
#define __SPHINX_IO_H__

#include <stdbool.h>
#include <stdint.h>

// sensor structs
typedef struct {
    double altitude_meters; 
    double altitude_tof_meters; 
    bool altitude_tof_available; // TODO: make sure to check this before incorporating into kalman filter
} AltimeterReadings; 

// all io declarations
typedef struct {
    // TODO: fill in


    int8_t (*altimeter_init)(); 
    int8_t (*altimeter_read)(AltimeterReadings* readings); 

    int8_t (*altimeter_tof_init)(); 
    int8_t (*altimeter_tof_read)(AltimeterReadings* readings); 
    
} SphinxIO; 

// bindings for Sphinx IO
extern SphinxIO sphinx_io_sim; 
extern SphinxIO sphinx_io_real; 

extern SphinxIO* sphinx_io_inst; // the current io instance being used, this is global and can be used anywhere

void sphinx_io_init(); 

#endif