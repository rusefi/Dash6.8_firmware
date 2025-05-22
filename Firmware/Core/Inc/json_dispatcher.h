/*
 * json_dispatcher.h
 *
 *  Created on: May 22, 2025
 *      Author: Raman
 */

#ifndef INC_JSON_DISPATCHER_H_
#define INC_JSON_DISPATCHER_H_

#pragma once
#include "Globals.h"

// Тип сообщения
typedef enum {
    JSON_TYPE_UNKNOWN = 0,
    JSON_TYPE_UPDATE,
    JSON_TYPE_FLAGS,
    JSON_TYPE_COMMAND,
    JSON_TYPE_DATA,
    JSON_TYPE_REQUEST
} JsonType;

// Универсальный контейнер результата
typedef struct {
    JsonType type;
    union {
        Statuses   status;
        Flags_t    flags;
        Command_t  command;
        Data_t     data;
        Request_t  request;
    } payload;
} JsonMessage;

// Основная функция разбора и маршрутизации
// Возвращает 1 если успешно, 0 если ошибка
int parse_and_dispatch_json(const char* json_str, JsonMessage* out_msg);

/// Для каждого типа — отдельные функции (если нужно использовать напрямую)
int parse_status_json(const char* json_str, Statuses* out_status);
int parse_flags_json(const char* json_str, Flags_t* out_flags);
int parse_command_json(const char* json_str, Command_t* out_cmd);
int parse_data_json(const char* json_str, Data_t* out_data);
int parse_request_json(const char* json_str, Request_t* out_req);




#endif /* INC_JSON_DISPATCHER_H_ */
