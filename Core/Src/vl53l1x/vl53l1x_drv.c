#include "vl53l1x_drv.h"
#include "vl53l1x_tof.h"

extern I2C_HandleTypeDef hi2c1;

int8_t tof_config() {
    return !tof_init(&hi2c1);  // 0 = good, 1 = bad :(
}

int8_t tof_read_altitude(AltimeterReadings* readings) {
    int16_t dist = tof_read_distance(); // distance in mm

    if (dist == -1) {
        // invalid distance, probably too far away
        readings->altitude_tof_available = false; 
        readings->altitude_tof_meters = -1; 
        return -1; 
    } else {
        readings->altitude_tof_meters = ((double) dist) / 1000; // convert to meters 
        readings->altitude_tof_available = true; 
        return 0; 
    }
}