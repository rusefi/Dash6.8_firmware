#ifndef ILI211X_H
#define ILI211X_H

#include "stm32f4xx_hal.h"


// Определение порта GPIO и пина для прерывания
#define INT_PORT GPIOI          // Замените на ваш фактический порт GPIO
#define INT_PIN GPIO_PIN_11     // Замените на ваш фактический пин GPIO

// Определение адреса I2C для ILI211X
#define ILI211X_ADDR             (0x26 << 1) // Сдвиг влево для 7-битного адреса

// Определение поворота осей
#define ILI211X_FLIP_AXIS        1

// Определение таймаутов
#define ILI211X_TIMEOUT           1000  // Таймаут для операций I2C в миллисекундах

// Адреса регистров для ILI211X
#define ILI211X_REG_MODE                0x00  // Регистры режима работы
#define ILI211X_REG_COMMAND             0x01  // Регистры команд
#define ILI211X_REG_RESET               0x02  // Регистры сброса
#define ILI211X_REG_SLEEP               0x03  // Регистры режима сна
#define ILI211X_REG_DISPLAY_ON          0x04  // Регистры включения дисплея
#define ILI211X_REG_SET_BRIGHTNESS      0x05  // Регистры управления яркостью
#define ILI211X_REG_SET_PIXEL_FORMAT    0x06  // Регистры управления форматом пикселей
#define ILI211X_REG_SET_DISPLAY_MODE     0x07  // Регистры настройки режима дисплея

// Дополнительные адреса регистров для функциональности касания
#define ILI211X_GEST_ID               0x01   // Регистры ID жеста
#define ILI211X_TD_STATUS             0x02   // Регистры статуса касания
#define ILI211X_TOUCH_DATA_PTR        0x00   // Указатель на данные касания
#define ILI211X_TOUCH_DATA_LEN        43      // Количество байтов на одну точку касания

// Определения жестов
#define ILI211X_GEST_NO_GESTURE       0x00   // Жест не обнаружен
#define ILI211X_GEST_MOVE_UP          0x10   // Жест "движение вверх"
#define ILI211X_GEST_MOVE_DOWN        0x18   // Жест "движение вниз"
#define ILI211X_GEST_MOVE_LEFT        0x14   // Жест "движение влево"
#define ILI211X_GEST_MOVE_RIGHT       0x1C   // Жест "движение вправо"

// Структура данных точки касания
typedef struct {
    uint16_t x;         // Координата X точки касания
    uint16_t y;         // Координата Y точки касания
    uint8_t id;         // ID касания (для мультитач)
    uint8_t z;         // Тип события касания (нажатие, движение, отпускание)
} CTP_point_data;

// Прототипы функций
uint8_t ILI211X_Init(I2C_HandleTypeDef hi2c);                 // Инициализация контроллера
void ILI211X_Write(uint8_t reg_addr, uint8_t byte_to_write);    // Запись в регистр
uint8_t ILI211X_Read(uint8_t reg_addr);                          // Чтение из регистра
void ILI211X_ReadMultiple(uint8_t reg_addr, uint8_t *buff, uint8_t num_of_bytes); // Чтение нескольких байтов из регистра
uint8_t ILI211X_GetGestureID(void);                              // Получить ID жеста от контроллера
uint8_t ILI211X_GetNumOfTouches(void);                           // Получить количество обнаруженных касаний
CTP_point_data ILI211X_ReadPoint(uint8_t point_num);           // Чтение данных для одной точки касания

void ILI211X_SetDisplaySize(uint16_t width, uint16_t height);   // Установка размера дисплея

#endif /*ILI211X_H */
