#ifndef __GLOBALS_H
#define __GLOBALS_H

#ifdef __cplusplus
extern "C" {
#endif

//----------------------
// Подключение заголовков
//----------------------
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <Def/CAN_Def.h>
#include <Def/UNIT_Def.h>
#include <Def/IO_Def.h>
#include <Def/STORAGE_Def.h>
#include <Def/SCREEN_Def.h>

//----------------------
// Константы CAN и яркости
//----------------------
#define CAN_SETUP_ID                0x580

#define LED_DEFAULT_BRIGHTNESS      50
#define LCD_DEFAULT_BRIGHTNESS      1000

//----------------------
// Пороговые значения защиты
//----------------------
#define PROTECTION_RPM_LOW          6500
#define PROTECTION_RPM_HIGH         8000
#define PROTECTION_RPM_LED          6

#define PROTECTION_OIL_LOW          40
#define PROTECTION_FUEL_LOW         40

//----------------------
// Коэффициенты преобразования давления и AFR
//----------------------
#define PSI_TO_BAR                  0.0689476f
#define PSI_TO_KPA                  6.89476f
#define BAR_TO_PSI                  14.5038f
#define BAR_TO_KPA                  100.0f
#define KPA_TO_BAR                  0.01f
#define KPA_TO_PSI                  0.145038f

#define AFR_TO_LAMBDA               0.06802721088f

//----------------------
// Разрешение экрана (выберите нужное)
//----------------------
#define USE_1024x600
#define USE_800x480

#ifdef USE_1024x600
    #define LCD_RES_X               1280
    #define LCD_RES_Y               480
#else
    #define LCD_RES_X               800
    #define LCD_RES_Y               480
#endif

//----------------------
// Перечисление типов CAN
//----------------------
typedef enum {
    CAN_LINK = 0,
    CAN_AIM,
    CAN_MX5,
    CAN_BMW_PHEV
    // ... добавьте остальные типы
} CANDefEnum;

//----------------------
// Структура RGB-светодиода
//----------------------
typedef struct {
    uint8_t enabled;   // Включен ли светодиод
    int     color;     // Цвет (код)
} RGBLED_t;

//----------------------
// Флаги состояния (битовые поля)
//----------------------
typedef struct {
    uint32_t motor_overheat     : 1;  // Перегрев двигателя
    uint32_t low_battery        : 1;  // Низкий заряд батареи
    uint32_t sensor_error       : 1;  // Ошибка датчика
    uint32_t communication_lost : 1;  // Потеря связи
    uint32_t CAN_RUSEFI         : 1;  // CAN rusefi активен
    uint32_t CAN_ABIT           : 1;  // CAN abit активен
    uint32_t FUEL_CAN           : 1;  // CAN топливной системы
    uint32_t FUEL_ADC           : 1;  // ADC топливной системы
    // ... остальные флаги ...
    uint32_t reserved           : 24; // Зарезервировано
} Flags_t;

// Унион для доступа к флагам как к uint32_t
typedef union {
    Flags_t   flags;
    uint32_t  raw;
} FlagsUnion_t;

//----------------------
// Структура команды
//----------------------
typedef struct {
    uint8_t  command_id;                // Идентификатор команды
    uint8_t  params[16];                // Параметры команды
    uint8_t  params_length;             // Длина параметров
} Command_t;

//----------------------
// Структура данных
//----------------------
typedef struct {
    uint8_t   data_buffer[64];          // Буфер данных
    uint16_t  data_length;              // Длина данных
} Data_t;

//----------------------
// Структура запроса
//----------------------
typedef struct {
    uint8_t  request_type;              // Тип запроса
    uint8_t  request_params[16];        // Параметры запроса
    uint8_t  params_length;             // Длина параметров
} Request_t;

//----------------------
// Основная структура статусов системы
//----------------------
typedef struct {

    // --- CAN сообщения ---

    // BO_3221225472 VECTOR__INDEPENDENT_SIG_MSG
    float     AFR;                      // Коэффициент избытка воздуха
    float     VVTPos;                   // Положение фазовращателя, градусы
    uint8_t   NewSignal_0010;
    uint8_t   NewSignal_0009;
    uint8_t   NewSignal_0008;
    uint8_t   NewSignal_0015;
    uint8_t   NewSignal_0016;
    uint8_t   NewSignal_0024;

    // BO_512 BASE0
    uint16_t  WarningCounter;           // Счетчик предупреждений
    uint16_t  LastError;                // Последняя ошибка
    uint8_t   RevLimAct;                // Активен ограничитель оборотов
    uint8_t   MainRelayAct;             // Главное реле активно
    uint8_t   FuelPumpAct;              // Насос топлива активен
    uint8_t   CELAct;                   // Check Engine активен
    uint8_t   EGOHeatAct;               // Подогрев лямбда-зонда
    uint8_t   LambdaProtectAct;         // Защита по лямбде
    uint8_t   Fan;                      // Вентилятор 1
    uint8_t   Fan2;                     // Вентилятор 2
    uint8_t   CurrentGear;              // Текущая передача
    float     DistanceTraveled;         // Пройденная дистанция, км

    // BO_513 BASE1
    uint16_t  RPM;                      // Обороты двигателя
    uint16_t  RPMs;                     // Обороты (альтернативно)
    float     IgnitionTiming;           // Угол опережения зажигания
    float     InjDuty;                  // Скважность впрыска, %
    float     IgnDuty;                  // Скважность зажигания, %
    uint8_t   VehicleSpeed;             // Скорость, км/ч
    uint8_t   FlexPct;                  // Содержание этанола, %

    // BO_514 BASE2
    float     PPS;                      // Педаль акселератора, %
    float     TPS1;                     // Дроссель 1, %
    float     TPS2;                     // Дроссель 2, %
    float     Wastegate;                // Положение вестгейта, %

    // BO_515 BASE3
    float     MAP;                      // Давление во впуске, кПа
    int8_t    CoolantTemp;              // Температура ОЖ, °C
    int8_t    IntakeTemp;               // Температура воздуха, °C
    int8_t    AUX1Temp;                 // Температура AUX1, °C
    int8_t    AUX2Temp;                 // Температура AUX2, °C
    int8_t    MCUTemp;                  // Температура МК, °C
    float     FuelLevel;                // Уровень топлива, %

    // BO_516 BASE4
    float     OilPress;                 // Давление масла, кПа
    int8_t    OilTemperature;           // Температура масла, °C
    int8_t    FuelTemperature;          // Температура топлива, °C
    float     BattVolt;                 // Напряжение батареи, В

    // BO_517 BASE5
    uint16_t  CylAM;                    // Масса воздуха на цилиндр, мг
    float     EstMAF;                   // Оценка MAF, кг/ч
    float     InjPW;                    // Время впрыска, мс
    uint16_t  KnockCt;                  // Счетчик детонации

    // BO_518 BASE6
    uint16_t  FuelUsed;                 // Израсходовано топлива, г
    float     FuelFlow;                 // Расход топлива, г/с
    float     FuelTrim1;                // Коррекция топлива 1, %
    float     FuelTrim2;                // Коррекция топлива 2, %

    // BO_519 BASE7
    float     Lam1;                     // Лямбда 1
    float     Lam2;                     // Лямбда 2
    float     FpLow;                    // Давление топлива низкое, кПа
    float     FpHigh;                   // Давление топлива высокое, бар

    // BO_520 BASE8
    int8_t    Cam1I, Cam1Itar;          // Фазы распредвала 1 (факт/таргет)
    int8_t    Cam1E, Cam1Etar;          // Фазы распредвала 1 выпуск (факт/таргет)
    int8_t    Cam2I, Cam2Itar;          // Фазы распредвала 2 (факт/таргет)
    int8_t    Cam2E, Cam2Etar;          // Фазы распредвала 2 выпуск (факт/таргет)

    // --- ДОПОЛНИТЕЛЬНЫЕ ПОЛЯ ---
    // 0x281
    float     MAF;                      // Массовый расход воздуха, кг/ч
    float     Load;                     // Нагрузка двигателя, %

    // 0x282
    float     InjectorDuty;             // Скважность форсунок, %
    float     InjTime1;                 // Время впрыска 1 ряд, мс
    float     InjTime2;                 // Время впрыска 2 ряд, мс

    // 0x283
    float     TPS;                      // Положение дросселя, %
    uint16_t  IAC_Step;                 // Положение РХХ, шаг
    float     TPS_Req;                  // Требуемое положение дросселя, %
    float     Pedal;                    // Положение педали, %

    // 0x284
    float     Accel1, Accel2;           // Ускорения, G
    float     KnockCorr;                // Коррекция УОЗ по детонации

    // 0x381
    float     Lambda, O2_Voltage, LTFT, STFT;

    // 0x382
    float     KZ_Time;
    uint8_t   Gear, ErrorIndex;

    // 0x383
    float     Boost, BoostLimit, EGR_Valve;
    int16_t   TPS_Delta;

    // 0x481
    float     CamPos, LambdaReq, InjPhase1, InjPhase2;

    // 0x482
    float     FuelPress, CoolantPress;

    // 0x581
    int16_t   EGT1, EGT2, EGT3, EGT4;

    // 0x582
    float     FuelHour, FuelRoad;

    // 0x583
    int8_t    CoolantTempOut;
    int16_t   AtmoPress;

    // 0x584
    uint8_t   ADLM_OUT[16];
    uint8_t   ADLM_IN[12];

    // 0x781
    char      SW_Version[8];

    // --- Для принимаемых сообщений ---
    float     Lambda1, O2_Content1;
    bool      Sensor1_Ready;
    float     Lambda2, O2_Content2;
    bool      Sensor2_Ready;
    int16_t   EGT[4];
    float     LoggerBattVolt;
    int16_t   LoggerIntTemp, LoggerExtTemp;
    float     WheelSpeed[4];
    uint8_t   ADLM_INX[16];

    // --- Дополнения ---
    uint8_t   CAN_ENABLED;              // CAN включён
    uint8_t   CAN_PROTOCOL;             // Протокол CAN

    // BO_521 BASE8
    int16_t   WarningCounterv;
    int16_t   LastErrorv;
    int16_t   NextOBD2Error;

    float   FUELUSEDs;

    // GPS данные
    uint16_t  GPS_LATITUDE;
    uint16_t  GPS_LONGITUDE;
    uint16_t  GPS_FIXED;
    uint16_t  GPS_SATTELITES;
    uint16_t  GPS_ALTITUDE;
    uint16_t  GPS_SPEED;

    uint8_t   ENGINE_PROTECTION;        // Защита двигателя

    // Индикаторы
    uint16_t  IND_FUEL;
    uint16_t  IND_OIL;
    uint16_t  IND_BATT;
    uint16_t  IND_DTC;
    uint16_t  IND_ECT;
    uint16_t  IND_ECT_BLUE;
    uint16_t  indServis;
    uint32_t  MOTOHOURS;                // Моточасы

    // Яркость подсветки
    uint16_t  LED_BRIGHTNESS;
    uint8_t   LED_BRIGHTNESS_CHANGED;
    uint16_t  LCD_BRIGHTNESS;
    uint8_t   LCD_BRIGHTNESS_CHANGED;

    const char* Error_Mes;              // Сообщение об ошибке
    uint16_t    container1;
    uint16_t    container2;
    uint16_t    container3;
    uint16_t    container4;
    uint16_t    container5;
    uint16_t    container6;

    float       remains_km;

    uint8_t     CELL[16];               // Ячейки (например, для данных BMS)

    float     OilLevel;

    SCREEN_Channel SCREEN_FIELDS[8];    // Параметры экрана
    uint8_t     SCREEN_FIELDS_CHANGED;  // Флаг изменения экрана

} Statuses;

//----------------------
// Буфер передачи по UART
//----------------------
uint8_t uartTransmitBufferSize;
uint8_t uartTransmitBuffer[128];

#ifdef __cplusplus
}
#endif

#endif // __GLOBALS_H
