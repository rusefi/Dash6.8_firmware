/*
 * commands.c
 *
 *  Created on: Jun 1, 2025
 *      Author: Raman
 */

#include "commands.h"
#include "level_tables.h"
#include "Globals.h"
#include <string.h>
#include "usart3_json_dma.h"
#include "json_dispatcher.h"
#include "cJSON.h"

#include "main.h"

// Временные буферы для приёма новых таблиц




void handle_command(const Command_t* cmd) {
    switch ((CommandIDs)cmd->command_id) {
        case CMD_SAVE_FUEL_TABLE:  // сохранить во флеш таблицу уровня топлива
            fuel_level_save_table();

            break;

        case CMD_SAVE_OIL_TABLE:   // сохранить во флеш таблицу уровня масла
        	oil_level_save_table();

            break;

        case CMD_READ_FUEL_TABLE:  // считать и отправить в USART3 таблицу уровня топлива
        	char* fuel_table = build_data_json(1, 16, fuelLevelTable);
              send_json_over_uart(fuel_table);

            break;

        case CMD_READ_OIL_TABLE:   //считать и отправить в USART3 таблицу уровня масла
            char* oil_table = build_data_json(2, 16, oilLevelTable);
            send_json_over_uart(oil_table);
            break;

        case CMD_SAVE_MOTOHOURS:   //отправить моточасы в USART3


            break;

        case CMD_READ_MOTOHOURS:   //считать и отправить моточасы в USART3

            break;

        case CMD_READ_DATA:        //считать и отправить все данные
            char* updat = build_status_json();
            send_json_over_uart(updat);
            break;

        default:
//            log_warning("Unknown command: 0x%02X", cmd->command_id);
            break;
    }
}

void send_command(CommandIDs cmd_id, const uint8_t* params, uint8_t params_length) {
    Command_t cmd;
    cmd.command_id = (uint8_t)cmd_id;
    cmd.params_length = params_length > 16 ? 16 : params_length;
    if (params && cmd.params_length) {
        for (uint8_t i = 0; i < cmd.params_length; ++i)
            cmd.params[i] = params[i];
    }
    // send_json_command(&cmd); // Реализуйте функцию отправки команды
}
