#ifndef _PORT_H_
#define _PORT_H_

#include <stdint.h>


typedef enum _in_port in_port_t;
enum _in_port {
    PORT2_4_LOW = (2 << 2) | 1,
    PORT2_4_HIGH = (2 << 2) | 2,
    PORT2_8 = (2 << 2) | 3,
    
    PORT3_4_LOW = (3 << 2) | 1,
    PORT3_4_HIGH = (3 << 2) | 2,
    PORT3_8 = (3 << 2) | 3,
    
    IN_UNKNOWN = 255
};

typedef enum _out_port out_port_t;
enum _out_port {
    PORT0_4_LOW = (0 << 2) | 1,
    PORT0_4_HIGH = (0 << 2) | 2,
    PORT0_8 = (0 << 2) | 3,
    
    PORT1_4_LOW = (1 << 2) | 1,
    PORT1_4_HIGH = (1 << 2) | 2,
    PORT1_8 = (1 << 2) | 3,
    
    OUT_UNKNOWN = 255
};


void port_init(void);

void port_write(out_port_t out_port, uint8_t value);

uint8_t port_read(in_port_t in_port);

out_port_t port_get_out_port(uint8_t raw);

in_port_t port_get_in_port(uint8_t raw);

#endif
