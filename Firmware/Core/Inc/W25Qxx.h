/*
 * W25Qxx.h
 *
 *  Created on: Jun 4, 2025
 *      Author: Raman
 */

#ifndef __W25QXX_H
#define __W25QXX_H

#include "stm32f4xx_hal.h"
#include "extern.h"
#include "main.h"

// === НАСТРОЙКИ ДЛЯ ВАШЕГО ПРОЕКТА ===
#define _W25QXX_SPI                   hspi1
#define _W25QXX_CS_GPIO               FLASH_CS_GPIO_Port
#define _W25QXX_CS_PIN                FLASH_CS_Pin
// ====================================


#define W25QXX_CS_LOW  HAL_GPIO_WritePin(_W25QXX_CS_GPIO, _W25QXX_CS_PIN, GPIO_PIN_RESET)
#define W25QXX_CS_HIGH HAL_GPIO_WritePin(_W25QXX_CS_GPIO, _W25QXX_CS_PIN, GPIO_PIN_SET)

typedef enum
{
	W25Q10=1,
	W25Q20,
	W25Q40,
	W25Q80,
	W25Q16,
	W25Q32,
	W25Q64,
	W25Q128,
	W25Q256,
	W25Q512,
} W25QXX_ID_t;

typedef struct
{
	W25QXX_ID_t	ID;
	uint8_t		UniqueID[8];
	uint16_t	PageSize;
	uint32_t	PageCount;
	uint32_t	SectorSize;
	uint32_t	SectorCount;
	uint32_t	BlockSize;
	uint32_t	BlockCount;
	uint32_t	CapacityInKiloByte;
	uint8_t		ManufacturerID;
	uint8_t		DeviceID;
	uint8_t		UID[8];
	uint8_t		SN[20];
} w25qxx_t;

extern w25qxx_t	W25qxx;

// Функции
void		W25qxx_Init(void);
void		W25qxx_EraseChip(void);
void		W25qxx_EraseSector(uint32_t SectorAddr);
void		W25qxx_EraseBlock(uint32_t BlockAddr);
uint32_t	W25qxx_PageToSector(uint32_t	PageAddress);
uint32_t	W25qxx_PageToBlock(uint32_t	PageAddress);
uint32_t	W25qxx_SectorToBlock(uint32_t	SectorAddress);
uint32_t	W25qxx_SectorToPage(uint32_t	SectorAddress);
uint32_t	W25qxx_BlockToPage(uint32_t	BlockAddress);

void		W25qxx_WritePage(uint8_t* pBuffer, uint32_t PageAddress, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_PageSize);
void		W25qxx_WriteSector(uint8_t* pBuffer, uint32_t SectorAddress, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_SectorSize);
void		W25qxx_WriteBlock(uint8_t* pBuffer, uint32_t BlockAddress, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_BlockSize);

void		W25qxx_ReadPage(uint8_t* pBuffer, uint32_t PageAddress, uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_PageSize);
void		W25qxx_ReadSector(uint8_t* pBuffer, uint32_t SectorAddress, uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_SectorSize);
void		W25qxx_ReadBlock(uint8_t* pBuffer, uint32_t BlockAddress, uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_BlockSize);

#endif
