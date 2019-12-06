#ifndef _CMD_DEF_H_
#define _CMD_DEF_H_

#include <stdint.h>
#include "system_def.h"

#define CMD_SERIAL_GET                      0x03
#define CMD_PORT_WRITE                      0x02
#define CMD_PORT_READ                       0x01
#define CMD_RESPONSE                        0x00
#define CMD_MASK                            0x03
#define CMD_SHIFT                           0x05

#define CMD_MARKER_START                    0x00
#define CMD_MARKER_END                      0x04
#define CMD_MARKER_MASK                     0x04

#define CMD_RESPONSE_LENGTH                 0x02
#define CMD_RESPONSE_FAILED_LENGTH          CMD_RESPONSE_LENGTH + 1
#define CMD_RESPONSE_PORT_WRITE_LENGTH      CMD_RESPONSE_LENGTH + 1
#define CMD_RESPONSE_PORT_READ_LENGTH       CMD_RESPONSE_LENGTH + 2
#define CMD_RESPONSE_SERIAL_GET_LENGTH      CMD_RESPONSE_LENGTH + SERIAL_LENGTH


typedef struct _cmd_desc cmd_desc_t;
struct _cmd_desc {
    union {
        uint8_t raw_val;
        
        struct {
            uint8_t data_len : 5;
            uint8_t cmd : 3;
        };
    };
};

#endif
