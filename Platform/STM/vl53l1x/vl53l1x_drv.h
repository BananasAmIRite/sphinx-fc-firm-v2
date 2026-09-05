#ifndef __VL53L1X_DRV_H__
#define __VL53L1X_DRV_H__

#include <stdbool.h>
#include <stdint.h>
#include "sphinx_io.h"

// entrypoint into time-of-flight driver, with abstractions for initialization and reading

int8_t tof_config(); 

int8_t tof_read_altitude(AltimeterReadings* readings); 

#endif