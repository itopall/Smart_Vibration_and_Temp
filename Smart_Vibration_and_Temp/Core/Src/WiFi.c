/*
 * WiFi.c
 *
 *  Created on: Mar 11, 2025
 *      Author: Ilker
 */

#include "WiFi.h"
#include <stdio.h>
#include <string.h>

static WiFi_Status_t _sendATCommand(WiFi_Config_t *config, const char *cmd, const char *expected_response)
{
    char response[256] = {0};

    UART_SendString(&config->uart_config, (char*)cmd);

    HAL_StatusTypeDef status = UART_ReceiveString(&config->uart_config, response, sizeof(response));
    if (status != HAL_OK) return WIFI_ERROR_TIMEOUT;

    if (strstr(response, expected_response) == NULL) {
        return WIFI_ERROR_NO_RESPONSE;
    }

    return WIFI_OK;
}


WiFi_Status_t WiFi_Init(WiFi_Config_t *config)
{
    WiFi_Status_t status;

    status = _sendATCommand(config, "AT\r\n", "OK");
    if (status != WIFI_OK) return status;

    // Wi-Fi Modunu Ayarla (STA Mod)
    status = _sendATCommand(config, "AT+CWMODE=1\r\n", "OK");
    if (status != WIFI_OK) return status;

    // Wi-Fi'ye Bağlan
    char connect_cmd[128];
    snprintf(connect_cmd, sizeof(connect_cmd), "AT+CWJAP=\"%s\",\"%s\"\r\n", config->ssid, config->password);
    status = _sendATCommand(config, connect_cmd, "OK");
    if (status != WIFI_OK) return WIFI_ERROR_CONNECTION_FAILED;

    return WIFI_OK;
}

WiFi_Status_t WiFi_SendHTTPRequest(WiFi_Config_t *config, const char *server, const char *endpoint, const char *data)
{
    char cmd[256];
    WiFi_Status_t status;

    // TCP Bağlantısı Aç
    snprintf(cmd, sizeof(cmd), "AT+CIPSTART=\"TCP\",\"%s\",80\r\n", server);
    status = _sendATCommand(config, cmd, "OK");
    if (status != WIFI_OK) return WIFI_ERROR_TCP_CONNECTION;

    // Veri Uzunluğunu Ayarla
    int data_len = strlen(data);
    snprintf(cmd, sizeof(cmd), "AT+CIPSEND=%d\r\n", data_len);
    status = _sendATCommand(config, cmd, ">");
    if (status != WIFI_OK) return WIFI_ERROR_SEND_FAILED;

    // Veriyi Gönder
    status = _sendATCommand(config, data, "SEND OK");
    if (status != WIFI_OK) return WIFI_ERROR_SEND_FAILED;

    // Bağlantıyı Kapat
    _sendATCommand(config, "AT+CIPCLOSE\r\n", "OK");

    return WIFI_OK;
}
