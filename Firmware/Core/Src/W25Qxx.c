/*
 * W25Qxx.c
 *
 *  Created on: Jun 4, 2025
 *      Author: Raman
 */

#include "W25Qxx.h"

w25qxx_t	W25qxx;

uint8_t W25qxx_Spi(uint8_t	Data)
{
	uint8_t	data;
	HAL_SPI_TransmitReceive(&_W25QXX_SPI, &Data, &data, 1, 100);
	return data;
}

void W25qxx_WriteEnable(void)
{
	W25QXX_CS_LOW;
	W25qxx_Spi(0x06);
	W25QXX_CS_HIGH;
	HAL_Delay(1);
}

void W25qxx_WriteDisable(void)
{
	W25QXX_CS_LOW;
	W25qxx_Spi(0x04);
	W25QXX_CS_HIGH;
	HAL_Delay(1);
}

uint8_t W25qxx_ReadStatusRegister(uint8_t	SelectStatusRegister_1_2_3)
{
	uint8_t	status=0;
	W25QXX_CS_LOW;
	W25qxx_Spi(SelectStatusRegister_1_2_3);
	status=W25qxx_Spi(0xff);
	W25QXX_CS_HIGH;
	return status;
}

void W25qxx_WriteStatusRegister(uint8_t	SelectStatusRegister_1_2_3, uint8_t	Data)
{
	W25QXX_CS_LOW;
	W25qxx_Spi(SelectStatusRegister_1_2_3);
	W25qxx_Spi(Data);
	W25QXX_CS_HIGH;
	HAL_Delay(1);
}

uint32_t W25qxx_ReadID(void)
{
	uint32_t	temp=0, temp2=0;
	W25QXX_CS_LOW;
	W25qxx_Spi(0x90);
	W25qxx_Spi(0x00);
	W25qxx_Spi(0x00);
	W25qxx_Spi(0x00);
	temp|=W25qxx_Spi(0xff)<<8;
	temp|=W25qxx_Spi(0xff);
	W25QXX_CS_HIGH;
	temp2|=temp>>8;
	temp2|=temp<<8;
	return temp2;
}

void W25qxx_ReadUniqID(void)
{
	uint8_t	i=0;
	W25QXX_CS_LOW;
	W25qxx_Spi(0x4B);
	for(i=0;i<4;i++)
		W25qxx_Spi(0xff);
	for(i=0;i<8;i++)
		W25qxx.UniqueID[i]=W25qxx_Spi(0xff);
	W25QXX_CS_HIGH;
}

void W25qxx_ReadManufacturerDeviceID(uint8_t* ManufacturerID, uint8_t* DeviceID)
{
	W25QXX_CS_LOW;
	W25qxx_Spi(0x90);
	W25qxx_Spi(0x00);
	W25qxx_Spi(0x00);
	W25qxx_Spi(0x00);
	*ManufacturerID=W25qxx_Spi(0xff);
	*DeviceID=W25qxx_Spi(0xff);
	W25QXX_CS_HIGH;
}

void W25qxx_WaitForWriteEnd(void)
{
	uint8_t	status=0;
	W25QXX_CS_LOW;
	W25qxx_Spi(0x05);
	do
	{
		status=W25qxx_Spi(0xff);
		HAL_Delay(1);
	}
	while((status&0x01)==0x01);
	W25QXX_CS_HIGH;
}

void W25qxx_Init(void)
{
	W25QXX_CS_HIGH;
	HAL_Delay(100);

	uint32_t id = W25qxx_ReadID();

	uint8_t ManufacturerID, DeviceID;
	W25qxx_ReadManufacturerDeviceID(&ManufacturerID, &DeviceID);

	W25qxx_ReadUniqID();

	switch(DeviceID)
	{
		case 0x10:	W25qxx.ID=W25Q10;	break;
		case 0x11:	W25qxx.ID=W25Q20;	break;
		case 0x12:	W25qxx.ID=W25Q40;	break;
		case 0x13:	W25qxx.ID=W25Q80;	break;
		case 0x14:	W25qxx.ID=W25Q16;	break;
		case 0x15:	W25qxx.ID=W25Q32;	break;
		case 0x16:	W25qxx.ID=W25Q64;	break;
		case 0x17:	W25qxx.ID=W25Q128;	break;
		case 0x18:	W25qxx.ID=W25Q256;	break;
		case 0x19:	W25qxx.ID=W25Q512;	break;
		default:	W25qxx.ID=W25Q32;	break; // по умолчанию
	}

	W25qxx.ManufacturerID = ManufacturerID;
	W25qxx.DeviceID = DeviceID;

	W25qxx.PageSize = 256;
	W25qxx.SectorSize = 0x1000;
	W25qxx.BlockSize = 0x10000;

	switch(W25qxx.ID)
	{
		case W25Q10:	W25qxx.PageCount = 512;	break;
		case W25Q20:	W25qxx.PageCount = 1024;	break;
		case W25Q40:	W25qxx.PageCount = 2048;	break;
		case W25Q80:	W25qxx.PageCount = 4096;	break;
		case W25Q16:	W25qxx.PageCount = 8192;	break;
		case W25Q32:	W25qxx.PageCount = 16384;	break;
		case W25Q64:	W25qxx.PageCount = 32768;	break;
		case W25Q128:	W25qxx.PageCount = 65536;	break;
		case W25Q256:	W25qxx.PageCount = 131072;	break;
		case W25Q512:	W25qxx.PageCount = 262144;	break;
		default:		W25qxx.PageCount = 16384;	break;
	}

	W25qxx.SectorCount = W25qxx.PageCount / 16;
	W25qxx.BlockCount = W25qxx.PageCount / 256;
	W25qxx.CapacityInKiloByte = (W25qxx.PageCount * W25qxx.PageSize) / 1024;
}

void W25qxx_EraseChip(void)
{
	W25qxx_WriteEnable();
	W25QXX_CS_LOW;
	W25qxx_Spi(0xC7);
	W25QXX_CS_HIGH;
	W25qxx_WaitForWriteEnd();
}

void W25qxx_EraseSector(uint32_t SectorAddr)
{
	W25qxx_WriteEnable();
	W25QXX_CS_LOW;
	W25qxx_Spi(0x20);
	W25qxx_Spi((SectorAddr * W25qxx.SectorSize) >> 16);
	W25qxx_Spi((SectorAddr * W25qxx.SectorSize) >> 8);
	W25qxx_Spi((SectorAddr * W25qxx.SectorSize));
	W25QXX_CS_HIGH;
	W25qxx_WaitForWriteEnd();
}

void W25qxx_EraseBlock(uint32_t BlockAddr)
{
	W25qxx_WriteEnable();
	W25QXX_CS_LOW;
	W25qxx_Spi(0xD8);
	W25qxx_Spi((BlockAddr * W25qxx.BlockSize) >> 16);
	W25qxx_Spi((BlockAddr * W25qxx.BlockSize) >> 8);
	W25qxx_Spi((BlockAddr * W25qxx.BlockSize));
	W25QXX_CS_HIGH;
	W25qxx_WaitForWriteEnd();
}

uint32_t W25qxx_PageToSector(uint32_t PageAddress)
{
	return PageAddress / 16;
}

uint32_t W25qxx_PageToBlock(uint32_t PageAddress)
{
	return PageAddress / 256;
}

uint32_t W25qxx_SectorToBlock(uint32_t SectorAddress)
{
	return SectorAddress / 16;
}

uint32_t W25qxx_SectorToPage(uint32_t SectorAddress)
{
	return SectorAddress * 16;
}

uint32_t W25qxx_BlockToPage(uint32_t BlockAddress)
{
	return BlockAddress * 256;
}

void W25qxx_WritePage(uint8_t* pBuffer, uint32_t PageAddress, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_PageSize)
{
	if ((NumByteToWrite_up_to_PageSize + OffsetInByte) > W25qxx.PageSize)
		NumByteToWrite_up_to_PageSize = W25qxx.PageSize - OffsetInByte;

	W25qxx_WriteEnable();
	W25QXX_CS_LOW;
	W25qxx_Spi(0x02);
	W25qxx_Spi((PageAddress * W25qxx.PageSize + OffsetInByte) >> 16);
	W25qxx_Spi((PageAddress * W25qxx.PageSize + OffsetInByte) >> 8);
	W25qxx_Spi((PageAddress * W25qxx.PageSize + OffsetInByte));
	HAL_SPI_Transmit(&_W25QXX_SPI, pBuffer, NumByteToWrite_up_to_PageSize, 100);
	W25QXX_CS_HIGH;
	W25qxx_WaitForWriteEnd();
}

void W25qxx_WriteSector(uint8_t* pBuffer, uint32_t SectorAddress, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_SectorSize)
{
	if ((NumByteToWrite_up_to_SectorSize + OffsetInByte) > W25qxx.SectorSize)
		NumByteToWrite_up_to_SectorSize = W25qxx.SectorSize - OffsetInByte;

	uint32_t startPage = W25qxx_SectorToPage(SectorAddress);
	uint32_t offset = OffsetInByte;
	uint32_t bytesToWrite;
	while (NumByteToWrite_up_to_SectorSize > 0)
	{
		bytesToWrite = W25qxx.PageSize - offset;
		if (NumByteToWrite_up_to_SectorSize < bytesToWrite)
			bytesToWrite = NumByteToWrite_up_to_SectorSize;
		W25qxx_WritePage(pBuffer, startPage, offset, bytesToWrite);
		NumByteToWrite_up_to_SectorSize -= bytesToWrite;
		pBuffer += bytesToWrite;
		offset = 0;
		startPage++;
	}
}

void W25qxx_WriteBlock(uint8_t* pBuffer, uint32_t BlockAddress, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_BlockSize)
{
	if ((NumByteToWrite_up_to_BlockSize + OffsetInByte) > W25qxx.BlockSize)
		NumByteToWrite_up_to_BlockSize = W25qxx.BlockSize - OffsetInByte;

	uint32_t startPage = W25qxx_BlockToPage(BlockAddress);
	uint32_t offset = OffsetInByte;
	uint32_t bytesToWrite;
	while (NumByteToWrite_up_to_BlockSize > 0)
	{
		bytesToWrite = W25qxx.PageSize - offset;
		if (NumByteToWrite_up_to_BlockSize < bytesToWrite)
			bytesToWrite = NumByteToWrite_up_to_BlockSize;
		W25qxx_WritePage(pBuffer, startPage, offset, bytesToWrite);
		NumByteToWrite_up_to_BlockSize -= bytesToWrite;
		pBuffer += bytesToWrite;
		offset = 0;
		startPage++;
	}
}

void W25qxx_ReadPage(uint8_t* pBuffer, uint32_t PageAddress, uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_PageSize)
{
	if ((NumByteToRead_up_to_PageSize + OffsetInByte) > W25qxx.PageSize)
		NumByteToRead_up_to_PageSize = W25qxx.PageSize - OffsetInByte;

	W25QXX_CS_LOW;
	W25qxx_Spi(0x03);
	W25qxx_Spi((PageAddress * W25qxx.PageSize + OffsetInByte) >> 16);
	W25qxx_Spi((PageAddress * W25qxx.PageSize + OffsetInByte) >> 8);
	W25qxx_Spi((PageAddress * W25qxx.PageSize + OffsetInByte));
	HAL_SPI_Receive(&_W25QXX_SPI, pBuffer, NumByteToRead_up_to_PageSize, 100);
	W25QXX_CS_HIGH;
}

void W25qxx_ReadSector(uint8_t* pBuffer, uint32_t SectorAddress, uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_SectorSize)
{
	if ((NumByteToRead_up_to_SectorSize + OffsetInByte) > W25qxx.SectorSize)
		NumByteToRead_up_to_SectorSize = W25qxx.SectorSize - OffsetInByte;

	uint32_t startPage = W25qxx_SectorToPage(SectorAddress);
	uint32_t offset = OffsetInByte;
	uint32_t bytesToRead;
	while (NumByteToRead_up_to_SectorSize > 0)
	{
		bytesToRead = W25qxx.PageSize - offset;
		if (NumByteToRead_up_to_SectorSize < bytesToRead)
			bytesToRead = NumByteToRead_up_to_SectorSize;
		W25qxx_ReadPage(pBuffer, startPage, offset, bytesToRead);
		NumByteToRead_up_to_SectorSize -= bytesToRead;
		pBuffer += bytesToRead;
		offset = 0;
		startPage++;
	}
}

void W25qxx_ReadBlock(uint8_t* pBuffer, uint32_t BlockAddress, uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_BlockSize)
{
	if ((NumByteToRead_up_to_BlockSize + OffsetInByte) > W25qxx.BlockSize)
		NumByteToRead_up_to_BlockSize = W25qxx.BlockSize - OffsetInByte;

	uint32_t startPage = W25qxx_BlockToPage(BlockAddress);
	uint32_t offset = OffsetInByte;
	uint32_t bytesToRead;
	while (NumByteToRead_up_to_BlockSize > 0)
	{
		bytesToRead = W25qxx.PageSize - offset;
		if (NumByteToRead_up_to_BlockSize < bytesToRead)
			bytesToRead = NumByteToRead_up_to_BlockSize;
		W25qxx_ReadPage(pBuffer, startPage, offset, bytesToRead);
		NumByteToRead_up_to_BlockSize -= bytesToRead;
		pBuffer += bytesToRead;
		offset = 0;
		startPage++;
	}
}



