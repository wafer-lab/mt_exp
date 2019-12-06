#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <stdint.h>


void system_serial_init(void);

void system_serial_read(uint8_t *buf);

#endif
