#include <stdint.h>
#include "devices/msp432/wdt_a.h"

#define PORT_BASE 0x40004C00

typedef struct PORT_t
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

#define PORT1 ((PORT_t*)PORT_BASE)

void Reset_Handler (void)
{
    WDT_A->WDTCTL = 0x5A84;

    PORT1->PDIR |= 1;

    while (1)
    {
        PORT1->POUT ^= 1;
    }
}

/* This is the vector table, which lists the functions pointers that will be called on reset and on interrupts */
__attribute((section(".isr_vector"))) __attribute__((used))
uint32_t *isr_vectors[] =
{
	0,
	(uint32_t *) Reset_Handler,	/* code entry point on CPU reset */
};
