#ifndef __GLOBALS_H
#define __GLOBALS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>
#include <Def/CAN_Def.h>
#include <Def/UNIT_Def.h>
#include <Def/IO_Def.h>
#include <Def/STORAGE_Def.h>
#include <Def/SCREEN_Def.h>

#define CAN_SETUP_ID			0x580

#define LED_DEFAULT_BRIGHTNESS	50
#define LCD_DEFAULT_BRIGHTNESS	800

#define PROTECTION_RPM_LOW 		6500
#define PROTECTION_RPM_HIGH 	8000
#define PROTECTION_RPM_LED 		6

#define PROTECTION_OIL_LOW 		40
#define PROTECTION_FUEL_LOW 	40

#define PSI_TO_BAR 				0.0689476f
#define PSI_TO_KPA 				6.89476f
#define BAR_TO_PSI 				14.5038f
#define BAR_TO_KPA 				100.0f
#define KPA_TO_BAR 				0.01f
#define KPA_TO_PSI 				0.145038f

#define AFR_TO_LAMBDA 			0.06802721088f

#define USE_1024x600
//#define USE_800x480

#ifdef USE_1024x600
	#define LCD_RES_X  1024
	#define LCD_RES_Y  600
#else
	#define LCD_RES_X  800
	#define LCD_RES_Y  480
#endif

typedef enum {
	CAN_LINK = 0,
	CAN_AIM,
	CAN_MX5,
	CAN_BMW_PHEV
} CANDefEnum;


struct RGBLED {
	uint8_t enabled;
	int color;
};

typedef struct {

	// BO_3221225472 VECTOR__INDEPENDENT_SIG_MSG
	  float AFR;                 // SG_ AFR : 7|16@0+ (0.001,0) [0|0] "AFR"
	  float VVTPos;              // SG_ VVTPos : 24|16@1- (0.02,0) [0|0] "deg"
	  uint8_t NewSignal_0010;    // SG_NewSignal_0010 : 24|8@1+ (1,0) [0|0] ""
	  uint8_t NewSignal_0009;    // SG_NewSignal_0009 : 16|8@1+ (1,0) [0|0] ""
	  uint8_t NewSignal_0008;    // SG_NewSignal_0008 : 8|8@1+ (1,0) [0|0] ""
	  uint8_t NewSignal_0015;    // SG_NewSignal_0015 : 8|8@1+ (1,0) [0|0] ""
	  uint8_t NewSignal_0016;    // SG_NewSignal_0016 : 16|8@1+ (1,0) [0|0] ""
	  uint8_t NewSignal_0024;    // SG_NewSignal_0024 : 0|8@1+ (1,0) [0|0] ""

	  // BO_512 BASE0
	  uint16_t WarningCounter;   // SG_ WarningCounter : 0|16@1+ (1,0) [0|0] ""
	  uint16_t LastError;        // SG_ LastError : 16|16@1+ (1,0) [0|0] ""
	  uint8_t RevLimAct;         // SG_ RevLimAct : 32|1@1+ (1,0) [0|0] ""
	  uint8_t MainRelayAct;      // SG_ MainRelayAct : 33|1@1+ (1,0) [0|0] ""
	  uint8_t FuelPumpAct;       // SG_ FuelPumpAct : 34|1@1+ (1,0) [0|0] ""
	  uint8_t CELAct;            // SG_ CELAct : 35|1@1+ (1,0) [0|0] ""
	  uint8_t EGOHeatAct;        // SG_ EGOHeatAct : 36|1@1+ (1,0) [0|0] ""
	  uint8_t LambdaProtectAct;  // SG_ LambdaProtectAct : 37|1@1+ (1,0) [0|0] ""
	  uint8_t Fan;               // SG_ Fan : 38|1@1+ (1,0) [0|0] ""
	  uint8_t Fan2;              // SG_ Fan2 : 39|1@1+ (1,0) [0|0] ""
	  uint8_t CurrentGear;       // SG_ CurrentGear : 40|8@1+ (1,0) [0|0] ""
	  float DistanceTraveled;    // SG_ DistanceTraveled : 48|16@1+ (0.1,0) [0|6553.5] "km"

	  // BO_513 BASE1
	  uint16_t RPM;              // SG_ RPM : 0|16@1+ (1,0) [0|0] "RPM"
	  uint16_t RPMs;
	  float IgnitionTiming;      // SG_ IgnitionTiming : 16|16@1- (0.02,0) [0|0] "deg"
	  float InjDuty;             // SG_ InjDuty : 32|8@1+ (0.5,0) [0|100] "%"
	  float IgnDuty;             // SG_ IgnDuty : 40|8@1+ (0.5,0) [0|100] "%"
	  uint8_t VehicleSpeed;      // SG_ VehicleSpeed : 48|8@1+ (1,0) [0|255] "kph"
	  uint8_t FlexPct;           // SG_ FlexPct : 56|8@1+ (1,0) [0|100] "%"

	  // BO_514 BASE2
	  float PPS;                 // SG_ PPS : 0|16@1- (0.01,0) [0|100] "%"
	  float TPS1;                // SG_ TPS1 : 16|16@1- (0.01,0) [0|100] "%"
	  float TPS2;                // SG_ TPS2 : 32|16@1- (0.01,0) [0|100] "%"
	  float Wastegate;           // SG_ Wastegate : 48|16@1- (0.01,0) [0|100] "%"

	  // BO_515 BASE3
	  float MAP;                 // SG_ MAP : 0|16@1+ (0.03333333,0) [0|0] "kPa"
	  int8_t CoolantTemp;        // SG_ CoolantTemp : 16|8@1+ (1,-40) [-40|200] "deg C"
	  int8_t IntakeTemp;         // SG_ IntakeTemp : 24|8@1+ (1,-40) [-40|200] "deg C"
	  int8_t AUX1Temp;           // SG_ AUX1Temp : 32|8@1+ (1,-40) [-40|200] "deg C"
	  int8_t AUX2Temp;           // SG_ AUX2Temp : 40|8@1+ (1,-40) [-40|200] "deg C"
	  int8_t MCUTemp;            // SG_ MCUTemp : 48|8@1+ (1,-40) [-40|100] "deg C"
	  float FuelLevel;           // SG_ FuelLevel : 56|8@1+ (0.5,0) [0|0] "%"

	  // BO_516 BASE4
	  float OilPress;            // SG_ OilPress : 16|16@1+ (0.03333333,0) [0|0] "kPa"
	  int8_t OilTemperature;     // SG_ OilTemperature : 32|8@1+ (1,-40) [-40|215] "deg C"
	  int8_t FuelTemperature;    // SG_ FuelTemperature : 40|8@1+ (1,-40) [-40|215] "deg C"
	  float BattVolt;            // SG_ BattVolt : 48|16@1+ (0.001,0) [0|25] "mV"

	  // BO_517 BASE5
	  uint16_t CylAM;            // SG_ CylAM : 0|16@1+ (1,0) [0|0] "mg"
	  float EstMAF;              // SG_ EstMAF : 16|16@1+ (0.01,0) [0|0] "kg/h"
	  float InjPW;               // SG_ InjPW : 32|16@1+ (0.003333333,0) [0|0] "ms"
	  uint16_t KnockCt;          // SG_ KnockCt : 48|16@1+ (1,0) [0|0] "count"

	  // BO_518 BASE6
	  uint16_t FuelUsed;         // SG_ FuelUsed : 0|16@1+ (1,0) [0|0] "g"
	  float FuelFlow;            // SG_ FuelFlow : 16|16@1+ (0.005,0) [0|327] "g/s"
	  float FuelTrim1;           // SG_ FuelTrim1 : 32|16@1+ (0.01,0) [-50|50] "%"
	  float FuelTrim2;           // SG_ FuelTrim2 : 48|16@1+ (0.01,0) [-50|50] "%"

	  // BO_519 BASE7
	  float Lam1;                // SG_ Lam1 : 0|16@1+ (0.0001,0) [0|2] "lambda"
	  float Lam2;                // SG_ Lam2 : 16|16@1+ (0.0001,0) [0|2] "lambda"
	  float FpLow;               // SG_ FpLow : 32|16@1+ (0.03333333,0) [0|0] "kPa"
	  float FpHigh;              // SG_ FpHigh : 48|16@1+ (0.1,0) [0|0] "bar"

	  // BO_520 BASE8
	  int8_t Cam1I;              // SG_ Cam1I : 0|8@1- (1,0) [-100|100] "deg"
	  int8_t Cam1Itar;           // SG_ Cam1Itar : 8|8@1- (1,0) [-100|100] "deg"
	  int8_t Cam1E;              // SG_ Cam1E : 16|8@1- (1,0) [-100|100] "deg"
	  int8_t Cam1Etar;           // SG_ Cam1Etar : 24|8@1- (1,0) [-100|100] "deg"
	  int8_t Cam2I;              // SG_ Cam2I : 32|8@1- (1,0) [-100|100] "deg"
	  int8_t Cam2Itar;           // SG_ Cam2Itar : 40|8@1- (1,0) [-100|100] "deg"
	  int8_t Cam2E;              // SG_ Cam2E : 48|8@1- (1,0) [-100|100] "deg"
	  int8_t Cam2Etar;           // SG_ Cam2Etar : 56|8@1- (1,0) [-100|100] "deg"

	  uint8_t CAN_ENABLED;
	  uint8_t CAN_PROTOCOL;

// дополнения влад


	  // BO_521 BASE8
	  	  int16_t WarningCounterv;              // SG_ Cam1I : 0|8@1- (1,0) [-100|100] "deg"
	  	  int16_t LastErrorv;           // SG_ Cam1Itar : 8|8@1- (1,0) [-100|100] "deg"
	  	  int16_t NextOBD2Error;              // SG_ Cam1E : 16|8@1- (1,0) [-100|100] "deg"



	int16_t FUELUSEDs;


	uint16_t GPS_LATITUDE;
	uint16_t GPS_LONGITUDE;
	uint16_t GPS_FIXED;
	uint16_t GPS_SATTELITES;
	uint16_t GPS_ALTITUDE;
	uint16_t GPS_SPEED;


	uint8_t ENGINE_PROTECTION;



	uint16_t IND_FUEL;
	uint16_t IND_OIL;
	uint16_t IND_BATT;

	uint16_t IND_DTC;
	uint16_t IND_ECT;


	uint16_t IND_ECT_BLUE;






	uint16_t LED_BRIGHTNESS;
	uint8_t LED_BRIGHTNESS_CHANGED;
	uint16_t LCD_BRIGHTNESS;
	uint8_t LCD_BRIGHTNESS_CHANGED;



	uint8_t CELL[16];

	SCREEN_Channel SCREEN_FIELDS[8];
	uint8_t SCREEN_FIELDS_CHANGED;

} Statuses;

uint8_t uartTransmitBufferSize;
uint8_t uartTransmitBuffer[128];

#ifdef __cplusplus
}
#endif

#endif // GLOBALS_H
