/*
 * LoRa.h
 *
 *  Created on: Mar 11, 2025
 *      Author: Ilker
 */
#include "LoRa.h"
#include <string.h>

void LoRa_Init(LoRa_Config_t *dev)
{
	uint8_t mode = 0x80 | 0x01; // LoRa + Standby
	SPI_WriteReg(&dev->spi_config, REG_OP_MODE, mode);
	HAL_Delay(10);

	uint64_t frf = (uint64_t)(dev->frequency * 1000000) / 61.03515625;
	uint8_t freq[3] = {
		(frf >> 16) & 0xFF,
		(frf >> 8) & 0xFF,
		frf & 0xFF
	};

	SPI_WriteReg(&dev->spi_config, REG_FRF_MSB, freq[0]);
	SPI_WriteReg(&dev->spi_config, REG_FRF_MID, freq[1]);
	SPI_WriteReg(&dev->spi_config, REG_FRF_LSB, freq[2]);

	uint8_t bw = (dev->bandwidth == 125000) ? 0x72 : 0x74; // BW=125kHz, CR=4/5
	uint8_t sf = (dev->spreading_factor << 4) | 0x04;       // SF, CRC enable
	SPI_WriteReg(&dev->spi_config, REG_MODEM_CONFIG_1, bw);
	SPI_WriteReg(&dev->spi_config, REG_MODEM_CONFIG_2, sf);

	SPI_WriteReg(&dev->spi_config, REG_PA_CONFIG, 0xFF);
}

HAL_StatusTypeDef LoRa_Send(LoRa_Config_t *dev, uint8_t *data, uint8_t len)
{
	uint8_t tx_buffer[len+1];
	SPI_WriteReg(&dev->spi_config, REG_FIFO_ADDR_PTR, 0x00);

	tx_buffer[0] = REG_FIFO | 0x80;
	memcpy(&tx_buffer[1], data, len);
	SPI_WriteMulti(&dev->spi_config, tx_buffer, len + 1);
	SPI_WriteReg(&dev->spi_config, REG_PAYLOAD_LENGTH, len);

	SPI_WriteReg(&dev->spi_config, REG_OP_MODE, 0x83);

	uint8_t irq_flags;
	do {
		SPI_ReadReg(&dev->spi_config, REG_IRQ_FLAGS, &irq_flags, 1);
	} while (!(irq_flags & 0x08));

	SPI_WriteReg(&dev->spi_config, REG_IRQ_FLAGS, 0xFF);
	return HAL_OK;
}
