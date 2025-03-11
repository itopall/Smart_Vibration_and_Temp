/*
 * WiFi.h
 *
 *  Created on: Mar 11, 2025
 *      Author: Ilker
 */

#ifndef INC_WIFI_H_
#define INC_WIFI_H_

#include "usart.h"

typedef struct {
    UART_Config_t uart_config;
    char ssid[32];
    char password[64];
    uint32_t response_timeout;
} WiFi_Config_t;

typedef enum {
    WIFI_OK = 0,
    WIFI_ERROR_TIMEOUT,
    WIFI_ERROR_NO_RESPONSE,
    WIFI_ERROR_CONNECTION_FAILED,
    WIFI_ERROR_TCP_CONNECTION,
    WIFI_ERROR_SEND_FAILED
} WiFi_Status_t;

WiFi_Status_t WiFi_Init(WiFi_Config_t *config);
WiFi_Status_t WiFi_SendHTTPRequest(WiFi_Config_t *config, const char *server, const char *endpoint, const char *data);

#endif /* INC_WIFI_H_ */
