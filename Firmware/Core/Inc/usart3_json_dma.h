#pragma once
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stddef.h>



// Конфигурация отправки
#define PACKET_START_MARKER '$'
#define PACKET_END_MARKER   '#'
#define PACKET_MAX_SIZE     64

// Конфигурация приёма
#define JSON_RX_DMA_BUF_SIZE 256
#define JSON_RX_BUF_SIZE     512

typedef void (*json_rx_callback_t)(const char* json);

// Инициализация
void USART3_JSON_Init(UART_HandleTypeDef *huart, uint32_t baudrate, json_rx_callback_t cb);

// Функции отправки
HAL_StatusTypeDef USART3_JSON_Send(const char* json_str);
uint8_t USART3_JSON_IsTxBusy(void);
void USART3_JSON_TxCpltCallback(UART_HandleTypeDef *huart);

// Обработка приёма
void USART3_JSON_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
//отправка строки json
void send_json_over_uart(char* json);
