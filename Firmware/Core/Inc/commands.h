/*
 * commands.h
 *
 *  Created on: Jun 1, 2025
 *      Author: Raman
 */

#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>
#include <stddef.h>

// Перечисление команд
typedef enum {
    CMD_SAVE_FUEL_TABLE   = 0x10,
    CMD_SAVE_OIL_TABLE    = 0x11,
    CMD_READ_FUEL_TABLE   = 0x12,
    CMD_READ_OIL_TABLE    = 0x13,
    CMD_SAVE_MOTOHOURS    = 0x20,
    CMD_READ_MOTOHOURS    = 0x21,
	CMD_READ_DATA         = 0x22
} CommandIDs;

// Структура команды
typedef struct {
    uint8_t  command_id;
    uint8_t  params[16];
    uint8_t  params_length;
} Command_t;


// Прототипы функций
void handle_command(const Command_t* cmd);
void send_command(CommandIDs cmd_id, const uint8_t* params, uint8_t params_length);


#endif // COMMANDS_H
