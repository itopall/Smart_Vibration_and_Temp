/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.h
  * @brief   This file contains all the function prototypes for
  *          the i2c.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

extern I2C_HandleTypeDef hi2c1;

typedef struct {
	I2C_HandleTypeDef *hi2c;
	uint16_t dev_address;
	uint32_t timeout;
}I2C_Config_t;

void MX_I2C1_Init(void);

HAL_StatusTypeDef I2C_WriteReg(I2C_Config_t *config, uint8_t reg, uint8_t data);
HAL_StatusTypeDef I2C_ReadReg(I2C_Config_t *config, uint8_t reg, uint8_t *data, uint8_t len);

HAL_StatusTypeDef I2C_WriteMulti(I2C_Config_t *config, uint8_t *tx_data, uint8_t len);
HAL_StatusTypeDef I2C_ReadMulti(I2C_Config_t *config, uint8_t reg, uint8_t *rx_data, uint8_t len);


#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */

