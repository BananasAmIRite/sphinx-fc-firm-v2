#ifndef __SPHINX_IO_H__
#define __SPHINX_IO_H__

#include <stdbool.h>
#include <stdint.h>
#include "sphinx_data.h"

// all io declarations
typedef struct {
    // TODO: fill in


    int8_t (*altimeter_init)(); 
    int8_t (*altimeter_read)(AltimeterReadings* readings); 

    int8_t (*altimeter_tof_init)(); 
    int8_t (*altimeter_tof_read)(AltimeterReadings* readings); 
    
} SphinxIO; 

void sphinx_io_init(); 

#endif