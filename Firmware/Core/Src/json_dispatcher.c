/*
 *  json_dispatcher.c
 *
 *  Created on: May 22, 2025
 *      Author: Raman
 */




#include "json_dispatcher.h"
#include "cJSON.h"
#include <string.h>

// Макросы для безопасного извлечения чисел
#define GET_INT(obj, key, def) (cJSON_HasObjectItem(obj, key) && cJSON_IsNumber(cJSON_GetObjectItem(obj, key)) ? cJSON_GetObjectItem(obj, key)->valueint : (def))
#define GET_DBL(obj, key, def) (cJSON_HasObjectItem(obj, key) && cJSON_IsNumber(cJSON_GetObjectItem(obj, key)) ? cJSON_GetObjectItem(obj, key)->valuedouble : (def))

int parse_status_json(const char* json_str, Statuses* out_status) {
    if (!json_str || !out_status) return 0;
    cJSON *root = cJSON_Parse(json_str);
    if (!root) return 0;
    cJSON *payload = cJSON_GetObjectItem(root, "payload");
    if (!cJSON_IsObject(payload)) { cJSON_Delete(root); return 0; }

    // Пример заполнения только части полей (добавьте остальные по необходимости!)
    out_status->RPM          = GET_INT(payload, "rpm", 0);
    out_status->MAP          = GET_DBL(payload, "map", 0.0);
    out_status->TPS          = GET_DBL(payload, "tps", 0.0);
    out_status->AFR          = GET_DBL(payload, "afr", 0.0);
    out_status->IgnitionTiming = GET_DBL(payload, "timing", 0.0);
    out_status->InjDuty      = GET_DBL(payload, "injDuty", 0.0);
    out_status->VVTPos       = GET_DBL(payload, "vvt", 0.0);
    out_status->VehicleSpeed = GET_INT(payload, "speed", 0);
    out_status->CoolantTemp  = GET_INT(payload, "clt", 0);
    out_status->IntakeTemp   = GET_INT(payload, "iat", 0);
    out_status->OilTemperature = GET_INT(payload, "oilTemp", 0);
    out_status->OilPress     = GET_DBL(payload, "oilPress", 0.0);
    out_status->FuelPress    = GET_DBL(payload, "fuelPress", 0.0);
    out_status->FuelLevel    = GET_DBL(payload, "fuelLevel", 0.0);
    out_status->FuelFlow     = GET_DBL(payload, "fuelFlow", 0.0);
    out_status->Lambda       = GET_DBL(payload, "lambda", 0.0);
//    out_status->statusFlags  = GET_INT(payload, "statusFlags", 0);
    out_status->LastError    = GET_INT(payload, "errorCode", 0);
    out_status->BattVolt     = GET_DBL(payload, "batteryVoltage", 0.0);
    out_status->KnockCt      = GET_INT(payload, "knockLevel", 0);

    cJSON_Delete(root);
    return 1;
}

int parse_flags_json(const char* json_str, Flags_t* out_flags) {
    if (!json_str || !out_flags) return 0;
    cJSON *root = cJSON_Parse(json_str);
    if (!root) return 0;
    cJSON *payload = cJSON_GetObjectItem(root, "payload");
    if (!cJSON_IsObject(payload)) { cJSON_Delete(root); return 0; }

    out_flags->motor_overheat     = GET_INT(payload, "motor_overheat", 0);
    out_flags->low_battery        = GET_INT(payload, "low_battery", 0);
    out_flags->sensor_error       = GET_INT(payload, "sensor_error", 0);
    out_flags->communication_lost = GET_INT(payload, "communication_lost", 0);
    out_flags->CAN_RUSEFI         = GET_INT(payload, "CAN_RUSEFI", 0);
    out_flags->CAN_ABIT           = GET_INT(payload, "CAN_ABIT", 0);
    out_flags->FUEL_CAN           = GET_INT(payload, "FUEL_CAN", 0);
    out_flags->FUEL_ADC           = GET_INT(payload, "FUEL_ADC", 0);
    // ... остальные флаги по необходимости ...
    cJSON_Delete(root);
    return 1;
}

int parse_command_json(const char* json_str, Command_t* out_cmd) {
    if (!json_str || !out_cmd) return 0;
    cJSON *root = cJSON_Parse(json_str);
    if (!root) return 0;
    cJSON *payload = cJSON_GetObjectItem(root, "payload");
    if (!cJSON_IsObject(payload)) { cJSON_Delete(root); return 0; }

    out_cmd->command_id = GET_INT(payload, "command_id", 0);
    cJSON *params = cJSON_GetObjectItem(payload, "params");
    if (cJSON_IsArray(params)) {
        int count = cJSON_GetArraySize(params);
        out_cmd->params_length = (count > 16) ? 16 : count;
        for (int i = 0; i < out_cmd->params_length; ++i)
            out_cmd->params[i] = cJSON_GetArrayItem(params, i)->valueint;
    } else {
        out_cmd->params_length = 0;
    }
    cJSON_Delete(root);
    return 1;
}

int parse_data_json(const char* json_str, Data_t* out_data) {
    if (!json_str || !out_data) return 0;
    cJSON *root = cJSON_Parse(json_str);
    if (!root) return 0;
    cJSON *payload = cJSON_GetObjectItem(root, "payload");
    if (!cJSON_IsObject(payload)) { cJSON_Delete(root); return 0; }

    cJSON *buffer = cJSON_GetObjectItem(payload, "data_buffer");
    if (cJSON_IsArray(buffer)) {
        int count = cJSON_GetArraySize(buffer);
        out_data->data_length = (count > 64) ? 64 : count;
        for (int i = 0; i < out_data->data_length; ++i)
            out_data->data_buffer[i] = cJSON_GetArrayItem(buffer, i)->valueint;
    } else {
        out_data->data_length = 0;
    }
    cJSON_Delete(root);
    return 1;
}

int parse_request_json(const char* json_str, Request_t* out_req) {
    if (!json_str || !out_req) return 0;
    cJSON *root = cJSON_Parse(json_str);
    if (!root) return 0;
    cJSON *payload = cJSON_GetObjectItem(root, "payload");
    if (!cJSON_IsObject(payload)) { cJSON_Delete(root); return 0; }

    out_req->request_type = GET_INT(payload, "request_type", 0);
    cJSON *params = cJSON_GetObjectItem(payload, "request_params");
    if (cJSON_IsArray(params)) {
        int count = cJSON_GetArraySize(params);
        out_req->params_length = (count > 16) ? 16 : count;
        for (int i = 0; i < out_req->params_length; ++i)
            out_req->request_params[i] = cJSON_GetArrayItem(params, i)->valueint;
    } else {
        out_req->params_length = 0;
    }
    cJSON_Delete(root);
    return 1;
}

int parse_and_dispatch_json(const char* json_str, JsonMessage* out_msg) {
    if (!json_str || !out_msg) return 0;
    cJSON *root = cJSON_Parse(json_str);
    if (!root) return 0;
    cJSON *type = cJSON_GetObjectItem(root, "type");
    if (!cJSON_IsString(type)) { cJSON_Delete(root); return 0; }

    if (strcmp(type->valuestring, "update") == 0) {
        out_msg->type = JSON_TYPE_UPDATE;
        int res = parse_status_json(json_str, &out_msg->payload.status);
        cJSON_Delete(root);
        return res;
    } else if (strcmp(type->valuestring, "Flags") == 0) {
        out_msg->type = JSON_TYPE_FLAGS;
        int res = parse_flags_json(json_str, &out_msg->payload.flags);
        cJSON_Delete(root);
        return res;
    } else if (strcmp(type->valuestring, "Command") == 0) {
        out_msg->type = JSON_TYPE_COMMAND;
        int res = parse_command_json(json_str, &out_msg->payload.command);
        cJSON_Delete(root);
        return res;
    } else if (strcmp(type->valuestring, "Data") == 0) {
        out_msg->type = JSON_TYPE_DATA;
        int res = parse_data_json(json_str, &out_msg->payload.data);
        cJSON_Delete(root);
        return res;
    } else if (strcmp(type->valuestring, "Request") == 0) {
        out_msg->type = JSON_TYPE_REQUEST;
        int res = parse_request_json(json_str, &out_msg->payload.request);
        cJSON_Delete(root);
        return res;
    }

    out_msg->type = JSON_TYPE_UNKNOWN;
    cJSON_Delete(root);
    return 0;
}
