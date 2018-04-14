#include <stdint.h>
#include "devices/msp432/wdt_a.h"

#define PORT_BASE 0x40004C00

typedef struct __attribute__((packed)) PORT_t
{
    volatile uint16_t PIN;
    volatile uint16_t POUT;
    volatile uint16_t PDIR;
    volatile uint16_t PREN;
    volatile uint16_t PDS;
    volatile uint16_t PSEL0;
    volatile uint16_t PSEL1;
    volatile uint16_t Reserved[4];
    volatile uint16_t PSELC;
    volatile uint16_t PIES;
    volatile uint16_t PIE;
    volatile uint16_t PIFG;
} PORT_t;

#define PORTA ((PORT_t*)PORT_BASE)

void main (void)
{
    WDT_A->WDTCTL = 0x5A84;

    PORTA->PDIR |= 0xFFFF;

    while (1)
    {
        PORTA->POUT ^= 0xFFFF;
    }
}
