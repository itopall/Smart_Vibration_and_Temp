/*
 * modbus.c
 *
 *  Created on: Mar 11, 2025
 *      Author: Ilker
 */
#include "modbus.h"

Modbus_Status_t Modbus_ReadHoldingRegisters(Modbus_Config_t *config, uint16_t start_addr, uint16_t num_regs, uint16_t *data)
{
	uint8_t tx_buffer[8];
	uint8_t rx_buffer[256];

	// Talep Paketi Oluştur
	tx_buffer[0] = MODBUS_SLAVE_ADDRESS;
	tx_buffer[1] = MODBUS_READ_HOLDING_REGISTERS;
	tx_buffer[2] = (start_addr >> 8) & 0xFF; // High byte
	tx_buffer[3] = start_addr & 0xFF;        // Low byte
	tx_buffer[4] = (num_regs >> 8) & 0xFF;   // High byte
	tx_buffer[5] = num_regs & 0xFF;          // Low byte

	// CRC Ekle
	uint16_t crc = Modbus_CRC16(tx_buffer, 6);
	tx_buffer[6] = crc & 0xFF;
	tx_buffer[7] = (crc >> 8) & 0xFF;

	// RS485 Gönderim Modu
	HAL_GPIO_WritePin(config->de_port, config->de_pin, GPIO_PIN_SET);
	HAL_UART_Transmit(config->huart, tx_buffer, 8, config->timeout);
	HAL_GPIO_WritePin(config->de_port, config->de_pin, GPIO_PIN_RESET);

	// Yanıtı Al
	uint8_t rx_len = 5 + (2 * num_regs); // Slave Adres (1) + FC (1) + Byte Count (1) + Data (2*N) + CRC (2)
	HAL_StatusTypeDef status = HAL_UART_Receive(config->huart, rx_buffer, rx_len, config->timeout);
	if (status != HAL_OK) return MODBUS_TIMEOUT;

	// CRC Kontrolü
	crc = Modbus_CRC16(rx_buffer, rx_len - 2);
	if (rx_buffer[rx_len - 2] != (crc & 0xFF) || rx_buffer[rx_len - 1] != ((crc >> 8) & 0xFF)) {
		return MODBUS_CRC_ERROR;
	}

	// Veriyi Ayrıştır
	for (int i = 0; i < num_regs; i++) {
		data[i] = (rx_buffer[3 + 2*i] << 8) | rx_buffer[4 + 2*i];
	}

	return MODBUS_OK;
}

Modbus_Status_t Modbus_WriteSingleRegister(Modbus_Config_t *config, uint16_t reg_addr, uint16_t value)
{
	uint8_t tx_buffer[8];
	uint8_t rx_buffer[8];

	// Talep Paketi Oluştur
	tx_buffer[0] = MODBUS_SLAVE_ADDRESS;
	tx_buffer[1] = MODBUS_WRITE_SINGLE_REGISTER;
	tx_buffer[2] = (reg_addr >> 8) & 0xFF; // High byte
	tx_buffer[3] = reg_addr & 0xFF;        // Low byte
	tx_buffer[4] = (value >> 8) & 0xFF;    // High byte
	tx_buffer[5] = value & 0xFF;           // Low byte

	// CRC Ekle
	uint16_t crc = Modbus_CRC16(tx_buffer, 6);
	tx_buffer[6] = crc & 0xFF;
	tx_buffer[7] = (crc >> 8) & 0xFF;

	// RS485 Gönderim Modu
	HAL_GPIO_WritePin(config->de_port, config->de_pin, GPIO_PIN_SET);
	HAL_UART_Transmit(config->huart, tx_buffer, 8, config->timeout);
	HAL_GPIO_WritePin(config->de_port, config->de_pin, GPIO_PIN_RESET);

	// Yanıtı Al (Echo kontrolü)
	HAL_StatusTypeDef status = HAL_UART_Receive(config->huart, rx_buffer, 8, config->timeout);
	if (status != HAL_OK) return MODBUS_TIMEOUT;

	// Yanıtı Doğrula
	if (memcmp(tx_buffer, rx_buffer, 8) != 0) {
		return MODBUS_INVALID_RESPONSE;
	}

	return MODBUS_OK;

}

uint16_t Modbus_CRC16(uint8_t *data, uint16_t len)
{
	uint16_t crc = 0xFFFF;
	for (int i = 0; i < len; i++)
	{
		crc ^= data[i];
		for (int j = 0; j < 8; j++)
		{
			if (crc & 0x0001)
			{
				crc = (crc >> 1) ^ 0xA001;
			}
			else
			{
				crc >>= 1;
			}
		}
	}
	return crc;
}
