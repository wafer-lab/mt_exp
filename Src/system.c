#include "system.h"
#include "system_def.h"
#include "main.h"
#include "string.h"


static device_serial_t serial;


static void system_uid_read(uid_t *puid)
{
    uid_t *mcu_uid = (uid_t *)UID_BASE;
    
    puid->p0 = mcu_uid->p0;
    puid->p1 = mcu_uid->p1;
    puid->p2 = mcu_uid->p2;
    puid->p3 = mcu_uid->p3;
}


void system_serial_init(void)
{
    serial.header = 'MT';
    serial.footer = 'PE';
    
    system_uid_read(&serial.unique_id);
}


void system_serial_read(uint8_t *buf)
{
    uint8_t *ptr = (uint8_t *)&serial;
    
    memcpy(buf, ptr, SERIAL_LENGTH);
    
//    for (uint8_t i = 0; i < SERIAL_LENGTH; i++)
//        buf[i] = ptr[i];
}
