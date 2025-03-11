/*
 * adxl357.c
 *
 *  Created on: Mar 9, 2025
 *      Author: Ilker
 */

#include "ADXL357.h"



void ADXL357_Init(ADXL357_Config_t *dev)
{
  // SPI Konfigürasyonu (dışarıdan verilir)
  SPI_WriteReg(&dev->spi_config, ADXL357_POWER_CTL, 0x02); // Measure Mode
  SPI_WriteReg(&dev->spi_config, ADXL357_RANGE, 0x03);     // ±40g
}

HAL_StatusTypeDef ADXL357_ReadXYZ(ADXL357_Config_t *dev, float *x, float *y, float *z)
{
  uint8_t rx_data[6];
  HAL_StatusTypeDef status = SPI_ReadMulti(&dev->spi_config, ADXL357_DATA_X0, rx_data, 6);

  if (status == HAL_OK)
  {
    int32_t raw_x = (rx_data[1] << 12) | (rx_data[0] << 4);
    int32_t raw_y = (rx_data[3] << 12) | (rx_data[2] << 4);
    *x = raw_x * dev->scale_factor;
    *y = raw_y * dev->scale_factor;
  }
  return status;
}
