#include "cmd.h"
#include "cmd_def.h"
#include "system.h"
#include "port.h"
#include "usbd_cdc_if.h"


int8_t port_write_handler(uint8_t *buf, uint8_t size);

int8_t port_read_handler(uint8_t *buf, uint8_t size);

int8_t serial_get_handler(uint8_t *buf, uint8_t size);


static void prepare_resp_buf(uint8_t *resp_buf, uint8_t buf_size, uint8_t data_size)
{
    cmd_desc_t *start, *end;
    
    start = (cmd_desc_t *)resp_buf;
    end = (cmd_desc_t *)(resp_buf + buf_size - 1);
    
    start->cmd = CMD_RESPONSE | CMD_MARKER_START;
    end->cmd = CMD_RESPONSE | CMD_MARKER_END;
    start->data_len = data_size;
    end->data_len = data_size;
}


int8_t cmd_parse(uint8_t *cmd_buf, uint32_t size)
{
    cmd_desc_t *start, *end;
    int8_t res;
    
    start = (cmd_desc_t *)cmd_buf;
    if ((start->cmd & CMD_MARKER_MASK) == CMD_MARKER_START)
    {
        if (size >= start->data_len + 2)
        {
            end = (cmd_desc_t *)(cmd_buf + start->data_len + 1);
            if (end->data_len == start->data_len)
            {
                if ((end->cmd & CMD_MARKER_MASK) == CMD_MARKER_END)
                {
                    if ((start->cmd & CMD_MASK) == (end->cmd & CMD_MASK))
                    {
                        switch (start->cmd & CMD_MASK)
                        {
                        case CMD_PORT_WRITE:
                            res = port_write_handler(cmd_buf + 1, start->data_len);
                            if (res != 0)
                                return -7;
                            break;
                        
                        case CMD_PORT_READ:
                            res = port_read_handler(cmd_buf + 1, start->data_len);
                            if (res != 0)
                                return -7;
                            break;
                        
                        case CMD_SERIAL_GET:
                            res = serial_get_handler(cmd_buf + 1, start->data_len);
                            if (res != 0)
                                return -7;
                            break;
                        
                        default:
                            return -6;
                        }
                    }
                    else
                        return -5;
                }
                else
                    return -4;
            }
            else
                return -3;
        }
        else
            return -2;
    }
    else
        return -1;
    
    return 0;
}


void cmd_send_response_failed(int8_t err_code)
{
    uint8_t resp_buf[CMD_RESPONSE_FAILED_LENGTH];
    prepare_resp_buf(resp_buf, CMD_RESPONSE_FAILED_LENGTH, 1);
    
    resp_buf[1] = (uint8_t)err_code;
    CDC_Transmit_FS(resp_buf, CMD_RESPONSE_FAILED_LENGTH);
}


int8_t port_write_handler(uint8_t *buf, uint8_t size)
{
    if (size == 2)
    {
        out_port_t port = port_get_out_port(buf[0]);
        if (port != OUT_UNKNOWN)
        {
            uint8_t resp_buf[CMD_RESPONSE_PORT_WRITE_LENGTH];
            prepare_resp_buf(resp_buf, CMD_RESPONSE_PORT_WRITE_LENGTH, 1);
            
            port_write(port, buf[1]);
            
            resp_buf[1] = (uint8_t)port | (CMD_PORT_WRITE << CMD_SHIFT);
            
            CDC_Transmit_FS(resp_buf, CMD_RESPONSE_PORT_WRITE_LENGTH);
        }
        else
            return -2;
    }
    else
        return -1;
    
    return 0;
}


int8_t port_read_handler(uint8_t *buf, uint8_t size)
{
    if (size == 1)
    {
        in_port_t port = port_get_in_port(buf[0]);
        if (port != IN_UNKNOWN)
        {
            uint8_t resp_buf[CMD_RESPONSE_PORT_READ_LENGTH];
            prepare_resp_buf(resp_buf, CMD_RESPONSE_PORT_READ_LENGTH, 2);
            
            uint8_t port_val;
            
            port_val = port_read(port);
            
            resp_buf[1] = (uint8_t)port | (CMD_PORT_READ << CMD_SHIFT);
            resp_buf[2] = port_val;
            
            CDC_Transmit_FS(resp_buf, CMD_RESPONSE_PORT_READ_LENGTH);
        }
        else
            return -2;
    }
    else
        return -1;
    
    return 0;
}


int8_t serial_get_handler(uint8_t *buf, uint8_t size)
{
    if (size == 0)
    {
        uint8_t resp_buf[CMD_RESPONSE_SERIAL_GET_LENGTH];
        prepare_resp_buf(resp_buf, CMD_RESPONSE_SERIAL_GET_LENGTH, SERIAL_LENGTH);
        
        system_serial_read(resp_buf + 1);
        
        CDC_Transmit_FS(resp_buf, CMD_RESPONSE_SERIAL_GET_LENGTH);
    }
    else
        return -1;
    
    return 0;
}
