#ifndef _SYSTEM_DEF_H_
#define _SYSTEM_DEF_H_

#include <stdint.h>


#pragma pack(1)

typedef struct _uid uid_t;
struct _uid {
    uint16_t p0;
    uint16_t p1;
    uint32_t p2;
    uint32_t p3;
};


typedef struct _dev_ser device_serial_t;
struct _dev_ser {
    uint16_t header;
    uid_t unique_id;
    uint16_t footer;
};

#define SERIAL_LENGTH       sizeof(device_serial_t)

#pragma pack()

#endif
