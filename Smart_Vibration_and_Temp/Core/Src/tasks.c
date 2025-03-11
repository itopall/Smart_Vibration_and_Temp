/*
 * tasks.c
 *
 *  Created on: Mar 11, 2025
 *      Author: Ilker
 */

#include "tasks.h"

void vVibrationTask(void *pvParameters)
{
	SensorData_t sensor_data;
	ADXL357_Config_t vib_cfg =
	{
			.spi_config =
			{
		      .hspi = &hspi1,
		      .cs_port = GPIOA,
		      .cs_pin = GPIO_PIN_4,
		      .timeout = 100
		    },
			.scale_factor = 0.0000384f
	};
    ADXL357_Init(&vib_cfg);

    while(1)
    {
        ADXL357_ReadXYZ(&vib_cfg, &sensor_data.vibration[0] , &sensor_data.vibration[1], &sensor_data.vibration[2]);
        xQueueSend(xSensorQueue, &sensor_data, portMAX_DELAY);

        vTaskDelay(pdMS_TO_TICKS(10)); // 100 Hz
    }
}

void vTempTask(void *pvParameters)
{
	SensorData_t sensor_data;
	MAX30205_t temp_cfg = {
			.i2c_config =
			{
				.hi2c=&hi2c1,
				.dev_address = 0x48,
				.timeout = 100
			}
	};

    while(1)
    {
        MAX30205_ReadTemp(&temp_cfg, &sensor_data.temperature);

        xQueueSend(xSensorQueue, &sensor_data.temperature, portMAX_DELAY);

        vTaskDelay(pdMS_TO_TICKS(100)); // 10 Hz
    }
}

void vCANBusTask(void *pvParameters)
{
	SensorData_t sensor_data;
	CAN_Config_t can_cfg = {.hcan=&hcan1};
    CAN_Init(&can_cfg);
    uint8_t can_data[8];

    while(1) {
        // Titreşim verisini kuyruktan al
        xQueueReceive(xSensorQueue, &sensor_data.vibration, portMAX_DELAY);
		can_data[0] = (uint8_t)(sensor_data.vibration[0] * 100);
		can_data[1] = (uint8_t)(sensor_data.vibration[1] * 100);
		can_data[2] = (uint8_t)(sensor_data.vibration[2] * 100);
		xQueueReceive(xSensorQueue, &sensor_data.temperature, portMAX_DELAY);
		can_data[3] = (uint8_t)(sensor_data.temperature * 100);
		CAN_SendData(&can_cfg, 0x100, can_data, 8);

        vTaskDelay(pdMS_TO_TICKS(20)); // 50 Hz
    }
}

void vLoRaTask(void *pvParameters)
{
	SensorData_t sensor_data;
	LoRa_Config_t lora_cfg =
	{
			.spi_config =
			{
					.hspi = &hspi1,
					.cs_port = GPIOA,
					.cs_pin = GPIO_PIN_4,
					.timeout = 100
			},
			.frequency=868000000,
			.bandwidth = 125000,
			.spreading_factor = 12
	};
    LoRa_Init(&lora_cfg);
    uint8_t lora_data[8];
    while(1) {
        // Tüm sensör verilerini topla
        xQueueReceive(xSensorQueue, &sensor_data.vibration, portMAX_DELAY);
        lora_data[0] = (uint8_t)(sensor_data.vibration[0] * 100);
        lora_data[1] = (uint8_t)(sensor_data.vibration[1] * 100);
        lora_data[2] = (uint8_t)(sensor_data.vibration[2] * 100);
		xQueueReceive(xSensorQueue, &sensor_data.temperature, portMAX_DELAY);
		lora_data[3] = (uint8_t)(sensor_data.temperature * 100);

        // LoRa ile gönder
        LoRa_Send(&lora_cfg, lora_data, sizeof(lora_data));

        vTaskDelay(pdMS_TO_TICKS(1000)); // 1 Hz
    }
}

void vWiFiTask(void *pvParameters)
{
	SensorData_t sensor_data;
	WiFi_Config_t wifi_cfg =
    {
    		.uart_config =
    		{
    				.huart=&huart2,
					.timeout = 1000
    		},
    		.ssid="FactoryNet",
    		.password="12345678",
			.response_timeout = 1000
    };
	WiFi_Init(&wifi_cfg);
    char http_data[128];

    while(1) {

		xQueueReceive(xSensorQueue, &sensor_data.vibration, portMAX_DELAY);
		xQueueReceive(xSensorQueue, &sensor_data.temperature, portMAX_DELAY);
		snprintf(http_data, sizeof(http_data), "x: %.2f\r\ny: %.2f\r\nz: %.2f\r\ntemp=%.2f",
				sensor_data.vibration[0], sensor_data.vibration[1], sensor_data.vibration[2],sensor_data.temperature);
		WiFi_SendHTTPRequest(&wifi_cfg, "api.....", "/update", http_data);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
