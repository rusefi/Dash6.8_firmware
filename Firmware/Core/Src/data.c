/*
 * data.c
 *
 *  Created on: Jun 1, 2025
 *      Author: Raman
 */
#include "data.h"
#include "level_tables.h"
#include <string.h>
//#include "extern.h"
#include "main.h"
#include "usart3_json_dma.h"

void handle_data(const Data_t* data) {
    switch (data->data_type) {
        case DATA_TYPE_FUEL_TABLE:
        	// Обновить таблицу топлива
        	fuel_level_set_table(data->data_buffer, data->data_length);
 //       	char* fuel_table2 = build_data_json(1, 16, fuelLevelTable);
 //       	          send_json_over_uart(fuel_table2);
            break;

        case DATA_TYPE_OIL_TABLE:
        	// Обновить таблицу масла
        	oil_level_set_table(data->data_buffer, data->data_length);
//        	char* oil_table2 = build_data_json(2, 16, oilLevelTable);
//        	            send_json_over_uart(oil_table2);
            break;

        case DATA_TYPE_MOTOHOURS:

            break;

        default:
 //           log_warning("Unknown data type: %d", data->data_type);   // логи
            break;
    }
}

void send_motohours_data(void) {
    Data_t data = {
        .data_type = DATA_TYPE_MOTOHOURS,
        .data_length = 4
    };
//    memcpy(data.data_buffer, &Current_Status.MOTOHOURS, 4);
//    send_json_data(&data); // Ваша функция отправки JSON  починить ил дабавить
}



