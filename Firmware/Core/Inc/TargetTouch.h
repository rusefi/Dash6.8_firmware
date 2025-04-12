#ifndef TARGET_TOUCH_H
#define TARGET_TOUCH_H


#include <ili211x.h>  // Подключаем заголовок для ILI2117A
#include "stm32f4xx_hal.h"

typedef struct {
    uint16_t x;    // Координата X точки касания
    uint16_t y;    // Координата Y точки касания
    uint8_t z;
    uint8_t id;

} TOUCH_Data;

void TOUCH_Init(void);
uint8_t TOUCH_IsPressed(void);
void TOUCH_Set(void);
void TOUCH_Reset(void);
TOUCH_Data TOUCH_GetData(void);

#endif /* TARGET_TOUCH_H */
