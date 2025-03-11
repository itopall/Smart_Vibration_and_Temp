/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.h
  * @brief   This file contains all the function prototypes for
  *          the spi.c file
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
#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"



extern SPI_HandleTypeDef hspi1;




typedef struct {
	SPI_HandleTypeDef *hspi;
	GPIO_TypeDef *cs_port;
	uint16_t cs_pin;
	uint32_t timeout;

}SPI_Config_t;

void MX_SPI1_Init(void);
HAL_StatusTypeDef SPI_WriteReg(SPI_Config_t *config, uint8_t reg, uint8_t data);
HAL_StatusTypeDef SPI_ReadReg(SPI_Config_t *config, uint8_t reg, uint8_t *data, uint8_t len);

HAL_StatusTypeDef SPI_WriteMulti(SPI_Config_t *config, uint8_t *tx_data, uint8_t len);
HAL_StatusTypeDef SPI_ReadMulti(SPI_Config_t *config, uint8_t reg, uint8_t *tx_data, uint8_t len);

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */

