/*
 * adxl357.h
 *
 *  Created on: Mar 9, 2025
 *      Author: Ilker
 */

#ifndef INC_ADXL357_H_
#define INC_ADXL357_H_

#include "spi.h"

#define ADXL357_POWER_CTL  0x2D
#define ADXL357_RANGE      0x2C
#define ADXL357_DATA_X0    0x08

typedef struct {
	SPI_Config_t spi_config;
	float scale_factor;
}ADXL357_Config_t;

void ADXL357_Init(ADXL357_Config_t *dev);
HAL_StatusTypeDef ADXL357_ReadXYZ(ADXL357_Config_t *dev, float *x, float *y, float *z);

#endif /* INC_ADXL357_H_ */
