#ifndef __BMP5_DRV_H__
#define __BMP5_DRV_H__

#include <stdbool.h>
#include <stdint.h>
#include "sphinx_io.h"

// entrypoint into bmp5 driver, with abstractions for initialization and reading

int8_t bmp5_config(); 

int8_t bmp5_read_altitude(AltimeterReadings* readings); 

#endif