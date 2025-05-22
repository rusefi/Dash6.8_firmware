/*
 * json_builder.c
 *
 *  Created on: May 7, 2025
 *      Author: Raman
 */


/*
#include "Globals.h"
#include "cJSON.h"
#include <stdlib.h>

// Предположим, что у вас есть глобальная или передаваемая структура Statuses
 Statuses Current_Status;

char* build_status_json(void) {
    cJSON *root = cJSON_CreateObject();
    if (!root) return NULL;

    cJSON_AddStringToObject(root, "type", "update");

    cJSON *payload = cJSON_CreateObject();
    if (!payload) {
        cJSON_Delete(root);
        return NULL;
    }

    // Добавляем поля из структуры, соответствующие вашему примеру JSON
    cJSON_AddNumberToObject(payload, "rpm", Current_Status.RPM);
    cJSON_AddNumberToObject(payload, "map", Current_Status.MAP);
    cJSON_AddNumberToObject(payload, "tps", Current_Status.TPS1);
    cJSON_AddNumberToObject(payload, "afr", Current_Status.AFR);
    cJSON_AddNumberToObject(payload, "timing", Current_Status.IgnitionTiming);
    cJSON_AddNumberToObject(payload, "injDuty", Current_Status.InjDuty);
    cJSON_AddNumberToObject(payload, "vvt", Current_Status.VVTPos);
    cJSON_AddNumberToObject(payload, "speed", Current_Status.VehicleSpeed);
    cJSON_AddNumberToObject(payload, "clt", Current_Status.CoolantTemp);
    cJSON_AddNumberToObject(payload, "iat", Current_Status.IntakeTemp);
    cJSON_AddNumberToObject(payload, "oilTemp", Current_Status.OilTemperature);
    cJSON_AddNumberToObject(payload, "oilPress", Current_Status.OilPress);
    cJSON_AddNumberToObject(payload, "fuelPress", Current_Status.FuelFlow); // В структуре FuelFlow - можно заменить на FuelPress если есть
    cJSON_AddNumberToObject(payload, "fuelLevel", Current_Status.FuelLevel);
    cJSON_AddNumberToObject(payload, "fuelFlow", Current_Status.FuelFlow);
    cJSON_AddNumberToObject(payload, "lambda", Current_Status.Lam1);
    cJSON_AddNumberToObject(payload, "statusFlags", Current_Status.ENGINE_PROTECTION);
    cJSON_AddNumberToObject(payload, "errorCode", Current_Status.LastError);
    cJSON_AddNumberToObject(payload, "batteryVoltage", Current_Status.BattVolt);
    cJSON_AddNumberToObject(payload, "knockLevel", Current_Status.KnockCt);

    cJSON_AddItemToObject(root, "payload", payload);

    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return json_str; // Освободить после использования через free()
}*/
