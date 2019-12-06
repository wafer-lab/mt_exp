#include "port.h"
#include "main.h"


#define P0_CP_LOW()                 GPIOB->BSRR = GPIO_BSRR_BR10
#define P0_CP_HIGH()                GPIOB->BSRR = GPIO_BSRR_BS10

#define P1_CP_LOW()                 GPIOB->BSRR = GPIO_BSRR_BR2
#define P1_CP_HIGH()                GPIOB->BSRR = GPIO_BSRR_BS2

#define P2_EN_LOW()                 GPIOB->BSRR = GPIO_BSRR_BR1
#define P2_EN_HIGH()                GPIOB->BSRR = GPIO_BSRR_BS1

#define P3_EN_LOW()                 GPIOB->BSRR = GPIO_BSRR_BR0
#define P3_EN_HIGH()                GPIOB->BSRR = GPIO_BSRR_BS0

#define LAST_VALUE(arr, out_port)   arr[(out_port >> 2) & 1]
#define DATA_WRITE(data)            GPIOA->ODR = (GPIOA->ODR & 0xFF00) | data
#define DATA_READ()                 GPIOA->IDR & 0x00FF


void port_init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN;
    
    P0_CP_LOW();
    P1_CP_LOW();
    P2_EN_HIGH();
    P3_EN_HIGH();
    
    GPIOB->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | 
                    GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_CNF2);
    GPIOB->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10);
    
    GPIOB->CRL |= GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2;
    GPIOB->CRH |= GPIO_CRH_MODE10;
}


static void port_set_dir_out()
{
    GPIOA->CRL = (GPIO_CRL_MODE0 | GPIO_CRL_MODE1 |
                  GPIO_CRL_MODE2 | GPIO_CRL_MODE3 |
                  GPIO_CRL_MODE4 | GPIO_CRL_MODE5 |
                  GPIO_CRL_MODE6 | GPIO_CRL_MODE7);
}


static void port_set_dir_in()
{
    GPIOA->CRL = (GPIO_CRL_CNF0_0 | GPIO_CRL_CNF1_0 |
                  GPIO_CRL_CNF2_0 | GPIO_CRL_CNF3_0 |
                  GPIO_CRL_CNF4_0 | GPIO_CRL_CNF5_0 |
                  GPIO_CRL_CNF6_0 | GPIO_CRL_CNF7_0);
}


void port_write(out_port_t out_port, uint8_t value)
{
    static uint8_t port_lv[2] = { 0 };
    
    switch(out_port)
    {
    case PORT0_4_LOW:
    case PORT1_4_LOW:
        value = (value & 0x0F) | (LAST_VALUE(port_lv, out_port) & 0xF0);
        break;
        
    case PORT0_4_HIGH:
    case PORT1_4_HIGH:
        value = (value & 0xF0) | (LAST_VALUE(port_lv, out_port) & 0x0F);
        break;
    
    case PORT0_8:
    case PORT1_8:
        break;
    }
    
    DATA_WRITE(value);
    LAST_VALUE(port_lv, out_port) = value;
    port_set_dir_out();
    
    switch(out_port)
    {
    case PORT0_4_LOW:
    case PORT0_4_HIGH:
    case PORT0_8:
        P0_CP_HIGH();
        P0_CP_LOW();
        break;
        
    case PORT1_4_LOW:
    case PORT1_4_HIGH:
    case PORT1_8:
        P1_CP_HIGH();
        P1_CP_LOW();
        break;
    }
    
    port_set_dir_in();
}


uint8_t port_read(in_port_t in_port)
{
    uint8_t value = 0;
    
    switch(in_port)
    {
    case PORT2_4_LOW:
    case PORT2_4_HIGH:
    case PORT2_8:
        P2_EN_LOW();
        break;
    
    case PORT3_4_LOW:
    case PORT3_4_HIGH:
    case PORT3_8:
        P3_EN_LOW();
        break;
    }
    
    value = DATA_READ();
    
    switch(in_port)
    {
    case PORT2_4_LOW:
    case PORT2_4_HIGH:
    case PORT2_8:
        P2_EN_HIGH();
        break;
    
    case PORT3_4_LOW:
    case PORT3_4_HIGH:
    case PORT3_8:
        P3_EN_HIGH();
        break;
    }
    
    switch(in_port)
    {
    case PORT2_4_LOW:
    case PORT3_4_LOW:
        value &= 0x0F;
        break;
        
    case PORT2_4_HIGH:
    case PORT3_4_HIGH:
        value &= 0xF0;
        break;
    
    case PORT2_8:
    case PORT3_8:
        break;
    }
    
    return value;
}


out_port_t port_get_out_port(uint8_t raw)
{
    out_port_t res;
    
    switch (raw)
    {
    case (0 << 2) | 1:
        res = PORT0_4_LOW;
        break;
    
    case (0 << 2) | 2:
        res = PORT0_4_HIGH;
        break;
    
    case (0 << 2) | 3:
        res = PORT0_8;
        break;
        
    case (1 << 2) | 1:
        res = PORT1_4_LOW;
        break;
    
    case (1 << 2) | 2:
        res = PORT1_4_HIGH;
        break;
    
    case (1 << 2) | 3:
        res = PORT1_8;
        break;
    
    default:
        res = OUT_UNKNOWN;
        break;
    }
    
    return res;
}

in_port_t port_get_in_port(uint8_t raw)
{
    in_port_t res;
    
    switch (raw)
    {
    case (2 << 2) | 1:
        res = PORT2_4_LOW;
        break;
    
    case (2 << 2) | 2:
        res = PORT2_4_HIGH;
        break;
    
    case (2 << 2) | 3:
        res = PORT2_8;
        break;
        
    case (3 << 2) | 1:
        res = PORT3_4_LOW;
        break;
    
    case (3 << 2) | 2:
        res = PORT3_4_HIGH;
        break;
    
    case (3 << 2) | 3:
        res = PORT3_8;
        break;
    
    default:
        res = IN_UNKNOWN;
        break;
    }
    
    return res;    
}
