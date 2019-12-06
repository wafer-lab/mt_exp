#ifndef _PORT_H_
#define _PORT_H_

#include <stdint.h>


typedef enum _in_port in_port_t;
enum _in_port {
    PORT2 = 2,
    PORT3 = 3,
    IN_UNKNOWN = 255
};

typedef enum _out_port out_port_t;
enum _out_port {
    PORT0 = 0,
    PORT1 = 1,
    OUT_UNKNOWN = 255
};


void port_init(void);

void port_write(out_port_t out_port, uint8_t value);

uint8_t port_read(in_port_t in_port);

out_port_t port_get_out_port(uint8_t raw);

in_port_t port_get_in_port(uint8_t raw);

#endif
