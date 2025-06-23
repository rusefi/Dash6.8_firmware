/*
 * usart_status.c
 *
 *  Created on: May 4, 2025
 *      Author: Raman
 */
#include "usart3_json_dma.h"
#include <string.h>

#include "extern.h"

// Структура для управления состоянием
typedef struct {
    // Отправка
    const char* tx_current_json;
    size_t tx_json_len;
    size_t tx_bytes_sent;
    uint8_t tx_packet_buf[PACKET_MAX_SIZE + 2];
    volatile uint8_t tx_busy;

    // Приём
    uint8_t rx_dma_buf[JSON_RX_DMA_BUF_SIZE];
    uint8_t rx_json_buf[JSON_RX_BUF_SIZE];
    size_t rx_json_idx;
    uint8_t rx_receiving;
    json_rx_callback_t rx_callback;
} USART3_JSON_HandleTypeDef;

static USART3_JSON_HandleTypeDef hj = {0};

void USART3_JSON_Init(UART_HandleTypeDef *huart, uint32_t baudrate, json_rx_callback_t cb) {
    // Настройка USART3
    huart->Instance = USART3;
    huart->Init.BaudRate = baudrate;
    huart->Init.WordLength = UART_WORDLENGTH_8B;
    huart->Init.StopBits = UART_STOPBITS_1;
    huart->Init.Parity = UART_PARITY_NONE;
    huart->Init.Mode = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart->Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(huart);

    // Инициализация приёма DMA
    HAL_UARTEx_ReceiveToIdle_DMA(huart, hj.rx_dma_buf, JSON_RX_DMA_BUF_SIZE);
    __HAL_DMA_DISABLE_IT(huart->hdmarx, DMA_IT_HT);

    // Колбэк для приёма
    hj.rx_callback = cb;
}

// Отправка (существующий код с оптимизацией)
static HAL_StatusTypeDef send_next_packet(UART_HandleTypeDef *huart) {
    if (hj.tx_bytes_sent >= hj.tx_json_len) {
        hj.tx_busy = 0;
        return HAL_OK;
    }

    size_t chunk_size = (hj.tx_json_len - hj.tx_bytes_sent > PACKET_MAX_SIZE) ?
                       PACKET_MAX_SIZE : (hj.tx_json_len - hj.tx_bytes_sent);

    size_t buf_offset = 0;
    if (hj.tx_bytes_sent == 0) {
        hj.tx_packet_buf[buf_offset++] = PACKET_START_MARKER;
    }

    memcpy(&hj.tx_packet_buf[buf_offset], &hj.tx_current_json[hj.tx_bytes_sent], chunk_size);
    buf_offset += chunk_size;

    if ((hj.tx_bytes_sent + chunk_size) >= hj.tx_json_len) {
        hj.tx_packet_buf[buf_offset++] = PACKET_END_MARKER;
    }

    HAL_StatusTypeDef status = HAL_UART_Transmit_DMA(huart, hj.tx_packet_buf, buf_offset);
    if (status == HAL_OK) {
        hj.tx_bytes_sent += chunk_size;
    } else {
        hj.tx_busy = 0;
    }
    return status;
}

HAL_StatusTypeDef USART3_JSON_Send(const char* json_str) {
    if (hj.tx_busy || !json_str) return HAL_BUSY;

    hj.tx_current_json = json_str;
    hj.tx_json_len = strlen(json_str);
    hj.tx_bytes_sent = 0;
    hj.tx_busy = 1;

    return send_next_packet(&huart3);
}

void USART3_JSON_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if (huart->Instance != USART3) return;

    for (uint16_t i = 0; i < Size; ++i) {
        uint8_t byte = hj.rx_dma_buf[i];

        if (!hj.rx_receiving) {
            if (byte == PACKET_START_MARKER) {
                hj.rx_receiving = 1;
                hj.rx_json_idx = 0;
            }
            continue;
        }

        if (byte == PACKET_END_MARKER) {
            hj.rx_json_buf[(hj.rx_json_idx < JSON_RX_BUF_SIZE) ? hj.rx_json_idx : (JSON_RX_BUF_SIZE-1)] = '\0';

            if (hj.rx_callback) {
                hj.rx_callback((char*)hj.rx_json_buf); // В callback копируйте строку!
            }
            hj.rx_receiving = 0;
            continue;
        }

        if (hj.rx_json_idx < JSON_RX_BUF_SIZE-1) {
            hj.rx_json_buf[hj.rx_json_idx++] = byte;
        } else {
            hj.rx_receiving = 0; // Переполнение буфера
            // Можно добавить логирование
        }
    }

    HAL_UARTEx_ReceiveToIdle_DMA(huart, hj.rx_dma_buf, JSON_RX_DMA_BUF_SIZE);
}

// Остальные функции
uint8_t USART3_JSON_IsTxBusy(void) { return hj.tx_busy; }

void USART3_JSON_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART3) {
        send_next_packet(huart);
    }
}

void send_json_over_uart(char* json)
{
    if (!json) return;

    // Ждём, пока UART не освободится (максимум 100 попыток по 1 мс)
    uint32_t try_cnt = 0;
    while (USART3_JSON_IsTxBusy())
    {
        osDelay(1);
        if (++try_cnt > 100)
        {
            // UART завис? Освобождаем память и выходим
            free(json);
            return;
        }
    }

    // UART свободен, отправляем строку
    if (USART3_JSON_Send(json) == HAL_OK)
    {
        // Можно добавить логирование успеха
    }
    else
    {
        // Ошибка отправки, можно залогировать
    }

    // Освобождаем память
    free(json);
}
