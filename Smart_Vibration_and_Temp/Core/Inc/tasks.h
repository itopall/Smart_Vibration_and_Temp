/*
 * tasks.h
 *
 *  Created on: Mar 11, 2025
 *      Author: Ilker
 */

#ifndef INC_TASKS_H_
#define INC_TASKS_H_

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "adxl357.h"
#include "MAX30205.h"
#include "can.h"
#include "WiFi.h"
#include "LoRa.h"
#include "modbus.h"
#include "gpio.h"


extern QueueHandle_t xSensorQueue;
extern EventGroupHandle_t xSystemEvents;


typedef struct {
    float vibration[3];  // X, Y, Z (g)
    float temperature;   // °C     // dps
} SensorData_t;

// Event bitleri
#define SENSOR_DATA_READY_BIT (1 << 0)
#define FAULT_DETECTED_BIT    (1 << 1)

void vVibrationTask(void *pvParameters);
void vTempTask(void *pvParameters);
void vCANBusTask(void *pvParameters);
void vLoRaTask(void *pvParameters);
void vWiFiTask(void *pvParameters);

#endif /* INC_TASKS_H_ */
