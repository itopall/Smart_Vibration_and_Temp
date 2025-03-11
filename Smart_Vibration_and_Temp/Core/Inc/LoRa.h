/*
 * LoRa.h
 *
 *  Created on: Mar 11, 2025
 *      Author: Ilker
 */

#ifndef INC_LORA_H_
#define INC_LORA_H_

#include "spi.h"

#define REG_OP_MODE         0x01
#define REG_FRF_MSB         0x06
#define REG_FRF_MID         0x07
#define REG_FRF_LSB         0x08
#define REG_PA_CONFIG       0x09
#define REG_MODEM_CONFIG_1  0x1D
#define REG_MODEM_CONFIG_2  0x1E
#define REG_IRQ_FLAGS       0x12
#define REG_FIFO_ADDR_PTR   0x0D
#define REG_FIFO            0x00
#define REG_PAYLOAD_LENGTH  0x22

typedef struct {
	SPI_Config_t spi_config;
	float frequency;
	uint8_t spreading_factor;
	uint32_t bandwidth;
}LoRa_Config_t;

void LoRa_Init(LoRa_Config_t *dev);
HAL_StatusTypeDef LoRa_Send(LoRa_Config_t *dev, uint8_t *data, uint8_t len);

#endif /* INC_LORA_H_ */
