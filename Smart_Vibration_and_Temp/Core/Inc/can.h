/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.h
  * @brief   This file contains all the function prototypes for
  *          the can.c file
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
#ifndef __CAN_H__
#define __CAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"


extern CAN_HandleTypeDef hcan1;

typedef struct {
    CAN_HandleTypeDef *hcan;      // STM32 CAN Handler
    uint32_t tx_mailbox;          // Tx Mailbox (0, 1, 2)
    uint32_t rx_fifo;             // Rx FIFO (CAN_RX_FIFO0 veya CAN_RX_FIFO1)
    uint32_t baudrate;            // Baudrate (Örn: 500000)
} CAN_Config_t;

typedef enum {
    CAN_OK = 0,
    CAN_ERROR_INIT,
    CAN_ERROR_TX,
    CAN_ERROR_RX_TIMEOUT,
    CAN_ERROR_FILTER
} CAN_Status_t;

void MX_CAN1_Init(void);



// Prototipler
CAN_Status_t CAN_Init(CAN_Config_t *config);
CAN_Status_t CAN_SendData(CAN_Config_t *config, uint32_t id, uint8_t *data, uint8_t len);
CAN_Status_t CAN_ReceiveData(CAN_Config_t *config, uint32_t *id, uint8_t *data, uint8_t *len);


#ifdef __cplusplus
}
#endif

#endif /* __CAN_H__ */

