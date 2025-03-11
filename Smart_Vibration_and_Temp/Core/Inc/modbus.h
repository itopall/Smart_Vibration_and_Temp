/*
 * modbus.h
 *
 *  Created on: Mar 11, 2025
 *      Author: Ilker
 */

#ifndef INC_MODBUS_H_
#define INC_MODBUS_H_

#include "gpio.h"
#include "usart.h"
#include <stdint.h>
#include <string.h>

#define MODBUS_SLAVE_ADDRESS 0x01

#define MODBUS_READ_HOLDING_REGISTERS 0x03
#define MODBUS_WRITE_SINGLE_REGISTER 0x06

// Hata Kodları
typedef enum {
    MODBUS_OK = 0,
    MODBUS_CRC_ERROR,
    MODBUS_TIMEOUT,
    MODBUS_INVALID_RESPONSE
} Modbus_Status_t;

typedef struct {
    UART_HandleTypeDef *huart;   // UART Handler
    GPIO_TypeDef *de_port;       // DE Pin Port (GPIOA, GPIOB, vb.)
    uint16_t de_pin;             // DE Pin
    uint32_t timeout;            // Timeout (ms)
} Modbus_Config_t;

Modbus_Status_t Modbus_ReadHoldingRegisters(Modbus_Config_t *config, uint16_t start_addr, uint16_t num_regs, uint16_t *data);
Modbus_Status_t Modbus_WriteSingleRegister(Modbus_Config_t *config, uint16_t reg_addr, uint16_t value);
uint16_t Modbus_CRC16(uint8_t *data, uint16_t len);

#endif /* INC_MODBUS_H_ */
