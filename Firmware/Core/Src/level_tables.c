/*
 * fuel_level.c
 *
 *  Created on: Jun 1, 2025
 *      Author: Raman
 */

#include "level_tables.h"
#include "W25Qxx.h"
#include <string.h>
#include <stdint.h>

// --- Адреса хранения ---
#define FUEL_LEVEL_TABLE_ADDR  0x00000
#define OIL_LEVEL_TABLE_ADDR   0x01000 // Следующий сектор

#define TEST_SECTOR_ADDR 0x00000

// --- Значения по умолчанию ---
static const float DEFAULT_FUEL_TABLE[FUEL_LEVEL_POINTS] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75};
static const float DEFAULT_OIL_TABLE[OIL_LEVEL_POINTS] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75};
static const float DEFAULT_FUEL_MAX = 75.0f;
static const float DEFAULT_OIL_MAX  = 7.5f;

// --- Структуры для хранения во флеш ---
#pragma pack(push, 1)
typedef struct {
    float table[FUEL_LEVEL_POINTS];
    float max;
    uint32_t crc32;
} FuelTableFlashBlock;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    float table[OIL_LEVEL_POINTS];
    float max;
    uint32_t crc32;
} OilTableFlashBlock;
#pragma pack(pop)

// --- Глобальные переменные ---
float fuelLevelTable[FUEL_LEVEL_POINTS];
float fuelLevelMax;
float oilLevelTable[OIL_LEVEL_POINTS];
float oilLevelMax;

// --- CRC32 (простая реализация) ---
static uint32_t crc32_calc(const uint8_t *data, size_t len)
{
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++)
            crc = (crc >> 1) ^ (0xEDB88320U & (-(crc & 1)));
    }
    return crc ^ 0xFFFFFFFF;
}

// --- Запись массива во флеш через W25Qxx ---
static void flash_write_array(uint32_t addr, const uint8_t *src, uint32_t len) {
    uint32_t page_size = W25qxx.PageSize;
    while (len > 0) {
        uint32_t page_addr = addr / page_size;
        uint32_t page_offset = addr % page_size;
        uint32_t bytes_in_page = page_size - page_offset;
        uint32_t chunk = (len < bytes_in_page) ? len : bytes_in_page;
        W25qxx_WritePage((uint8_t*)src, page_addr, page_offset, chunk);
        addr += chunk;
        src  += chunk;
        len  -= chunk;
    }
}

// --- Чтение массива из флеш через W25Qxx ---
static void flash_read_array(uint32_t addr, uint8_t *dst, uint32_t len) {
    uint32_t page_size = W25qxx.PageSize;
    while (len > 0) {
        uint32_t page_addr = addr / page_size;
        uint32_t page_offset = addr % page_size;
        uint32_t bytes_in_page = page_size - page_offset;
        uint32_t chunk = (len < bytes_in_page) ? len : bytes_in_page;
        W25qxx_ReadPage(dst, page_addr, page_offset, chunk);
        addr += chunk;
        dst  += chunk;
        len  -= chunk;
    }
}

// ================== ТОПЛИВО ==================

void fuel_level_save_table(void) {
    FuelTableFlashBlock block;
    memcpy(block.table, fuelLevelTable, sizeof(block.table));
    block.max = fuelLevelMax;
    block.crc32 = crc32_calc((uint8_t*)&block, sizeof(block) - sizeof(uint32_t));
    W25qxx_EraseSector(FUEL_LEVEL_TABLE_ADDR / W25qxx.SectorSize);
    flash_write_array(FUEL_LEVEL_TABLE_ADDR, (uint8_t*)&block, sizeof(block));
}

void fuel_level_load_table(void) {
    FuelTableFlashBlock block;
    flash_read_array(FUEL_LEVEL_TABLE_ADDR, (uint8_t*)&block, sizeof(block));
    uint32_t calc_crc = crc32_calc((uint8_t*)&block, sizeof(block) - sizeof(uint32_t));
    if (block.crc32 != calc_crc) {
        memcpy(fuelLevelTable, DEFAULT_FUEL_TABLE, sizeof(fuelLevelTable));
        fuelLevelMax = DEFAULT_FUEL_MAX;
        fuel_level_save_table();
    } else {
        memcpy(fuelLevelTable, block.table, sizeof(fuelLevelTable));
        fuelLevelMax = block.max;
    }
}

float fuel_level_get_liters(float raw_value) {
    if (raw_value <= 0.0f)
        return fuelLevelTable[0];
    if (raw_value >= 100.0f)
        return fuelLevelTable[FUEL_LEVEL_POINTS - 1];
    float step = 100.0f / (FUEL_LEVEL_POINTS - 1);
    int idx = (int)(raw_value / step);
    float frac = (raw_value - idx * step) / step;
    if (idx >= FUEL_LEVEL_POINTS - 1)
        return fuelLevelTable[FUEL_LEVEL_POINTS - 1];
    return fuelLevelTable[idx] + frac * (fuelLevelTable[idx + 1] - fuelLevelTable[idx]);
}

void fuel_level_set_table(const float *table, size_t len) {
    if (!table || len == 0) return;
    size_t count = (len > FUEL_LEVEL_POINTS) ? FUEL_LEVEL_POINTS : len;
    memcpy(fuelLevelTable, table, count * sizeof(float));
    fuelLevelMax = fuelLevelTable[FUEL_LEVEL_POINTS - 1];
}

size_t fuel_level_get_table(float *user_table, size_t max_len) {
    if (!user_table || max_len == 0) return 0;
    size_t count = (max_len > FUEL_LEVEL_POINTS) ? FUEL_LEVEL_POINTS : max_len;
    memcpy(user_table, fuelLevelTable, count * sizeof(float));
    return count;
}

// ================== МАСЛО ==================

void oil_level_save_table(void)
{
	OilTableFlashBlock block;
	memcpy(block.table, oilLevelTable, sizeof(block.table));
	block.max = oilLevelMax;
    block.crc32 = crc32_calc((uint8_t*)&block, sizeof(block) - sizeof(uint32_t));
	W25qxx_EraseSector(OIL_LEVEL_TABLE_ADDR / W25qxx.SectorSize);
	flash_write_array(OIL_LEVEL_TABLE_ADDR, (uint8_t*)&block, sizeof(block));
}

void oil_level_load_table(void)
{
    OilTableFlashBlock block;
    flash_read_array(OIL_LEVEL_TABLE_ADDR, (uint8_t*)&block, sizeof(block));
    uint32_t calc_crc = crc32_calc((uint8_t*)&block, sizeof(block) - sizeof(uint32_t));
    if (block.crc32 != calc_crc) {
        memcpy(oilLevelTable, DEFAULT_OIL_TABLE, sizeof(oilLevelTable));
        oilLevelMax = DEFAULT_OIL_MAX;
        oil_level_save_table(); // Сохраняем дефолтные значения
    } else {
        memcpy(oilLevelTable, block.table, sizeof(oilLevelTable));
        oilLevelMax = block.max;
    }
}

float oil_level_get_liters(float raw_value) {
    if (raw_value <= 0.0f)
        return oilLevelTable[0];
    if (raw_value >= 100.0f)
        return oilLevelTable[OIL_LEVEL_POINTS - 1];
    float step = 100.0f / (OIL_LEVEL_POINTS - 1);
    int idx = (int)(raw_value / step);
    float frac = (raw_value - idx * step) / step;
    if (idx >= OIL_LEVEL_POINTS - 1)
        return oilLevelTable[OIL_LEVEL_POINTS - 1];
    return oilLevelTable[idx] + frac * (oilLevelTable[idx + 1] - oilLevelTable[idx]);
}

void oil_level_set_table(const float *table, size_t len) {
    if (!table || len == 0) return;
    size_t count = (len > OIL_LEVEL_POINTS) ? OIL_LEVEL_POINTS : len;
    memcpy(oilLevelTable, table, count * sizeof(float));
    oilLevelMax = oilLevelTable[OIL_LEVEL_POINTS - 1];
}

size_t oil_level_get_table(float *user_table, size_t max_len) {
    if (!user_table || max_len == 0) return 0;
    size_t count = (max_len > OIL_LEVEL_POINTS) ? OIL_LEVEL_POINTS : max_len;
    memcpy(user_table, oilLevelTable, count * sizeof(float));
    return count;
}


void test_flash_erase_sector(void) {
    uint8_t read_buf[256]; // буфер для проверки (одна страница)
    uint32_t sector_num = TEST_SECTOR_ADDR / W25qxx.SectorSize;

    printf("Стираем сектор %lu (адрес 0x%08lX)...\n", (unsigned long)sector_num, (unsigned long)TEST_SECTOR_ADDR);

    // Стираем сектор
    W25qxx_EraseSector(sector_num);

    // Проверяем первые 256 байт сектора
    W25qxx_ReadPage(read_buf, TEST_SECTOR_ADDR / W25qxx.PageSize, 0, sizeof(read_buf));

    // Анализируем результат
    int ok = 1;
    for (int i = 0; i < sizeof(read_buf); ++i) {
        if (read_buf[i] != 0xFF) {
            printf("Ошибка: байт %d = 0x%02X (ожидалось 0xFF)\n", i, read_buf[i]);
            ok = 0;
        }
    }
    if (ok) {
        printf("Сектор успешно стёрт, все байты = 0xFF\n");
    } else {
        printf("Стирание сектора выполнено с ошибками!\n");
    }
}

void test_flash_table(void) {
    float test_table[FUEL_LEVEL_POINTS] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    FuelTableFlashBlock block;
    memcpy(block.table, test_table, sizeof(block.table));
    block.max = 15.0f;
    block.crc32 = crc32_calc((uint8_t*)&block, sizeof(block) - sizeof(uint32_t));

    printf("sizeof(FuelTableFlashBlock) = %lu\n", (unsigned long)sizeof(FuelTableFlashBlock));

    // Стираем сектор
    W25qxx_EraseSector(FUEL_LEVEL_TABLE_ADDR / W25qxx.SectorSize);

    // Записываем структуру
    flash_write_array(FUEL_LEVEL_TABLE_ADDR, (uint8_t*)&block, sizeof(FuelTableFlashBlock));

    // Очищаем RAM
    memset(&block, 0, sizeof(block));

    // Читаем обратно
    flash_read_array(FUEL_LEVEL_TABLE_ADDR, (uint8_t*)&block, sizeof(FuelTableFlashBlock));

    // Выводим результат
    for (int i = 0; i < FUEL_LEVEL_POINTS; ++i) {

    }

}
