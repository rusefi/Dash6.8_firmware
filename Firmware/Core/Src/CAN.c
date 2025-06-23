/*
 * CAN.c
 *
 *  Created on: May 7, 2025
 *      Author: Raman
 */

#include"CAN.h"

void Handle_BO_3221225472(uint8_t *data) {
  // SG_ AFR : 7|16@0+ (0.001,0) [0|0] "AFR"
  uint16_t AFR_raw = EXTRACT_U16(data, 7);
  Current_Status.AFR = AFR_raw * 0.001;

  // SG_ VVTPos : 24|16@1- (0.02,0) [0|0] "deg"
  uint16_t VVTPos_raw = EXTRACT_U16(data, 24);
  Current_Status.VVTPos = (int16_t)VVTPos_raw * 0.02;

  // SG_ NewSignal_0010 : 24|8@1+ (1,0) [0|0] ""
  Current_Status.NewSignal_0010 = (data[3] >> 0) & 0xFF;

  // SG_ NewSignal_0009 : 16|8@1+ (1,0) [0|0] ""
  Current_Status.NewSignal_0009 = (data[2] >> 0) & 0xFF;

    // SG_ NewSignal_0008 : 8|8@1+ (1,0) [0|0] ""
  Current_Status.NewSignal_0008 = (data[1] >> 0) & 0xFF;

    // SG_ NewSignal_0015 : 8|8@1+ (1,0) [0|0] ""
  Current_Status.NewSignal_0015 = (data[1] >> 0) & 0xFF;

    // SG_ NewSignal_0016 : 16|8@1+ (1,0) [0|0] ""
  Current_Status.NewSignal_0016 = (data[2] >> 0) & 0xFF;

   // SG_ NewSignal_0024 : 0|8@1+ (1,0) [0|0] ""
  Current_Status.NewSignal_0024 = (data[0] >> 0) & 0xFF;
}

void Handle_BO_512(uint8_t *data) {
  // SG_ WarningCounter : 0|16@1+ (1,0) [0|0] ""
  Current_Status.WarningCounter = EXTRACT_U16(data, 0);

  // SG_ LastError : 16|16@1+ (1,0) [0|0] ""
  Current_Status.LastError = EXTRACT_U16(data, 16);

  // SG_ RevLimAct : 32|1@1+ (1,0) [0|0] ""
  Current_Status.RevLimAct = (data[4] >> 0) & 0x01;

  // SG_ MainRelayAct : 33|1@1+ (1,0) [0|0] ""
  Current_Status.MainRelayAct = (data[4] >> 1) & 0x01;

  // SG_ FuelPumpAct : 34|1@1+ (1,0) [0|0] ""
  Current_Status.FuelPumpAct = (data[4] >> 2) & 0x01;

  // SG_ CELAct : 35|1@1+ (1,0) [0|0] ""
  Current_Status.CELAct = (data[4] >> 3) & 0x01;

  // SG_ EGOHeatAct : 36|1@1+ (1,0) [0|0] ""
  Current_Status.EGOHeatAct = (data[4] >> 4) & 0x01;

  // SG_ LambdaProtectAct : 37|1@1+ (1,0) [0|0] ""
  Current_Status.LambdaProtectAct = (data[4] >> 5) & 0x01;

  // SG_ Fan : 38|1@1+ (1,0) [0|0] ""
  Current_Status.Fan = (data[4] >> 6) & 0x01;

  // SG_ Fan2 : 39|1@1+ (1,0) [0|0] ""
  Current_Status.Fan2 = (data[4] >> 7) & 0x01;

  // SG_ CurrentGear : 40|8@1+ (1,0) [0|0] ""
  Current_Status.CurrentGear = (data[5] >> 0) & 0xFF;

  // SG_ DistanceTraveled : 48|16@1+ (0.1,0) [0|6553.5] "km"
  uint16_t DistanceTraveled_raw = EXTRACT_U16(data, 48);
  Current_Status.DistanceTraveled = DistanceTraveled_raw * 0.1;
}

void Handle_BO_513(uint8_t *data) {
  // SG_ RPM : 0|16@1+ (1,0) [0|0] "RPM"
  Current_Status.RPM = EXTRACT_U16(data, 0);

  // SG_ IgnitionTiming : 16|16@1- (0.02,0) [0|0] "deg"
  uint16_t IgnitionTiming_raw = EXTRACT_U16(data, 16);
  Current_Status.IgnitionTiming = (int16_t)IgnitionTiming_raw * 0.02;

  // SG_ InjDuty : 32|8@1+ (0.5,0) [0|100] "%"
  Current_Status.InjDuty = ((data[4] >> 0) & 0xFF) * 0.5;

  // SG_ IgnDuty : 40|8@1+ (0.5,0) [0|100] "%"
  Current_Status.IgnDuty = ((data[5] >> 0) & 0xFF) * 0.5;

  // SG_ VehicleSpeed : 48|8@1+ (1,0) [0|255] "kph"
  Current_Status.VehicleSpeed = (data[6] >> 0) & 0xFF;

  // SG_ FlexPct : 56|8@1+ (1,0) [0|100] "%"
  Current_Status.FlexPct = (data[7] >> 0) & 0xFF;
}

void Handle_BO_514(uint8_t *data) {
  // SG_ PPS : 0|16@1- (0.01,0) [0|100] "%"
  uint16_t PPS_raw = EXTRACT_U16(data, 0);
  Current_Status.PPS = (int16_t)PPS_raw * 0.01;

  // SG_ TPS1 : 16|16@1- (0.01,0) [0|100] "%"
  uint16_t TPS1_raw = EXTRACT_U16(data, 16);
  Current_Status.TPS1 = (int16_t)TPS1_raw * 0.01;

  // SG_ TPS2 : 32|16@1- (0.01,0) [0|100] "%"
  uint16_t TPS2_raw = EXTRACT_U16(data, 32);
  Current_Status.TPS2 = (int16_t)TPS2_raw * 0.01;

  // SG_ Wastegate : 48|16@1- (0.01,0) [0|100] "%"
  uint16_t Wastegate_raw = EXTRACT_U16(data, 48);
  Current_Status.Wastegate = (int16_t)Wastegate_raw * 0.01;
}

void Handle_BO_515(uint8_t *data) {
  // SG_ MAP : 0|16@1+ (0.03333333,0) [0|0] "kPa"
  uint16_t MAP_raw = EXTRACT_U16(data, 0);
  Current_Status.MAP = MAP_raw * 0.03333333;

  // SG_ CoolantTemp : 16|8@1+ (1,-40) [-40|200] "deg C"
  Current_Status.CoolantTemp = ((data[2] >> 0) & 0xFF) * 1 - 40;

  // SG_ IntakeTemp : 24|8@1+ (1,-40) [-40|200] "deg C"
  Current_Status.IntakeTemp = ((data[3] >> 0) & 0xFF) * 1 - 40;

  // SG_ AUX1Temp : 32|8@1+ (1,-40) [-40|200] "deg C"
  Current_Status.AUX1Temp = ((data[4] >> 0) & 0xFF) * 1 - 40;

  // SG_ AUX2Temp : 40|8@1+ (1,-40) [-40|200] "deg C"
  Current_Status.AUX2Temp = ((data[5] >> 0) & 0xFF) * 1 - 40;

  // SG_ MCUTemp : 48|8@1+ (1,-40) [-40|100] "deg C"
  Current_Status.MCUTemp = ((data[6] >> 0) & 0xFF) * 1 - 40;

  // SG_ FuelLevel : 56|8@1+ (0.5,0) [0|0] "%"
  Current_Status.FuelLevelRaw = ((data[7] >> 0) & 0xFF) * 0.5;
}

void Handle_BO_516(uint8_t *data) {
  // SG_ OilPress : 16|16@1+ (0.03333333,0) [0|0] "kPa"
  uint16_t OilPress_raw = EXTRACT_U16(data, 16);
  Current_Status.OilPress = OilPress_raw * 0.03333333;

  // SG_ OilTemperature : 32|8@1+ (1,-40) [-40|215] "deg C"
  Current_Status.OilTemperature = ((data[4] >> 0) & 0xFF) * 1 - 40;

  // SG_ FuelTemperature : 40|8@1+ (1,-40) [-40|215] "deg C"
  Current_Status.FuelTemperature = ((data[5] >> 0) & 0xFF) * 1 - 40;

  // SG_ BattVolt : 48|16@1+ (0.001,0) [0|25] "mV"
  uint16_t BattVolt_raw = EXTRACT_U16(data, 48);
  Current_Status.BattVolt = BattVolt_raw * 0.001;
}

void Handle_BO_517(uint8_t *data) {
  // SG_ CylAM : 0|16@1+ (1,0) [0|0] "mg"
  Current_Status.CylAM = EXTRACT_U16(data, 0);

  // SG_ EstMAF : 16|16@1+ (0.01,0) [0|0] "kg/h"
  Current_Status.EstMAF = EXTRACT_U16(data, 16) * 0.01;

  // SG_ InjPW : 32|16@1+ (0.003333333,0) [0|0] "ms"
  Current_Status.InjPW = EXTRACT_U16(data, 32) * 0.003333333;

  // SG_ KnockCt : 48|16@1+ (1,0) [0|0] "count"
  Current_Status.KnockCt = EXTRACT_U16(data, 48);
}

void Handle_BO_518(uint8_t *data) {
  // SG_ FuelUsed : 0|16@1+ (1,0) [0|0] "g"
  Current_Status.FuelUsed = EXTRACT_U16(data, 0);

  // SG_ FuelFlow : 16|16@1+ (0.005,0) [0|327] "g/s"
  Current_Status.FuelFlow = EXTRACT_U16(data, 16) * 0.005;

  // SG_ FuelTrim1 : 32|16@1+ (0.01,0) [-50|50] "%"
  Current_Status.FuelTrim1 = EXTRACT_U16(data, 32) * 0.01;

  // SG_ FuelTrim2 : 48|16@1+ (0.01,0) [-50|50] "%"
  Current_Status.FuelTrim2 = EXTRACT_U16(data, 48) * 0.01;
}

void Handle_BO_519(uint8_t *data) {
  // SG_ Lam1 : 0|16@1+ (0.0001,0) [0|2] "lambda"
  Current_Status.Lam1 = EXTRACT_U16(data, 0) * 0.0001;

  // SG_ Lam2 : 16|16@1+ (0.0001,0) [0|2] "lambda"
  Current_Status.Lam2 = EXTRACT_U16(data, 16) * 0.0001;

  // SG_ FpLow : 32|16@1+ (0.03333333,0) [0|0] "kPa"
  Current_Status.FpLow = EXTRACT_U16(data, 32) * 0.03333333;

  // SG_ FpHigh : 48|16@1+ (0.1,0) [0|0] "bar"
  Current_Status.FpHigh = EXTRACT_U16(data, 48) * 0.1;
}

void Handle_BO_520(uint8_t *data) {
    // SG_Cam1I : 0|8@1- (1,0) [-100|100] "deg"
    Current_Status.Cam1I = (int8_t)(data[0]);

    // SG_Cam1Itar : 8|8@1- (1,0) [-100|100] "deg"
    Current_Status.Cam1Itar = (int8_t)(data[1]);

    // SG_Cam1E : 16|8@1- (1,0) [-100|100] "deg"
    Current_Status.Cam1E = (int8_t)(data[2]);

    // SG_Cam1Etar : 24|8@1- (1,0) [-100|100] "deg"
    Current_Status.Cam1Etar = (int8_t)(data[3]);

    // SG_Cam2I : 32|8@1- (1,0) [-100|100] "deg"
    Current_Status.Cam2I = (int8_t)(data[4]);

    // SG_Cam2Itar : 40|8@1- (1,0) [-100|100] "deg"
    Current_Status.Cam2Itar = (int8_t)(data[5]);

    // SG_Cam2E : 48|8@1- (1,0) [-100|100] "deg"
    Current_Status.Cam2E = (int8_t)(data[6]);

    // SG_Cam2Etar : 56|8@1- (1,0) [-100|100] "deg"
    Current_Status.Cam2Etar = (int8_t)(data[7]);
}
    void Handle_BO_521(uint8_t *data) {
        // SG_Cam1I : 0|8@1- (1,0) [-100|100] "deg"
     Current_Status.WarningCounterv = (int16_t)(data[0]);

        // SG_Cam1Itar : 8|8@1- (1,0) [-100|100] "deg"
     Current_Status.LastErrorv = (int16_t)(data[1]);

        // SG_Cam1E : 16|8@1- (1,0) [-100|100] "deg"
     Current_Status.NextOBD2Error = (int16_t)(data[2]);

}




    // 0x281
    void Handle_BO_0x281(uint8_t *data) {
        Current_Status.RPM  = EXTRACT_U16(data, 0);
        Current_Status.MAF  = EXTRACT_U16(data, 16) * 0.1f;
        Current_Status.MAP  = EXTRACT_U16(data, 32);
        Current_Status.Load = EXTRACT_U16(data, 48) * 0.1f;
    }

    // 0x282
    void Handle_BO_0x282(uint8_t *data) {
        Current_Status.IgnitionTiming = EXTRACT_S16(data, 0) * 0.1f;
        Current_Status.InjectorDuty    = EXTRACT_U16(data, 16) / 40960.0f;
        Current_Status.InjTime1        = EXTRACT_U16(data, 32) * 0.01f;
        Current_Status.InjTime2        = EXTRACT_U16(data, 48) * 0.01f;
    }

    // 0x283
    void Handle_BO_0x283(uint8_t *data) {
        Current_Status.TPS      = EXTRACT_U16(data, 0) * 0.1f;
        Current_Status.IAC_Step = EXTRACT_U16(data, 16);
        Current_Status.TPS_Req  = EXTRACT_U16(data, 32) * 0.1f;
        Current_Status.Pedal    = EXTRACT_U16(data, 48) * 0.1f;
    }

    // 0x284
    void Handle_BO_0x284(uint8_t *data) {
        Current_Status.Accel1    = EXTRACT_S16(data, 0) * 0.001f;
        Current_Status.Accel2    = EXTRACT_S16(data, 16) * 0.001f;
        Current_Status.KnockCorr = EXTRACT_S8(data, 32) * 0.1f; // байт 4
    }

    // 0x381
    void Handle_BO_0x381(uint8_t *data) {
        Current_Status.Lambda     = EXTRACT_U16(data, 0) * 0.001f;
        Current_Status.O2_Voltage = EXTRACT_U16(data, 16) * 5.0f / 1024.0f;
        Current_Status.LTFT       = EXTRACT_U16(data, 32) * 0.001f;
        Current_Status.STFT       = EXTRACT_U16(data, 48) * 0.001f;
    }

    // 0x382
    void Handle_BO_0x382(uint8_t *data) {
        Current_Status.VehicleSpeed = EXTRACT_U16(data, 0) / 16.0f;
        Current_Status.KZ_Time      = EXTRACT_U8(data, 16) * 0.1f;
        Current_Status.Gear         = EXTRACT_U8(data, 24);
        Current_Status.ErrorIndex   = EXTRACT_U8(data, 32);
    }

    // 0x383
    void Handle_BO_0x383(uint8_t *data) {
        Current_Status.Boost      = EXTRACT_S16(data, 0) * 0.001f;
        Current_Status.BoostLimit = EXTRACT_S16(data, 16) * 0.001f;
        Current_Status.EGR_Valve  = EXTRACT_U16(data, 32) * 0.1f;
        Current_Status.TPS_Delta  = EXTRACT_S16(data, 48);
    }

    // 0x481
    void Handle_BO_0x481(uint8_t *data) {
        Current_Status.CamPos     = EXTRACT_S16(data, 0) * 0.1f;
        Current_Status.LambdaReq  = EXTRACT_U16(data, 16) * 0.001f;
        Current_Status.InjPhase1  = EXTRACT_S16(data, 32) * 0.1f;
        Current_Status.InjPhase2  = EXTRACT_S16(data, 48) * 0.1f;
    }

    // 0x482
    void Handle_BO_0x482(uint8_t *data) {
        Current_Status.FuelPress    = EXTRACT_U16(data, 0) * 0.01f;
        Current_Status.OilPress     = EXTRACT_U16(data, 16) * 0.01f;
        Current_Status.CoolantPress = EXTRACT_U16(data, 32) * 0.01f;
    }

    // 0x581
    void Handle_BO_0x581(uint8_t *data) {
        Current_Status.EGT1      = EXTRACT_S16(data, 0);
        Current_Status.FuelTemperature  = EXTRACT_S16(data, 16);
        Current_Status.OilTemperature  = EXTRACT_S16(data, 32);
  //      Current_Status.OilTemp2  = EXTRACT_S16(data, 48);
    }

    // 0x582
    void Handle_BO_0x582(uint8_t *data) {
        Current_Status.FuelHour = EXTRACT_U16(data, 0) * 0.01f;
        Current_Status.FuelRoad = EXTRACT_U16(data, 16) * 0.1f;
        Current_Status.FuelUsed = EXTRACT_U16(data, 32) * 0.01f;
        Current_Status.BattVolt = EXTRACT_U16(data, 48) * 0.1f;
    }

    // 0x583
    void Handle_BO_0x583(uint8_t *data) {
        Current_Status.CoolantTemp     = (int8_t)EXTRACT_U8(data, 0) - 40;
        Current_Status.IntakeTemp   = (int8_t)EXTRACT_U8(data, 8) - 40;
        Current_Status.CoolantTempOut  = EXTRACT_S16(data, 32);
        Current_Status.AtmoPress       = EXTRACT_S16(data, 48);
    }

    // 0x584
    void Handle_BO_0x584(uint8_t *data) {
        for (int i = 0; i < 8; ++i)
            Current_Status.ADLM_OUT[i] = (data[0] >> i) & 0x01;
        for (int i = 0; i < 8; ++i)
            Current_Status.ADLM_OUT[8 + i] = (data[1] >> i) & 0x01;
        for (int i = 0; i < 8; ++i)
            Current_Status.ADLM_IN[i] = (data[2] >> i) & 0x01;
        for (int i = 0; i < 4; ++i)
            Current_Status.ADLM_IN[8 + i] = (data[3] >> i) & 0x01;
    }

    // 0x781
    void Handle_BO_0x781(uint8_t *data) {
        memcpy(Current_Status.SW_Version, data, 7);
        Current_Status.SW_Version[7] = 0;
    }


