#ifndef _CMD_H_
#define _CMD_H_

#include <stdint.h>


int8_t cmd_parse(uint8_t *cmd_buf, uint32_t size);

void cmd_send_response_failed(int8_t err_code);

#endif
