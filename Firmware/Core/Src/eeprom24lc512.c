#include "eeprom24lc512.h"
#include <string.h>

/* ===================== Константы ===================== */
#define EEPROM_WRITE_RETRY_MAX   3    // Максимум попыток записи
#define EEPROM_READ_RETRY_MAX    3    // Максимум попыток чтения

/* =================== Глобальные переменные =================== */
static EEPROM_Config_t eeprom_cfg;     // Конфигурация EEPROM

/* =================== Внутренние функции =================== */

/**
 * @brief  Вычисление CRC32 (аппаратно или программно)
 * @param  data: указатель на данные
 * @param  size: размер данных
 * @retval CRC32
 */
static uint32_t CRC32_Calc(const uint8_t* data, uint16_t size)
{
    if (eeprom_cfg.hcrc) {
        // Аппаратный CRC
        uint32_t crc = HAL_CRC_Calculate(eeprom_cfg.hcrc, (uint32_t*)data, size / 4);
        // Обработка хвоста, если size не кратно 4
        for (uint16_t i = (size & ~3); i < size; i++) {
            crc ^= data[i];
            for (uint8_t j = 0; j < 8; j++)
                crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
        }
        return ~crc;
    } else {
        // Программный CRC32 (полином 0xEDB88320)
        uint32_t crc = 0xFFFFFFFF;
        for (uint16_t i = 0; i < size; i++) {
            crc ^= data[i];
            for (uint8_t j = 0; j < 8; j++)
                crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
        }
        return ~crc;
    }
}

/**
 * @brief  Инициализация модуля EEPROM
 * @param  config: указатель на структуру конфигурации
 */
void EEPROM_Init(const EEPROM_Config_t* config)
{
    eeprom_cfg = *config;
}

/**
 * @brief  Ожидание готовности EEPROM к работе
 * @retval HAL_OK или HAL_TIMEOUT
 */
HAL_StatusTypeDef eeprom_wait_ready(void)
{
    uint32_t tickstart = HAL_GetTick();
    while (HAL_I2C_IsDeviceReady(eeprom_cfg.hi2c, eeprom_cfg.i2c_address, 3, 5) != HAL_OK) {
        if ((HAL_GetTick() - tickstart) > eeprom_cfg.write_timeout)
            return HAL_TIMEOUT;
    }
    return HAL_OK;
}

/* =================== Базовые функции =================== */

/**
 * @brief  Страничная запись в EEPROM
 * @param  mem_addr: адрес в памяти
 * @param  data: указатель на данные
 * @param  size: размер данных
 * @retval HAL_OK или HAL_ERROR
 */
static HAL_StatusTypeDef EEPROM_Write(uint16_t mem_addr, const uint8_t* data, uint16_t size)
{
    uint16_t page_size = eeprom_cfg.page_size;
    while (size > 0) {
        uint16_t page_space = page_size - (mem_addr % page_size);
        uint16_t write_size = (size < page_space) ? size : page_space;
        uint8_t  buf[2 + 128];
        buf[0] = (mem_addr >> 8) & 0xFF;
        buf[1] = mem_addr & 0xFF;
        memcpy(&buf[2], data, write_size);

        HAL_StatusTypeDef status = HAL_ERROR;
        for (uint8_t attempt = 0; attempt < EEPROM_WRITE_RETRY_MAX; attempt++) {
            status = HAL_I2C_Master_Transmit(
                eeprom_cfg.hi2c,
                eeprom_cfg.i2c_address,
                buf,
                2 + write_size,
                eeprom_cfg.write_timeout
            );
            if (status == HAL_OK && eeprom_wait_ready() == HAL_OK)
                break;
            HAL_Delay(5);
        }
        if (status != HAL_OK)
            return status;

        mem_addr += write_size;
        data     += write_size;
        size     -= write_size;
    }
    return HAL_OK;
}

/**
 * @brief  Чтение из EEPROM
 * @param  mem_addr: адрес в памяти
 * @param  data: указатель на буфер
 * @param  size: размер данных
 * @retval HAL_OK или HAL_ERROR
 */
static HAL_StatusTypeDef EEPROM_Read(uint16_t mem_addr, uint8_t* data, uint16_t size)
{
    uint8_t addr_buf[2] = { (mem_addr >> 8) & 0xFF, mem_addr & 0xFF };
    if (HAL_I2C_Master_Transmit(eeprom_cfg.hi2c, eeprom_cfg.i2c_address, addr_buf, 2, eeprom_cfg.read_timeout) != HAL_OK)
        return HAL_ERROR;
    if (HAL_I2C_Master_Receive(eeprom_cfg.hi2c, eeprom_cfg.i2c_address, data, size, eeprom_cfg.read_timeout) != HAL_OK)
        return HAL_ERROR;
    return HAL_OK;
}

/* =================== Универсальные функции с CRC и дублированием =================== */

/**
 * @brief  Запись блока с CRC и дублированием (основной + резервный адрес)
 * @param  main_addr: основной адрес
 * @param  backup_addr: резервный адрес
 * @param  data: указатель на данные
 * @param  size: размер данных
 * @retval true/false
 */
bool EEPROM_WriteDual(uint16_t main_addr, uint16_t backup_addr, const void* data, uint16_t size)
{
    if (size > 128) return false;
    uint8_t buf[128 + 4];
    memcpy(buf, data, size);
    uint32_t crc = CRC32_Calc((const uint8_t*)data, size);
    memcpy(buf + size, &crc, 4);

    // Запись основной копии
    if (EEPROM_Write(main_addr, buf, size + 4) != HAL_OK)
        return false;
    // Запись резервной копии
    if (EEPROM_Write(backup_addr, buf, size + 4) != HAL_OK)
        return false;
    return true;
}

/**
 * @brief  Чтение блока с CRC и дублированием (основной + резервный адрес)
 * @param  main_addr: основной адрес
 * @param  backup_addr: резервный адрес
 * @param  data: указатель на буфер
 * @param  size: размер данных
 * @retval true/false
 */
bool EEPROM_ReadDual(uint16_t main_addr, uint16_t backup_addr, void* data, uint16_t size)
{
    if (size > 128) return false;
    uint8_t  buf_main[128 + 4], buf_backup[128 + 4];
    bool     main_ok = false, backup_ok = false;
    uint32_t crc_main, crc_backup;

    // Чтение основной копии
    for (uint8_t attempt = 0; attempt < EEPROM_READ_RETRY_MAX; attempt++) {
        if (EEPROM_Read(main_addr, buf_main, size + 4) == HAL_OK) {
            memcpy(&crc_main, buf_main + size, 4);
            if (CRC32_Calc(buf_main, size) == crc_main) {
                main_ok = true;
                break;
            }
        }
        HAL_Delay(2);
    }
    // Чтение резервной копии
    for (uint8_t attempt = 0; attempt < EEPROM_READ_RETRY_MAX; attempt++) {
        if (EEPROM_Read(backup_addr, buf_backup, size + 4) == HAL_OK) {
            memcpy(&crc_backup, buf_backup + size, 4);
            if (CRC32_Calc(buf_backup, size) == crc_backup) {
                backup_ok = true;
                break;
            }
        }
        HAL_Delay(2);
    }

    // Восстановление данных и синхронизация копий
    if (main_ok) {
        memcpy(data, buf_main, size);
        if (!backup_ok) EEPROM_WriteDual(backup_addr, main_addr, data, size);
        return true;
    }
    if (backup_ok) {
        memcpy(data, buf_backup, size);
        EEPROM_WriteDual(main_addr, backup_addr, data, size);
        return true;
    }
    return false;
}

/* =================== Специализированные функции =================== */

// --- Моточасы ---
bool EEPROM_WriteMotohours(uint32_t motohours)
{
    return EEPROM_WriteDual(eeprom_cfg.motohours_main_addr, eeprom_cfg.motohours_backup_addr, &motohours, sizeof(motohours));
}

bool EEPROM_ReadMotohours(uint32_t* motohours)
{
    return EEPROM_ReadDual(eeprom_cfg.motohours_main_addr, eeprom_cfg.motohours_backup_addr, motohours, sizeof(*motohours));
}

bool EEPROM_ResetMotohours(void)
{
    uint32_t zero = 0;
    return EEPROM_WriteDual(eeprom_cfg.motohours_main_addr, eeprom_cfg.motohours_backup_addr, &zero, sizeof(zero));
}

// --- Флаги ---
void EEPROM_WriteFlags(uint32_t flags)
{
    EEPROM_WriteDual(eeprom_cfg.flags_main_addr, eeprom_cfg.flags_backup_addr, &flags, sizeof(flags));
}

uint32_t EEPROM_ReadFlags(void)
{
    uint32_t flags = 0;
    EEPROM_ReadDual(eeprom_cfg.flags_main_addr, eeprom_cfg.flags_backup_addr, &flags, sizeof(flags));
    return flags;
}

// --- Массив 16 байт ---
void EEPROM_WriteArray(const uint8_t* array)
{
    EEPROM_WriteDual(eeprom_cfg.array_main_addr, eeprom_cfg.array_backup_addr, array, 16);
}

bool EEPROM_ReadArray(uint8_t* array)
{
    return EEPROM_ReadDual(eeprom_cfg.array_main_addr, eeprom_cfg.array_backup_addr, array, 16);
}
