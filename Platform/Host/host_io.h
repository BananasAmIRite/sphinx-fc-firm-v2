#ifndef __HOST_IO_H__
#define __HOST_IO_H__

#include "stdint.h"
#include "sphinx_io.h"
#include "sphinx_data.h"

int8_t altimeter_init(); 
int8_t altimeter_read(AltimeterReadings* readings); 

int8_t altimeter_tof_init(); 
int8_t altimeter_tof_read(AltimeterReadings* readings); 

#endif