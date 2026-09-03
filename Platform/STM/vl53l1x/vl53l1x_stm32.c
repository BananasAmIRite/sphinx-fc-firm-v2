#include "vl53l1x_stm32.h"

I2C_HandleTypeDef *_tof_i2c_port;

void tof_assignI2C(I2C_HandleTypeDef *hi2c_device) {
  _tof_i2c_port = hi2c_device;
}


void tof_delay(int time) {
#ifdef FREERTOS_ENABLED
  osDelay(time);
#else
  HAL_Delay(time);
#endif
}

bool tof_write(uint16_t reg, uint8_t *data, uint8_t len) {
  uint8_t txdata[2 + len];
  txdata[0] = (reg >> 8) & 0xFF;
  txdata[1] = reg & 0xFF;
  for (uint8_t i = 0; i < len; i++) {
    txdata[2 + i] = data[i];
  }

  uint8_t status = HAL_I2C_Master_Transmit(_tof_i2c_port, TOF_I2C_ADDR << 1,
                                           txdata, sizeof(txdata), 10);
  if (status != HAL_OK) {
    tof_log_i2c_status(_tof_i2c_port, status);
    return false; 
  }

  return true; 
}
// bool tof_write(uint16_t reg, uint8_t *data, uint8_t len) {
//   if (HAL_I2C_Mem_Write(_tof_i2c_port,
//                         TOF_I2C_ADDR << 1,
//                         reg,
//                         I2C_MEMADD_SIZE_16BIT,
//                         data,
//                         len,
//                         100) != HAL_OK) {
//     return false;
//   }
//   return true;
// }

bool tof_read(uint16_t reg, uint8_t *data, uint8_t len) {

  uint8_t status_t, status_r; 

  uint8_t reg_split[2] = {(reg >> 8) & 0xFF, reg & 0xFF};
  status_t = HAL_I2C_Master_Transmit(_tof_i2c_port, TOF_I2C_ADDR << 1, reg_split, sizeof(reg_split),
                          100);
  status_r = HAL_I2C_Master_Receive(_tof_i2c_port, TOF_I2C_ADDR << 1, data, len,
                         100);

  return status_t == HAL_OK && status_r == HAL_OK; 
}
// bool tof_read(uint16_t reg, uint8_t *data, uint8_t len) {
//   if (HAL_I2C_Mem_Read(_tof_i2c_port,
//                        TOF_I2C_ADDR << 1,
//                        reg,
//                        I2C_MEMADD_SIZE_16BIT,
//                        data,
//                        len,
//                        100) != HAL_OK) {
//     return false;
//   }
//   return true;
// }
