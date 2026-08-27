#include "bmp5_drv.h"
#include "bmp5.h"
#include "bmp5_stm32.h"
#include "sphinx_io.h"
#include <stdint.h>

bmp5_dev bmp5_io; 
struct bmp5_osr_odr_press_config bmp5_osr_odr_config; 
struct bmp5_sensor_data bmp_sensor_data; 
volatile double bmp5_altitude;


#define BMP5_CAL_PTS 10
double bmp5_cal_zero = 0; 



int8_t bmp5_calibrate()
{
    double altitude_sum = 0.0;

    AltimeterReadings readings = {0}; 

    for (uint8_t i = 0; i < BMP5_CAL_PTS; i++) {

        int8_t status = bmp5_read_altitude(&readings);
        if (status != BMP5_OK) return status; 
        altitude_sum += readings.altitude_meters; 

        HAL_Delay(10);
    }

    bmp5_cal_zero = altitude_sum / BMP5_CAL_PTS;

    return 0;
}


int8_t bmp5_config() {
    bmp5_io.read = bmp5_read; 
    bmp5_io.write = bmp5_write; 
    bmp5_io.delay_us = bmp5_delay;
    bmp5_io.intf = BMP5_I2C_INTF; 

    int8_t status; 
    HAL_Delay(100);
    status = bmp5_init(&bmp5_io); 

    if (status == BMP5_OK) {
        bmp5_osr_odr_config.odr = BMP5_ODR_100_2_HZ;
        bmp5_osr_odr_config.press_en = true;  
        bmp5_osr_odr_config.osr_p = BMP5_OVERSAMPLING_1X; 
        bmp5_osr_odr_config.osr_t = BMP5_OVERSAMPLING_1X; 
        status = bmp5_set_osr_odr_press_config(&bmp5_osr_odr_config, &bmp5_io); 
        if (status == BMP5_OK) {
            status = bmp5_set_power_mode(BMP5_POWERMODE_NORMAL, &bmp5_io);
        }
        return bmp5_calibrate(); 
    } else {
        return status; 
    }
}



int8_t bmp5_read_altitude(AltimeterReadings* readings) {
    int8_t status = bmp5_get_sensor_data(&bmp_sensor_data, &bmp5_osr_odr_config, &bmp5_io);
    
    if (status != 0) return status; 
    // convert pressure and temperature -> altitude
    // this code uses the hypsometric formula
    // bmp5_altitude = (
    //     (pow(
    //         (101325.0 / bmp_sensor_data.pressure),
    //         0.1902
    //     )-1)
    //     * (bmp_sensor_data.temperature + 273.15)
    // ) / 0.0065;
    
    // this code uses barometric formula
    bmp5_altitude = 44330.0 * (1.0 - pow((bmp_sensor_data.pressure / 101325.0), 0.1902));

    readings->altitude_meters = bmp5_altitude - bmp5_cal_zero; 

    return 0; 
}
