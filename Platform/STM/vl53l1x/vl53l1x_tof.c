#include "vl53l1x_tof.h"

uint8_t _shutdown_pin = -1;
uint8_t _irq_pin = -1; 


VL53L1X_ERROR _InitSensor()
{
   VL53L1X_ERROR status = 0;
   uint8_t sensorState = 0;

   #ifdef DEBUG_MODE
         uint8_t byteData;
         uint16_t wordData;
         status = VL53L1X_RdByte(Device, 0x010F, &byteData);
         Serial.println("VL53L1X Model_ID: " + String(byteData));
         status = VL53L1X_RdByte(Device, 0x0110, &byteData);
         Serial.println("VL53L1X Module_Type: " + String(byteData));
         status = VL53L1X_RdWord(Device, 0x010F, &wordData);
         Serial.println("VL53L1X: " + String(wordData));
   #endif


   while (!sensorState && !status)
   {
      status = VL53L1X_BootState(&sensorState);
      tof_delay(2); 
   }
   if(!status)
   {
      status = VL53L1X_SensorInit();
   }
   return status;
}

bool tof_init(I2C_HandleTypeDef *hi2c_device
  // , uint8_t shutdown_pin, uint8_t irq_pin
) {
  // _s)hutdown_pin = shutdown_pin;
  // _irq_pin = irq_pin;
  tof_assignI2C(hi2c_device); 

  tof_delay(5);


  VL53L1X_Error vl_status = _InitSensor();
  if (vl_status != VL53L1X_ERROR_NONE) {
    return false;
  }


  tof_sensor_id_t sensor_id = tof_sensor_id(); 
  
  if (sensor_id.model_id != TOF_MODEL_ID || sensor_id.model_type != TOF_MODEL_TYPE) {
      printf("Can't find VL53L1X TOF, model_id: 0x%02x, model_type: 0x%02x. Please check your wiring.\r\n", sensor_id.model_id, sensor_id.model_type);
      return false;
  }

  return true; 
}


tof_sensor_id_t tof_sensor_id() {
  uint16_t out; 
  VL53L1X_GetSensorId(&out);
  
  tof_sensor_id_t ids; 
  ids.model_id = (out >> 8) & 0xFF; 
  ids.model_type = out & 0xFF; 

  return ids; 
}

bool tof_start_ranging() {
  VL53L1X_ERROR vl_status = VL53L1X_StartRanging();
  return (vl_status == VL53L1X_ERROR_NONE);
}

bool tof_stop_ranging() {
  VL53L1X_ERROR vl_status = VL53L1X_StopRanging();
  return (vl_status == VL53L1X_ERROR_NONE);
}

bool tof_set_timing_budget(uint16_t ms) {
  VL53L1X_ERROR vl_status = VL53L1X_SetTimingBudgetInMs(ms);
  return (vl_status == VL53L1X_ERROR_NONE);
}

uint16_t tof_get_timing_budget() {
  uint16_t ms = 0;

  VL53L1X_ERROR vl_status = VL53L1X_GetTimingBudgetInMs(&ms);
  if (vl_status == VL53L1X_ERROR_NONE) {
    return ms;
  }
  return 0;
} 

bool tof_data_ready() {
  uint8_t x = 0;
  VL53L1X_CheckForDataReady(&x);
  return (bool)x;
} 

int16_t tof_read_distance() {
  uint16_t distance;
  uint8_t rangeStatus;

  VL53L1X_Error vl_status = VL53L1X_GetRangeStatus(&rangeStatus);
  printf("%d", vl_status); 
  if ((vl_status != VL53L1X_ERROR_NONE) || (rangeStatus != 0x0)) {
    return -1;
  }

  vl_status = VL53L1X_GetDistance(&distance);
  printf("%d", vl_status); 
  if (vl_status != VL53L1X_ERROR_NONE) {
    return -1;
  }
  return (int16_t)distance;
}

bool tof_clear_interrupt() {
  VL53L1X_Error vl_status = VL53L1X_ClearInterrupt();
  return (vl_status == VL53L1X_ERROR_NONE);
}

bool tof_set_int_polarity(bool polarity) {
  VL53L1X_Error vl_status = VL53L1X_SetInterruptPolarity(polarity);
  return (vl_status == VL53L1X_ERROR_NONE);
}
bool tof_get_int_polarity() {
  uint8_t x = 0;
  VL53L1X_GetInterruptPolarity(&x);
  return (bool)x;
}
