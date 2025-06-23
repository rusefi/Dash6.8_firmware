/*
 * data.h
 *
 *  Created on: Jun 1, 2025
 *      Author: Raman
 */

#ifndef INC_DATA_H_
#define INC_DATA_H_

#include <stdint.h>
#include <stddef.h>

#define MAX_DATA_LENGTH      512

typedef enum {
    DATA_TYPE_NONE = 0,
    DATA_TYPE_FUEL_TABLE = 1,
    DATA_TYPE_OIL_TABLE  = 2,
    DATA_TYPE_MOTOHOURS  = 3
} DataType_t;


typedef struct {
    uint8_t  data_type;
    uint16_t data_length;
    float    data_buffer[MAX_DATA_LENGTH];
} Data_t;

void handle_data(const Data_t* data);
void send_motohours_data(void);

#endif /* INC_DATA_H_ */
