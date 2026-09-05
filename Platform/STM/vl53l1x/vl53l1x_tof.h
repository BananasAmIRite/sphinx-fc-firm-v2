#ifndef VL53L1X_TOF_H_
#define VL53L1X_TOF_H_

#ifdef __cplusplus
  extern "C" {
#endif
// #define FREERTOS_ENABLED true

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "vl53l1x.h"

typedef struct {
    uint8_t model_id; 
    uint8_t model_type; 
} tof_sensor_id_t; 

bool tof_init(I2C_HandleTypeDef *hi2c_device
  // , uint8_t shutdown_pin, uint8_t irq_pin
);
tof_sensor_id_t tof_sensor_id();
bool tof_start_ranging();
bool tof_stop_ranging();
bool tof_set_timing_budget(uint16_t ms);
uint16_t tof_get_timing_budget();
bool tof_data_ready();
int16_t tof_read_distance();
bool tof_clear_interrupt();
bool tof_set_int_polarity(bool polarity);
bool tof_get_int_polarity();



#ifdef __cplusplus
  }
#endif
#endif  // VL53L1X_TOF_H_
