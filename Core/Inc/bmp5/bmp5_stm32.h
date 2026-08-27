#ifndef _BMP5_STM32_H_
#define _BMP5_STM32_H_

#include "bmp5_defs.h"

#include "stm32h7xx_hal.h"

#ifdef FREERTOS_ENABLED
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#endif

I2C_HandleTypeDef *_bmp5_i2c_port;

void bmp5_assignI2C(I2C_HandleTypeDef *hi2c_device) {
  _bmp5_i2c_port = hi2c_device;
}

BMP5_INTF_RET_TYPE bmp5_read(uint8_t reg_addr, uint8_t *read_data, uint32_t len, void *intf_ptr) {
  HAL_StatusTypeDef status;
  
  status = HAL_I2C_Master_Transmit(_bmp5_i2c_port, BMP5_I2C_ADDR << 1, &reg_addr, 1,
                          100);

    if (status != HAL_OK) return 1;
  HAL_I2C_Master_Receive(_bmp5_i2c_port, BMP5_I2C_ADDR << 1, read_data, len,
                         100);

    if (status != HAL_OK) return 1;


    return BMP5_INTF_RET_SUCCESS; 
}


BMP5_INTF_RET_TYPE bmp5_write(uint8_t reg_addr, const uint8_t *read_data, uint32_t len, void *intf_ptr) {

    uint8_t txdata[1 + len];
    txdata[0] = reg_addr; 
    for (uint8_t i = 0; i < len; i++) {
        txdata[1 + i] = read_data[i];
    }

    uint8_t status = HAL_I2C_Master_Transmit(_bmp5_i2c_port, BMP5_I2C_ADDR << 1,
                                            txdata, sizeof(txdata), 10);
    if (status != HAL_OK) {
        return 1; 
    }

    return BMP5_INTF_RET_SUCCESS; 
}

/*! Delay function pointer */
void bmp5_delay(uint32_t period, void *intf_ptr) {
    #ifdef FREERTOS_ENABLED
    osDelay(period);
    #else
    HAL_Delay(period / 1000);
    #endif
}
#endif