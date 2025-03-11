/*
 * MAX30205.h
 *
 *  Created on: Mar 9, 2025
 *      Author: Ilker
 */

#ifndef INC_MAX30205_H_
#define INC_MAX30205_H_

#include "i2c.h"

#define MAX30205_TEMP     0x00
#define MAX30205_CONFIG   0x01



typedef struct {
  I2C_Config_t i2c_config;
} MAX30205_t;

void MAX30205_Init(MAX30205_t *dev);
HAL_StatusTypeDef MAX30205_ReadTemp(MAX30205_t *dev, float *temp);

#endif /* INC_MAX30205_H_ */
