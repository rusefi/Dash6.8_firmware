/*
 * CAN.h
 *
 *  Created on: May 7, 2025
 *      Author: Raman
 */

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <string.h> // Для memcpy
#include "extern.h"

#ifndef INC_CAN_H_
#define INC_CAN_H_

#define EXTRACT_U16(data, start_bit) ((uint16_t)(((data)[(start_bit) / 8] >> ((start_bit) % 8)) | ((data)[((start_bit) / 8) + 1] << (8 - ((start_bit) % 8)))))
#define EXTRACT_S16(data, start_bit) \
        ((int16_t)EXTRACT_U16(data, start_bit))
#define EXTRACT_U8(data, start_bit) \
        ((uint8_t)(((data)[(start_bit)/8] >> ((start_bit)%8)) & 0xFF))
#define EXTRACT_S8(data, start_bit) \
        ((int8_t)EXTRACT_U8(data, start_bit))

void Handle_BO_3221225472(uint8_t *data);
void Handle_BO_512(uint8_t *data);
void Handle_BO_513(uint8_t *data);
void Handle_BO_514(uint8_t *data);
void Handle_BO_515(uint8_t *data);
void Handle_BO_516(uint8_t *data);
void Handle_BO_517(uint8_t *data);
void Handle_BO_518(uint8_t *data);
void Handle_BO_519(uint8_t *data);
void Handle_BO_520(uint8_t *data);
void Handle_BO_521(uint8_t *data);
void Handle_BO_0x281(uint8_t *data);
void Handle_BO_0x282(uint8_t *data);
void Handle_BO_0x283(uint8_t *data);
void Handle_BO_0x284(uint8_t *data);
void Handle_BO_0x381(uint8_t *data);
void Handle_BO_0x382(uint8_t *data);
void Handle_BO_0x383(uint8_t *data);
void Handle_BO_0x481(uint8_t *data);
void Handle_BO_0x482(uint8_t *data);
void Handle_BO_0x581(uint8_t *data);
void Handle_BO_0x582(uint8_t *data);
void Handle_BO_0x583(uint8_t *data);
void Handle_BO_0x584(uint8_t *data);
void Handle_BO_0x781(uint8_t *data);




#endif /* INC_CAN_H_ */

