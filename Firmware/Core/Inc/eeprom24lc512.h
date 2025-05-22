#pragma once

#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

/*-----------------------------------------------------------------------------
 * Структура конфигурации EEPROM 24LC512
 *-----------------------------------------------------------------------------
 * hi2c                  - указатель на используемый I2C
 * hcrc                  - указатель на CRC (может быть NULL, если не используется)
 * i2c_address           - 7-битный адрес EEPROM (сдвинутый << 1)
 * page_size             - размер страницы EEPROM (обычно 128 для 24LC512)
 * write_timeout         - таймаут на запись (мс)
 * read_timeout          - таймаут на чтение (мс)
 * motohours_main_addr   - основной адрес хранения моточасов
 * motohours_backup_addr - резервный адрес хранения моточасов
 * flags_main_addr       - основной адрес хранения флагов
 * flags_backup_addr     - резервный адрес хранения флагов
 * array_main_addr       - основной адрес хранения массива (16 байт)
 * array_backup_addr     - резервный адрес хранения массива
 *-----------------------------------------------------------------------------
 */
typedef struct
{
    I2C_HandleTypeDef* hi2c;           // I2C-устройство
    CRC_HandleTypeDef* hcrc;           // CRC-устройство (NULL, если не используется)
    uint16_t           i2c_address;    // 7-битный адрес (<<1)
    uint16_t           page_size;      // Размер страницы (128 для 24LC512)
    uint32_t           write_timeout;  // Таймаут записи (мс)
    uint32_t           read_timeout;   // Таймаут чтения (мс)
    uint16_t           motohours_main_addr;
    uint16_t           motohours_backup_addr;
    uint16_t           flags_main_addr;
    uint16_t           flags_backup_addr;
    uint16_t           array_main_addr;
    uint16_t           array_backup_addr;
} EEPROM_Config_t;

/*-----------------------------------------------------------------------------
 * Функции для работы с EEPROM 24LC512
 *-----------------------------------------------------------------------------
 */

/**
 * @brief Ожидание готовности EEPROM (после записи)
 * @retval HAL_OK или HAL_TIMEOUT
 */
HAL_StatusTypeDef eeprom_wait_ready(void);

/**
 * @brief  Инициализация библиотеки EEPROM
 * @param  config: указатель на структуру конфигурации
 */
void EEPROM_Init(const EEPROM_Config_t* config);

/**
 * @brief  Запись блока данных с CRC и дублированием (основной + резервный адрес)
 * @param  main_addr: основной адрес
 * @param  backup_addr: резервный адрес
 * @param  data: указатель на данные
 * @param  size: размер данных (до 128 байт)
 * @retval true/false
 */
bool EEPROM_WriteDual(uint16_t main_addr, uint16_t backup_addr, const void* data, uint16_t size);

/**
 * @brief  Чтение блока данных с CRC и дублированием (основной + резервный адрес)
 * @param  main_addr: основной адрес
 * @param  backup_addr: резервный адрес
 * @param  data: указатель на буфер для данных
 * @param  size: размер данных (до 128 байт)
 * @retval true/false
 */
bool EEPROM_ReadDual(uint16_t main_addr, uint16_t backup_addr, void* data, uint16_t size);

/* --- Моточасы --- */

/**
 * @brief  Записать моточасы с резервированием
 * @param  motohours: значение моточасов
 * @retval true/false
 */
bool EEPROM_WriteMotohours(uint32_t motohours);

/**
 * @brief  Прочитать моточасы с резервированием
 * @param  motohours: указатель на переменную для чтения
 * @retval true/false
 */
bool EEPROM_ReadMotohours(uint32_t* motohours);

/**
 * @brief  Сбросить моточасы (обнулить)
 * @retval true/false
 */
bool EEPROM_ResetMotohours(void);

/* --- Флаги --- */

/**
 * @brief  Записать флаги (32 бита) с резервированием
 * @param  flags: значение флагов
 */
void EEPROM_WriteFlags(uint32_t flags);

/**
 * @brief  Прочитать флаги (32 бита) с резервированием
 * @retval flags
 */
uint32_t EEPROM_ReadFlags(void);

/* --- Массив 16 байт --- */

/**
 * @brief  Записать массив (16 байт) с резервированием
 * @param  array: указатель на массив
 */
void EEPROM_WriteArray(const uint8_t* array);

/**
 * @brief  Прочитать массив (16 байт) с резервированием
 * @param  array: указатель на массив
 * @retval true/false
 */
bool EEPROM_ReadArray(uint8_t* array);


