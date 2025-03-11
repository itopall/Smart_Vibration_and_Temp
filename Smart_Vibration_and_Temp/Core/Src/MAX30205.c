/*
 * MAX30205.c
 *
 *  Created on: Mar 9, 2025
 *      Author: Ilker
 */


#include "MAX30205.h"


void MAX30205_Init(MAX30205_t *dev)
{
  I2C_WriteReg(&dev->i2c_config, MAX30205_CONFIG, 0x40); // One-Shot Mode
}

HAL_StatusTypeDef MAX30205_ReadTemp(MAX30205_t *dev, float *temp)
{
  uint8_t data[2];
  HAL_StatusTypeDef status = I2C_ReadReg(&dev->i2c_config, MAX30205_TEMP, data, 2);

  if (status == HAL_OK)
  {
    uint16_t raw = (data[0] << 8) | data[1];
    *temp = raw * 0.00390625f; // 16-bit → °C
  }
  return status;
}
