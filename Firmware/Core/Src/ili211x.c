
#include "ili211x.h"
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
// Статическая переменная для хранения указателя на I2C-обработчик.
static I2C_HandleTypeDef hi2c;

// Функция для чтения регистра из ILI211X.
static int ili211x_read_reg(uint8_t reg, void *buf, size_t len) {
    return HAL_I2C_Mem_Read(&hi2c, ILI211X_ADDR, reg, I2C_MEMADD_SIZE_8BIT, buf, len, ILI211X_TIMEOUT);
}

// Функция для чтения данных касания из ILI211X.
static int ili211x_read_touch_data(uint8_t *data) {
    int ret = HAL_I2C_Mem_Read(&hi2c, ILI211X_ADDR, ILI211X_TOUCH_DATA_PTR, I2C_MEMADD_SIZE_8BIT, data, ILI211X_TOUCH_DATA_LEN, ILI211X_TIMEOUT);

    if (ret != HAL_OK) {
        return -1; // Вернуть ошибку, если чтение не удалось
    }

    // Вычисление контрольной суммы (пользовательский CRC)
    int16_t sum = 0; // Изменено на int16_t
    for (int i = 0; i < ILI211X_TOUCH_DATA_LEN - 1; i++) {
        sum = (sum + data[i]) & 0xff;
    }

    if ((-sum & 0xff) != data[ILI211X_TOUCH_DATA_LEN - 1]) {
        return -1; // Ошибка контрольной суммы
    }

    return 0; // Успешное чтение
}

// Функция для парсинга данных касания в координаты
static bool ili211x_touchdata_to_coords(const uint8_t *touchdata, unsigned int finger, unsigned int *x, unsigned int *y, unsigned int *z) {
    if (!(touchdata[0] & (1 << finger))) // Проверка на наличие касания пальцем
        return false;

    *x = ((touchdata[1] & 0xF0) << 4) | touchdata[2]; // Извлечение координаты X
    *y = ((touchdata[1] & 0x0F) << 8) | touchdata[3]; // Извлечение координаты Y
    *z = touchdata[0]; //

    return true;
}

// Инициализация контроллера сенсорного экрана.
uint8_t ILI211X_Init(I2C_HandleTypeDef i2c_handle) {
    hi2c = i2c_handle; // Сохранение указателя на переданный обработчик.

    if (HAL_I2C_IsDeviceReady(&hi2c, ILI211X_ADDR, 5, HAL_MAX_DELAY) == HAL_OK) {
        // Отправка команд инициализации контроллеру.
        ILI211X_Write(ILI211X_REG_RESET, 0x00);
//        HAL_Delay(1); // Задержка после сброса.
        ILI211X_Write(ILI211X_REG_MODE, 0x01);
 //       HAL_Delay(1);
        return 0; // Инициализация успешна.
    }

    return 1; // Устройство не готово.
}

// Функция для записи данных в регистр драйвера сенсорной панели.
void ILI211X_Write(uint8_t reg_addr, uint8_t byte_to_write) {
    uint8_t data[2];
    data[0] = reg_addr;         // Адрес регистра.
    data[1] = byte_to_write;    // Значение для записи.

    HAL_I2C_Master_Transmit(&hi2c, ILI211X_ADDR, data, sizeof(data), ILI211X_TIMEOUT);
}

// Функция для чтения одного байта из регистра драйвера сенсорной панели.
uint8_t ILI211X_Read(uint8_t reg_addr) {
    uint8_t data;
    if (HAL_I2C_Mem_Read(&hi2c, ILI211X_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, &data, sizeof(data), ILI211X_TIMEOUT) == HAL_OK) {
        return data;
    }
    return 0; // Вернуть 0 в случае ошибки или если чтение не удалось.
}

// Функция для чтения нескольких байтов из регистра драйвера сенсорной панели.
void ILI211X_ReadMultiple(uint8_t reg_addr, uint8_t *buff, uint8_t num_of_bytes) {
    HAL_I2C_Mem_Read(&hi2c, ILI211X_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, buff, num_of_bytes, ILI211X_TIMEOUT);
}

// Функция для получения ID жеста от контроллера сенсорного экрана.
uint8_t ILI211X_GetGestureID(void) {
    return ILI211X_Read(ILI211X_GEST_ID);
}

// Функция для получения количества касаний от контроллера сенсорного экрана.
uint8_t ILI211X_GetNumOfTouches(void) {
    return ILI211X_Read(ILI211X_TD_STATUS);
}

// Функция для чтения данных одной точки касания от контроллера сенсорного экрана.
CTP_point_data ILI211X_ReadPoint(uint8_t point_num) {
    uint8_t data[ILI211X_TOUCH_DATA_LEN];

    if (ili211x_read_touch_data(data) < 0) {
        return (CTP_point_data){0}; // Вернуть пустую структуру в случае ошибки
    }

    CTP_point_data point = {0};

    ili211x_touchdata_to_coords(data, point_num, &point.x, &point.y, &point.z);

    point.id = (data[4] & 0xF0) >> 4;            // ID касания
//    point.pressure = data[5];                    // Уровень давления

    return point;
}

// Функция для установки размера дисплея (пример реализации).
void ILI211X_SetDisplaySize(uint16_t width , uint16_t height ) {
	uint8_t command[5];

	// Команда для установки размера дисплея (пример).
	command[0] = 0x10;
	command[1] = (width >> 8) & 0xFF;
	command[2] = width & 0xFF;
	command[3] = (height >> 8) & 0xFF;
	command[4] = height & 0xFF;

	// Отправка команды через I2C.
	HAL_I2C_Master_Transmit(&hi2c , ILI211X_ADDR , command , sizeof(command), ILI211X_TIMEOUT);
}
